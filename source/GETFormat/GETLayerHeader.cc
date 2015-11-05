#include "GETLayerHeader.hh"

ClassImp(GETLayerHeader)

   UInt_t GETLayerHeader::GetHeaderSize(Bool_t inBytes)  { return CorrectEndianness(fHeaderSize, 2)*(inBytes ? GetUnitBlock() : 1); }
   UInt_t GETLayerHeader::GetItemSize()                  { return CorrectEndianness(fItemSize, 2); }
   UInt_t GETLayerHeader::GetNItems()                    { return CorrectEndianness(fNItems, 4); }
ULong64_t GETLayerHeader::GetFrameSkip()                 { return GetFrameSize() - GetHeaderSize(); }
   UInt_t GETLayerHeader::GetHeaderSkip()                { return GetHeaderSize() - GETLAYERHEADERSIZE; }
 
void GETLayerHeader::Clear() {
  GETHeaderBase::Clear();

  memset(fHeaderSize, 0, sizeof(uint8_t)*2);
  memset(fItemSize,   0, sizeof(uint8_t)*2);
  memset(fNItems,     0, sizeof(uint8_t)*4);
}

void GETLayerHeader::Read(ifstream &stream) {
  GETHeaderBase::Read(stream);

  stream.read((Char_t *) fHeaderSize, 2);
  stream.read((Char_t *)   fItemSize, 2);
  stream.read((Char_t *)     fNItems, 4);

  stream.ignore(GetHeaderSkip());
}

void GETLayerHeader::Print() {
  cout << showbase << hex;
  cout << " == GETLayerHeader ======================" << endl;
  cout << "    metaType: " << GetMetaType() << endl;
  cout << "              - Endianness: " << (IsLittleEndian() ? "Little" : "Big") << endl;
  cout << "              -   Blobness: " << (IsBlob() ? "YES" : "NO") << endl;
  cout << "              -  UnitBlock: " << dec << GetUnitBlock() << hex << endl;
  cout << "   frameSize: " << GetFrameSize(kFALSE) << " (" << dec << GetFrameSize(kFALSE) << " Blocks = " << GetFrameSize() << hex << " Bytes)" << endl;
  cout << "  dataSource: " << GetDataSource() << endl;
  cout << "   frameType: " << GetFrameType() << endl;
  cout << "    revision: " << GetRevision() << endl;
  cout << "  headerSize: " << GetHeaderSize(kFALSE) << " (" << dec << GetHeaderSize(kFALSE) << " Blocks = " << GetHeaderSize() << hex << " Bytes)" << endl;
  cout << "    itemSize: " << GetItemSize() << endl;
  cout << "      nItems: " << GetNItems() << dec << " (" << GetNItems() << ")" << hex << endl;
  cout << " ========================================" << endl;
}
