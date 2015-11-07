#include "GETFrameInfo.hh"

#include <cstdio>

ClassImp(GETFrameInfo)

     void GETFrameInfo::SetFileID(UInt_t value)       { fFileID = value; }
     void GETFrameInfo::SetEventID(UInt_t value)      { fEventID = value; }
     void GETFrameInfo::SetEventTime(ULong64_t value) { fEventTime = value; }
     void GETFrameInfo::SetDeltaT(UInt_t value)       { fDeltaT = value; }
     void GETFrameInfo::SetStartByte(ULong64_t value) { fStartByte = value; }
     void GETFrameInfo::SetEndByte(ULong64_t value)   { fEndByte = value; }

   UInt_t GETFrameInfo::GetFileID()                   { return fFileID; }
   UInt_t GETFrameInfo::GetEventID()                  { return fEventID; }
ULong64_t GETFrameInfo::GetEventTime()                { return fEventTime; }
   UInt_t GETFrameInfo::GetDeltaT()                   { return fDeltaT; }
ULong64_t GETFrameInfo::GetStartByte()                { return fStartByte; }
ULong64_t GETFrameInfo::GetEndByte()                  { return fEndByte; }

   Bool_t GETFrameInfo::IsFill()                      { return (fStartByte != fEndByte); }

void GETFrameInfo::Clear(Option_t *) {
  fFileID    = 0;
  fEventID   = 0;
  fEventTime = 0;
  fDeltaT    = 0;
  fStartByte = 0;
  fEndByte   = 0;
}
