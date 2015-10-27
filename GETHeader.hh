#ifndef GETHEADER
#define GETHEADER

#include <iostream>
#include <iomanip>
#include <arpa/inet.h>
#include <cmath>
#include <bitset>

using std::cout;
using std::endl;
using std::hex;
using std::dec;
using std::showbase;
using std::bitset;

class GETHeaderBase {
  public:
       UInt_t GetMetaType();                       
       UInt_t GetFrameSize(Bool_t inBytes = kTRUE);
       UInt_t GetDataSource();
       UInt_t GetFrameType();
       UInt_t GetRevision();
      ULong_t GetFrameSkip();
  
       Bool_t IsLittleEndian();
       Bool_t IsBlob();
       UInt_t GetUnitBlock();

    ULong64_t CorrectEndianness(uint8_t *variable, Short_t length);

    void Print();

  private:
    uint8_t fMetaType;
    uint8_t fFrameSize[3];
    uint8_t fDataSource;
    uint8_t fFrameType[2];
    uint8_t fRevision;
};

class GETBlobHeader : public GETHeaderBase {
  public:
     UInt_t GetCoboIdx();
     UInt_t GetAsadMask();
     UInt_t Get2pMode();
     UInt_t GetUNUSED();
    ULong_t GetFrameSkip();

    void Print();

  private:
    uint8_t fCoboIdx;
    uint8_t fAsadMask;
    uint8_t fTwopMode;
    uint8_t fUNUSED;
};

class GETLayerHeader : public GETHeaderBase {
  public:
     UInt_t GetHeaderSize(Bool_t inBytes = kTRUE);
     UInt_t GetItemSize();
     UInt_t GetNItems();
    ULong_t GetFrameSkip();
     UInt_t GetHeaderSkip();

    void Print();

  private:
    uint8_t fHeaderSize[2];
    uint8_t fItemSize[2];
    uint8_t fNItems[4];
};

#endif
