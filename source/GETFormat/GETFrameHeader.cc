#include "GETFrameHeader.hh"

ClassImp(GETFrameHeader)

 UInt_t GETFrameHeader::GetHeaderSize(Bool_t inBytes)  { return CorrectEndianness(fHeaderSize, 2)*(inBytes ? GetUnitBlock() : 1); }
 UInt_t GETFrameHeader::GetItemSize()                  { return CorrectEndianness(fItemSize, 2); }
 UInt_t GETFrameHeader::GetNItems()                    { return CorrectEndianness(fNItems, 4); }
ULong_t GETFrameHeader::GetEventTime()                 { return CorrectEndianness(fEventTime, 6); }
 UInt_t GETFrameHeader::GetEventIdx()                  { return CorrectEndianness(fEventIdx, 4); }
 UInt_t GETFrameHeader::GetCoboIdx()                   { return (UInt_t) fCoboIdx; }
 UInt_t GETFrameHeader::GetAsadIdx()                   { return (UInt_t) fAsadIdx; }
 UInt_t GETFrameHeader::GetReadOffset()                { return CorrectEndianness(fReadOffset, 2); }
 UInt_t GETFrameHeader::GetStatus()                    { return (UInt_t) fStatus; }

bitset<72> GETFrameHeader::GetHitPat(Int_t asadIdx) {
  bitset<72> tempHitPat;

  if (!IsLittleEndian()) {
    for (Int_t iByte = 0; iByte < 9; iByte++)
      for (Int_t iBit = 0; iBit < 8; iBit++)
        if ((fHitPat[asadIdx][iByte] >> iBit)&0x1)
          tempHitPat.set((9 - iByte - 1)*8 + iBit);
  } else {
    for (Int_t iByte = 0; iByte < 9; iByte++)
      for (Int_t iBit = 0; iBit < 8; iBit++)
        if ((fHitPat[asadIdx][iByte] >> iBit)&0x1)
          tempHitPat.set(iByte*8 + iBit);
  }

  return tempHitPat;
}

UInt_t GETFrameHeader::GetMultip(Int_t asadIdx) {
  uint8_t *tempMultip = fMultip[0] + 2*asadIdx;
  return CorrectEndianness(tempMultip, 2);
}

UInt_t GETFrameHeader::GetWindowOut()                  { return CorrectEndianness(fWindowOut, 4); }
UInt_t GETFrameHeader::GetLastCell(Int_t asadIdx) {
  uint8_t *tempLastCell = fLastCell[0] + 2*asadIdx;

  return CorrectEndianness(tempLastCell, 2);
}

ULong64_t GETFrameHeader::GetFrameSkip()               { return GetFrameSize() - GETFRAMEHEADERSIZE; }
   UInt_t GETFrameHeader::GetHeaderSkip()              { return GetHeaderSize() - GETFRAMEHEADERSIZE; }

void GETFrameHeader::Clear() {
  GETHeaderBase::Clear();

  memset( fHeaderSize, 0, sizeof(uint8_t)*  2);
  memset( fItemSize,   0, sizeof(uint8_t)*  2);
  memset( fNItems,     0, sizeof(uint8_t)*  4);
  memset( fEventTime,  0, sizeof(uint8_t)*  6);
  memset( fEventIdx,   0, sizeof(uint8_t)*  4);
  memset(&fCoboIdx,    0, sizeof(uint8_t)*  1);
  memset(&fAsadIdx,    0, sizeof(uint8_t)*  1);
  memset( fReadOffset, 0, sizeof(uint8_t)*  2);
  memset(&fStatus,     0, sizeof(uint8_t)*  1);
  memset( fHitPat,     0, sizeof(uint8_t)*4*9);
  memset( fMultip,     0, sizeof(uint8_t)*4*2);
  memset( fWindowOut,  0, sizeof(uint8_t)*  4);
  memset( fLastCell,   0, sizeof(uint8_t)*4*2);
}

void GETFrameHeader::Read(ifstream &stream) {
  GETHeaderBase::Read(stream);

  stream.read((Char_t *)   fHeaderSize,   2);
  stream.read((Char_t *)     fItemSize,   2);
  stream.read((Char_t *)       fNItems,   4);
  stream.read((Char_t *)    fEventTime,   6);
  stream.read((Char_t *)     fEventIdx,   4);
  stream.read((Char_t *) &    fCoboIdx,   1);
  stream.read((Char_t *) &    fAsadIdx,   1);
  stream.read((Char_t *)   fReadOffset,   2);
  stream.read((Char_t *) &     fStatus,   1);
  stream.read((Char_t *)       fHitPat, 4*9);
  stream.read((Char_t *)       fMultip, 4*2);
  stream.read((Char_t *)    fWindowOut,   4);
  stream.read((Char_t *)     fLastCell, 4*2);

  stream.ignore(GetHeaderSkip());
}

void GETFrameHeader::Print() {
  cout << showbase << hex;
  cout << " == GETFrameHeader ========================================================================" << endl;
  cout << "    metaType: " << GetMetaType() << endl;
  cout << "              - Endianness: " << (IsLittleEndian() ? "Little" : "Big") << endl;
  cout << "              -   Blobness: " << (IsBlob() ? "YES" : "NO") << endl;
  cout << "              -  UnitBlock: " << dec << GetUnitBlock() << " Bytes" << hex << endl;
  cout << "   frameSize: " << GetFrameSize(kFALSE) << " (" << dec << GetFrameSize(kFALSE) << " Blocks = " << GetFrameSize() << hex << " Bytes)" << endl;
  cout << "  dataSource: " << GetDataSource() << endl;
  cout << "   frameType: " << GetFrameType() << endl;
  cout << "    revision: " << GetRevision() << endl;
  cout << "  headerSize: " << GetHeaderSize(kFALSE) << " (" << dec << GetHeaderSize(kFALSE) << " Blocks = " << GetHeaderSize() << hex << " Bytes)" << endl;
  cout << "    itemSize: " << GetItemSize() << endl;
  cout << "      nItems: " << GetNItems() << dec << " (" << GetNItems() << ")" << hex << endl;
  cout << "   eventTime: " << dec << GetEventTime() << endl;
  cout << "    eventIdx: " << GetEventIdx() << endl;
  cout << "     coboIdx: " << GetCoboIdx() << endl;
  cout << "     asadIdx: " << GetAsadIdx() << endl;
  cout << "  readOffset: " << GetReadOffset() << endl;
  cout << "      status: " << GetStatus() << endl;
  for (Int_t iAsad = 0; iAsad < 4; iAsad++)
    cout << "    hitPat_" << iAsad << ": " << GetHitPat(iAsad) << endl;
  for (Int_t iAsad = 0; iAsad < 4; iAsad++)
    cout << "    multip_" << iAsad << ": " << GetMultip(iAsad) << endl;
  cout << "   windowOut: " << GetWindowOut() << endl;
  for (Int_t iAsad = 0; iAsad < 4; iAsad++)
    cout << "  lastCell_" << iAsad << ": " << GetLastCell(iAsad) << endl;
  cout << " ==========================================================================================" << endl;
}
