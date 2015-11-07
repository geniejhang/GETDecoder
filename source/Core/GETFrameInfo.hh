#ifndef GETFRAMEINFO
#define GETFRAMEINFO

#include "TObject.h"

class GETFrameInfo : public TObject {
  public:
         void SetFileID(UInt_t value);
         void SetEventID(UInt_t value);
         void SetEventTime(ULong64_t value);
         void SetDeltaT(UInt_t value);
         void SetStartByte(ULong64_t value);
         void SetEndByte(ULong64_t value);

       UInt_t GetFileID();
       UInt_t GetEventID();
    ULong64_t GetEventTime();
       UInt_t GetDeltaT();
    ULong64_t GetStartByte();
    ULong64_t GetEndByte();

       Bool_t IsFill();
         void Clear(Option_t * = "");

  private:
       UInt_t fFileID;
       UInt_t fEventID;
    ULong64_t fEventTime;
       UInt_t fDeltaT;
    ULong64_t fStartByte;
    ULong64_t fEndByte;

  ClassDef(GETFrameInfo, 1)
};

#endif
