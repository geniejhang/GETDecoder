#include "GETFrameInfo.hh"

#include <cstdio>
#include <iostream>

using std::cout;
using std::endl;

ClassImp(GETFrameInfo)

        void  GETFrameInfo::SetDataID(UInt_t value)            { fDataID = value; }
        void  GETFrameInfo::SetEventID(UInt_t value)           { fEventID = value; }
        void  GETFrameInfo::SetEventTime(ULong64_t value)      { fEventTime = value; }
        void  GETFrameInfo::SetDeltaT(UInt_t value)            { fDeltaT = value; }
        void  GETFrameInfo::SetStartByte(ULong64_t value)      { fStartByte = value; }
        void  GETFrameInfo::SetEndByte(ULong64_t value)        { fEndByte = value; }
        void  GETFrameInfo::SetNextInfo(GETFrameInfo *pointer) { fNextInfo = pointer; }

      UInt_t  GETFrameInfo::GetDataID()                   { return fDataID; }
      UInt_t  GETFrameInfo::GetEventID()                  { return fEventID; }
   ULong64_t  GETFrameInfo::GetEventTime()                { return fEventTime; }
      UInt_t  GETFrameInfo::GetDeltaT()                   { return fDeltaT; }
   ULong64_t  GETFrameInfo::GetStartByte()                { return fStartByte; }
   ULong64_t  GETFrameInfo::GetEndByte()                  { return fEndByte; }
GETFrameInfo *GETFrameInfo::GetNextInfo()                 { return fNextInfo; }

      Bool_t  GETFrameInfo::IsFill()                      { return (fStartByte != fEndByte); }

void GETFrameInfo::Clear(Option_t *) {
  fDataID    = 0;
  fEventID   = 0;
  fEventTime = 0;
  fDeltaT    = 0;
  fStartByte = 0;
  fEndByte   = 0;
  fNextInfo  = NULL;
}

void GETFrameInfo::Print() {
  cout << "== GETFrameInfo =================" << endl;
  cout << "   fDataID: " << fDataID << endl;
  cout << "  fEventID: " << fEventID << endl;
  cout << "fEventTime: " << fEventTime << endl;
  cout << "   fDeltaT: " << fDeltaT << endl;
  cout << "fStartByte: " << fStartByte << endl;
  cout << "  fEndByte: " << fEndByte << endl;
  cout << " fNextInfo: " << fNextInfo << endl;
  cout << "=================================" << endl;
}
