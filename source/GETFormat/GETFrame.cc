#include "GETFrame.hh"

Int_t *GETFrame::GetRawADC(Int_t agetIdx, Int_t chIdx) { return fRawADC + GetIndex(agetIdx, chIdx, 0); }

    void  GETFrame::SetADC(Int_t agetIdx, Int_t chIdx, Int_t tbIdx, Double_t adc) { fADC[GetIndex(agetIdx, chIdx, tbIdx)] = adc; }
    void  GETFrame::SetADC(Int_t agetIdx, Int_t chIdx, Double_t *adc, Int_t tbsSize) { memcpy(fADC + GetIndex(agetIdx, chIdx, 0), adc, sizeof(Double_t)*tbsSize); }
Double_t *GETFrame::GetADC(Int_t agetIdx, Int_t chIdx) { return fADC + GetIndex(agetIdx, chIdx, 0); }

Int_t GETFrame::GetFrameSkip() { return GetFrameSize() - GETFRAMEHEADERSIZE - GetHeaderSkip() - GetItemSize()*GetNItems(); }

void GETFrame::Clear() {
  GETFrameHeader::Clear();

  memset(fRawADC, 0, sizeof(Int_t)*4*68*512);
  memset(fADC,    0, sizeof(Int_t)*4*68*512);
}

void GETFrame::Read(ifstream &stream) {
  Clear();

  GETFrameHeader::Read(stream);

  if (GetFrameType() == GETFRAMEBASICTYPE1) {
    uint8_t data[4];
    for (Int_t iItem = 0; iItem < GetNItems(); iItem++) {
      stream.read((Char_t *) data, GetItemSize());

      UInt_t item = CorrectEndianness(data, 4);

      UShort_t agetIdx = ((item & 0xc0000000) >> 30);
      UShort_t   chIdx = ((item & 0x3f800000) >> 23);
      UShort_t   tbIdx = ((item & 0x007fc000) >> 14);
      UShort_t  rawADC =  (item & 0x00000fff);         

      fRawADC[GetIndex(agetIdx, chIdx, tbIdx)] = rawADC; 
    }
  } else if (GetFrameType() == GETFRAMEBASICTYPE2) {
    uint8_t data[2];
    for (Int_t iItem = 0; iItem < GetNItems(); iItem++) {
      stream.read((Char_t *) data, GetItemSize());

      UShort_t item = CorrectEndianness(data, 2);

      UShort_t agetIdx = ((item & 0xc000) >> 14);
      UShort_t   chIdx = ((iItem/8)*2 + iItem%2)%68;
      UShort_t   tbIdx = iItem/(68*4);
      UShort_t  rawADC = item & 0x0fff;

      fRawADC[GetIndex(agetIdx, chIdx, tbIdx)] = rawADC; 
    }
  }

  stream.ignore(GetFrameSkip());
}

UInt_t GETFrame::GetIndex(Int_t agetIdx, Int_t chIdx, Int_t tbIdx) { return agetIdx*68*512 + chIdx*512 + tbIdx; }
