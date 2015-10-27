#ifndef GETHEADERBASE
#define GETHEADERBASE

#include "Rtypes.h"

#include <fstream>
#include <iostream>
#include <bitset>
#include <cmath>

using namespace std;

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

    void Read(ifstream &file);

    void Print();

  private:
    uint8_t fMetaType;
    uint8_t fFrameSize[3];
    uint8_t fDataSource;
    uint8_t fFrameType[2];
    uint8_t fRevision;

  ClassDef(GETHeaderBase, 1)
};

#endif
