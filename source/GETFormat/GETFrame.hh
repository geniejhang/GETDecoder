#ifndef GETFRAME
#define GETFRAME

#include "GETFrameHeader.hh"

class GETFrame : public GETFrameHeader {
  public:
       Int_t *GetRawADC(Int_t agetIdx, Int_t chIdx);

        void  SetADC(Int_t agetIdx, Int_t chIdx, Int_t tbIdx, Double_t adc);
        void  SetADC(Int_t agetIdx, Int_t chIdx, Double_t *adc, Int_t tbsSize = 512);
    Double_t *GetADC(Int_t agetIdx, Int_t chIdx);

       Int_t  GetFrameSkip();

        void  Clear();
        void  Read(ifstream &stream);

  private:
       Int_t fRawADC[4*68*512];
    Double_t fADC[4*68*512];

      UInt_t GetIndex(Int_t agetIdx, Int_t chIdx, Int_t tbIdx);

  ClassDef(GETFrame, 1)
};

#endif
