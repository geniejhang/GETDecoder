#ifndef GETLAYEREDFRAME
#define GETLAYEREDFRAME

#include "GETLayerHeader.hh"
#include "GETFrame.hh"

#include "TClonesArray.h"

class GETLayeredFrame : public GETLayerHeader {
  public:
    GETLayeredFrame();

           Int_t  GetNumFrames();
    TClonesArray *GetFrames();
        GETFrame *GetFrame(Int_t index);

            void  Clear();
            void  Read(ifstream &stream);

  private:
    TClonesArray *fFrames;

  ClassDef(GETLayeredFrame, 1);
};

#endif
