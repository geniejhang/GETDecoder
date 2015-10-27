#ifndef GETFRAMEHEADER
#define GETFRAMEHEADER

#include "GETHeaderBase.hh"

#include <bitset>

using std::bitset;

class GETFrameHeader : public GETHeaderBase {
  public:
        UInt_t GetHeaderSize(Bool_t inBytes = kTRUE);
        UInt_t GetItemSize();
        UInt_t GetNItems();
       ULong_t GetEventTime();
        UInt_t GetEventIdx();
        UInt_t GetCoboIdx();
        UInt_t GetAsadIdx();
        UInt_t GetReadOffset();
        UInt_t GetStatus();
    bitset<72> GetHitPat(Int_t asadIdx);
        UInt_t GetMultip(Int_t asadIdx);
        UInt_t GetWindowOut();
        UInt_t GetLastCell(Int_t asadIdx);
       ULong_t GetFrameSkip();
        UInt_t GetHeaderSkip();

    void Read(ifstream &stream);

    void Print();

  private:
    uint8_t fHeaderSize[2];
    uint8_t fItemSize[2];
    uint8_t fNItems[4];
    uint8_t fEventTime[6];
    uint8_t fEventIdx[4];
    uint8_t fCoboIdx;
    uint8_t fAsadIdx;
    uint8_t fReadOffset[2];
    uint8_t fStatus;
    uint8_t fHitPat[4][9];
    uint8_t fMultip[4][2];
    uint8_t fWindowOut[4];
    uint8_t fLastCell[4][2];

  ClassDef(GETFrameHeader, 1)
};

#endif
