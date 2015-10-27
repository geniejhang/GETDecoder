#ifndef GETLAYERHEADER
#define GETLAYERHEADER

#include "GETHeaderBase.hh"

class GETLayerHeader : public GETHeaderBase {
  public:
     UInt_t GetHeaderSize(Bool_t inBytes = kTRUE);
     UInt_t GetItemSize();
     UInt_t GetNItems();
    ULong_t GetFrameSkip();
     UInt_t GetHeaderSkip();

    void Read(ifstream &stream);

    void Print();

  private:
    uint8_t fHeaderSize[2];
    uint8_t fItemSize[2];
    uint8_t fNItems[4];

  ClassDef(GETLayerHeader, 1)
};

#endif
