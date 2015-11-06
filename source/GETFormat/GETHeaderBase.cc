#include "GETHeaderBase.hh"

ClassImp(GETHeaderBase)

   UInt_t GETHeaderBase::GetMetaType()                 { return (UInt_t) fMetaType; }
   UInt_t GETHeaderBase::GetFrameSize(Bool_t inBytes)  { return CorrectEndianness(fFrameSize, 3)*(inBytes ? GetUnitBlock() : 1); }
   UInt_t GETHeaderBase::GetDataSource()               { return (UInt_t) fDataSource; }
   UInt_t GETHeaderBase::GetFrameType()                { return CorrectEndianness(fFrameType, 2); }
   UInt_t GETHeaderBase::GetRevision()                 { return (UInt_t) fRevision; }
ULong64_t GETHeaderBase::GetFrameSkip(Bool_t rewind)   { return GetFrameSize() - GETHEADERBASESIZE*(!rewind); }

   Bool_t GETHeaderBase::IsLittleEndian()              { return ((GetMetaType()&0x80) >> 7); }
   Bool_t GETHeaderBase::IsBlob()                      { return ((GetMetaType()&0x40) >> 6); }
   UInt_t GETHeaderBase::GetUnitBlock()                { return pow(2, GetMetaType()&0xf); }

ULong64_t GETHeaderBase::CorrectEndianness(uint8_t *variable, Short_t length) {
  ULong64_t returnVal = 0;

  if (!IsLittleEndian())
    for (Short_t idx = 0; idx < length; idx++)
      returnVal += (variable[idx] << (8*(length - idx - 1)));
  else
    for (Short_t idx = 0; idx < length; idx++)
      returnVal += (variable[(length - idx - 1)] << (8*(length - idx - 1)));

  return returnVal;
}

void GETHeaderBase::Clear() {
  memset(&fMetaType,   0, sizeof(uint8_t)*1);
  memset( fFrameSize,  0, sizeof(uint8_t)*3);
  memset(&fDataSource, 0, sizeof(uint8_t)*1);
  memset( fFrameType,  0, sizeof(uint8_t)*2);
  memset(&fRevision,   0, sizeof(uint8_t)*1);
}

void GETHeaderBase::Read(ifstream &stream, Bool_t rewind) {
  Clear();

  stream.read((Char_t *) &   fMetaType, 1);
  stream.read((Char_t *)    fFrameSize, 3);
  stream.read((Char_t *) & fDataSource, 1);
  stream.read((Char_t *)    fFrameType, 2);
  stream.read((Char_t *) &   fRevision, 1);

  stream.seekg((ULong64_t) stream.tellg() - GETHEADERBASESIZE*rewind);
}

void GETHeaderBase::Print() {
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