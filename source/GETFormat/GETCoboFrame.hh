#ifndef GETCOBOFRAME
#define GETCOBOFRAME

#include "GETBasicFrame.hh"

class GETCoboFrame : public TObject {
  public:
    GETCoboFrame();

             void  AddFrame(GETBasicFrame *frame);
             void  SetFrame(Int_t index, GETBasicFrame *frame);

            Int_t  GetNumFrames();
    GETBasicFrame *GetFrames();
    GETBasicFrame *GetFrame(Int_t index);

             void  Clear(Option_t * = "");

  private:
            Int_t  fNumFrames;
    GETBasicFrame  fFrame[4];

  ClassDef(GETCoboFrame, 1);
};

#endif
