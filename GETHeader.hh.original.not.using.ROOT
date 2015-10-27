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
    unsigned  int GetMetaType()                       { return (unsigned int) metaType; }
    unsigned  int GetFrameSize(bool inBytes = true)   { return CorrectEndianness(frameSize, 3)*(inBytes ? GetUnitBlock() : 1); }
    unsigned  int GetDataSource()                     { return (unsigned int) dataSource; }
    unsigned  int GetFrameType()                      { return CorrectEndianness(frameType, 2); }
    unsigned  int GetRevision()                       { return (unsigned int) revision; }
    unsigned long GetFrameSkip()                      { return GetFrameSize() - sizeof(GETHeaderBase); }

             bool IsLittleEndian()                    { return ((GetMetaType()&0x80) >> 7); }
             bool IsBlob()                            { return ((GetMetaType()&0x40) >> 6); }
    unsigned  int GetUnitBlock()                      { return pow(2, GetMetaType()&0xf); }

    unsigned long long CorrectEndianness(uint8_t *variable, short length) {
      unsigned long long returnVal = 0;

      if (!IsLittleEndian())
        for (short idx = 0; idx < length; idx++)
          returnVal += (variable[idx] << (8*(length - idx - 1)));
      else
        for (short idx = 0; idx < length; idx++)
          returnVal += (variable[(length - idx - 1)] << (8*(length - idx - 1)));

      return returnVal;
    }

    void Print() {
      cout << showbase << hex;
      cout << " == GETHeaderBase ================================" << endl;
      cout << "    metaType: " << GetMetaType() << endl;
      cout << "              - Endianness: " << (IsLittleEndian() ? "Little" : "Big") << endl;
      cout << "              -   Blobness: " << (IsBlob() ? "YES" : "NO") << endl;
      cout << "              -  UnitBlock: " << dec << GetUnitBlock() << " Bytes" << hex << endl;
      cout << "   frameSize: " << GetFrameSize(false) << " (" << dec << GetFrameSize(false) << " Blocks = " << GetFrameSize() << hex << " Bytes)" << endl;
      cout << "  dataSource: " << GetDataSource() << endl;
      cout << "   frameType: " << GetFrameType() << endl;
      cout << "    revision: " << GetRevision() << endl;
      cout << " =================================================" << endl;
    }

  private:
    uint8_t metaType;
    uint8_t frameSize[3];
    uint8_t dataSource;
    uint8_t frameType[2];
    uint8_t revision;
};

class GETBlobHeader : public GETHeaderBase {
  public:
    unsigned  int GetCoboIdx()   { return (unsigned int) coboIdx; }
    unsigned  int GetAsadMask()  { return (unsigned int) asadMask; }
    unsigned  int Get2pMode()    { return (unsigned int) twopMode; }
    unsigned  int GetUNUSED()    { return (unsigned int) UNUSED; }
    unsigned long GetFrameSkip() { return 0; }

    void Print() {
      cout << showbase << hex;
      cout << " == GETBlobHeader =======================" << endl;
      cout << "    metaType: " << GetMetaType() << endl;
      cout << "              - Endianness: " << (IsLittleEndian() ? "Little" : "Big") << endl;
      cout << "              -   Blobness: " << (IsBlob() ? "YES" : "NO") << endl;
      cout << "              -  UnitBlock: " << dec << GetUnitBlock() << " Bytes" << hex << endl;
      cout << "   frameSize: " << GetFrameSize(false) << " (" << dec << GetFrameSize(false) << " Blocks = " << GetFrameSize() << hex << " Bytes)" << endl;
      cout << "  dataSource: " << GetDataSource() << endl;
      cout << "   frameType: " << GetFrameType() << endl;
      cout << "    revision: " << GetRevision() << endl;
      cout << "     coboIdx: " << GetCoboIdx() << endl;
      cout << "    asadMask: " << GetAsadMask() << " (" << bitset<4>(GetAsadMask()) << ")" << endl;
      cout << "      2pMode: " << Get2pMode() << endl;
      cout << "      UNUSED: " << GetUNUSED() << endl;
      cout << " ========================================" << endl;
    }

  private:
    uint8_t coboIdx;
    uint8_t asadMask;
    uint8_t twopMode;
    uint8_t UNUSED;
};

class GETFrameHeader : public GETHeaderBase {
  public:
    unsigned  int GetHeaderSize(bool inBytes = true)  { return CorrectEndianness(headerSize, 2)*(inBytes ? GetUnitBlock() : 1); }
    unsigned  int GetItemSize()                       { return CorrectEndianness(itemSize, 2); }
    unsigned  int GetNItems()                         { return CorrectEndianness(nItems, 4); }
    unsigned long GetEventTime()                      { return CorrectEndianness(eventTime, 6); }
    unsigned  int GetEventIdx()                       { return CorrectEndianness(eventIdx, 4); }
    unsigned  int GetCoboIdx()                        { return (unsigned int) coboIdx; }
    unsigned  int GetAsadIdx()                        { return (unsigned int) asadIdx; }
    unsigned  int GetReadOffset()                     { return CorrectEndianness(readOffset, 2); }
    unsigned  int GetStatus()                         { return (unsigned int) status; }
       bitset<72> GetHitPat(int asadIdx)              {
                                                        bitset<72> tempHitPat;
                                              
                                                        if (!IsLittleEndian()) {
                                                          for (int iByte = 0; iByte < 9; iByte++)
                                                            for (int iBit = 0; iBit < 8; iBit++)
                                                              if ((hitPat[asadIdx][iByte] >> iBit)&0x1)
                                                                tempHitPat.set((9 - iByte - 1)*8 + iBit);
                                                        } else {
                                                          for (int iByte = 0; iByte < 9; iByte++)
                                                            for (int iBit = 0; iBit < 8; iBit++)
                                                              if ((hitPat[asadIdx][iByte] >> iBit)&0x1)
                                                                tempHitPat.set(iByte*8 + iBit);
                                                        }

                                                        return tempHitPat;
                                                      }
    unsigned  int GetMultip(int asadIdx)              {
                                                        uint8_t *tempMultip = multip[0] + 2*asadIdx;
                                                        return CorrectEndianness(tempMultip, 2);
                                                      }
    unsigned  int GetWindowOut()                      { return CorrectEndianness(windowOut, 4); }
    unsigned  int GetLastCell(int asadIdx)            {
                                                        uint8_t *tempLastCell = lastCell[0] + 2*asadIdx;
                                                        return CorrectEndianness(tempLastCell, 2);
                                                      }
    unsigned long GetFrameSkip()                      { return GetFrameSize() - sizeof(GETFrameHeader); }
    unsigned  int GetHeaderSkip()                     { return GetHeaderSize() - sizeof(GETFrameHeader); }

