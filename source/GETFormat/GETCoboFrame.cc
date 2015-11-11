#include "GETCoboFrame.hh"

GETCoboFrame::GETCoboFrame() {
  Clear();
}

void GETCoboFrame::AddFrame(GETBasicFrame *frame) {
  fFrame[fNumFrames++] = *frame;
}

void GETCoboFrame::SetFrame(Int_t index, GETBasicFrame *frame) {
  fFrame[index].Clear();
  fFrame[index] = *frame;
}

        Int_t  GETCoboFrame::GetNumFrames()        { return fNumFrames; }
GETBasicFrame *GETCoboFrame::GetFrames()           { return fFrame; }
GETBasicFrame *GETCoboFrame::GetFrame(Int_t index) { return &fFrame[index]; }

void GETCoboFrame::Clear(Option_t *) {
  for (Int_t iFrame = 0; iFrame < fNumFrames; iFrame++)
    fFrame[iFrame].Clear();

  fNumFrames = 0;
}