    void Print() {
      cout << showbase << hex;
      cout << " == GETFrameHeader ========================================================================" << endl;
      cout << "    metaType: " << GetMetaType() << endl;
      cout << "              - Endianness: " << (IsLittleEndian() ? "Little" : "Big") << endl;
      cout << "              -   Blobness: " << (IsBlob() ? "YES" : "NO") << endl;
      cout << "              -  UnitBlock: " << dec << GetUnitBlock() << " Bytes" << hex << endl;
      cout << "   frameSize: " << GetFrameSize(false) << " (" << dec << GetFrameSize(false) << " Blocks = " << GetFrameSize() << hex << " Bytes)" << endl;
      cout << "  dataSource: " << GetDataSource() << endl;
      cout << "   frameType: " << GetFrameType() << endl;
      cout << "    revision: " << GetRevision() << endl;
      cout << "  headerSize: " << GetHeaderSize(false) << " (" << dec << GetHeaderSize(false) << " Blocks = " << GetHeaderSize() << hex << " Bytes)" << endl;
      cout << "    itemSize: " << GetItemSize() << endl;
      cout << "      nItems: " << GetNItems() << dec << " (" << GetNItems() << ")" << hex << endl;
      cout << "   eventTime: " << dec << GetEventTime() << endl;
      cout << "    eventIdx: " << GetEventIdx() << endl;
      cout << "     coboIdx: " << GetCoboIdx() << endl;
      cout << "     asadIdx: " << GetAsadIdx() << endl;
      cout << "  readOffset: " << GetReadOffset() << endl;
      cout << "      status: " << GetStatus() << endl;
      for (int iAsad = 0; iAsad < 4; iAsad++)
        cout << "    hitPat_" << iAsad << ": " << GetHitPat(iAsad) << endl;
      for (int iAsad = 0; iAsad < 4; iAsad++)
        cout << "    multip_" << iAsad << ": " << GetMultip(iAsad) << endl;
      cout << "   windowOut: " << GetWindowOut() << endl;
      for (int iAsad = 0; iAsad < 4; iAsad++)
        cout << "  lastCell_" << iAsad << ": " << GetLastCell(iAsad) << endl;
      cout << " ==========================================================================================" << endl;
    }

  private:
    uint8_t headerSize[2];
    uint8_t itemSize[2];
    uint8_t nItems[4];
    uint8_t eventTime[6];
    uint8_t eventIdx[4];
    uint8_t coboIdx;
    uint8_t asadIdx;
    uint8_t readOffset[2];
    uint8_t status;
    uint8_t hitPat[4][9];
    uint8_t multip[4][2];
    uint8_t windowOut[4];
    uint8_t lastCell[4][2];
};

class GETLayerHeader : public GETHeaderBase {
  public:
    unsigned  int GetHeaderSize(bool inBytes = true)  { return CorrectEndianness(headerSize, 2)*(inBytes ? GetUnitBlock() : 1); }
    unsigned  int GetItemSize()                       { return CorrectEndianness(itemSize, 2); }
    unsigned  int GetNItems()                         { return CorrectEndianness(nItems, 4); }
    unsigned long GetFrameSkip()                      { return GetFrameSize() - sizeof(GETLayerHeader); }
    unsigned  int GetHeaderSkip()                     { return GetHeaderSize() - sizeof(GETLayerHeader); }

    void Print() {
      cout << showbase << hex;
      cout << " == GETLayerHeader ======================" << endl;
      cout << "    metaType: " << GetMetaType() << endl;
      cout << "              - Endianness: " << (IsLittleEndian() ? "Little" : "Big") << endl;
      cout << "              -   Blobness: " << (IsBlob() ? "YES" : "NO") << endl;
      cout << "              -  UnitBlock: " << dec << GetUnitBlock() << hex << endl;
      cout << "   frameSize: " << GetFrameSize(false) << " (" << dec << GetFrameSize(false) << " Blocks = " << GetFrameSize() << hex << " Bytes)" << endl;
      cout << "  dataSource: " << GetDataSource() << endl;
      cout << "   frameType: " << GetFrameType() << endl;
      cout << "    revision: " << GetRevision() << endl;
      cout << "  headerSize: " << GetHeaderSize(false) << " (" << dec << GetHeaderSize(false) << " Blocks = " << GetHeaderSize() << hex << " Bytes)" << endl;
      cout << "    itemSize: " << GetItemSize() << endl;
      cout << "      nItems: " << GetNItems() << dec << " (" << GetNItems() << ")" << hex << endl;
      cout << " ========================================" << endl;
    }

  private:
    uint8_t headerSize[2];
    uint8_t itemSize[2];
    uint8_t nItems[4];
};

#endif
