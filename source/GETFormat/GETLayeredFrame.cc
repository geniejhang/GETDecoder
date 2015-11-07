#include "GETLayeredFrame.hh"

GETLayeredFrame::GETLayeredFrame() {
  fFrames = NULL;

  Clear();
}

       Int_t  GETLayeredFrame::GetNumFrames()        { return GetNItems(); }
TClonesArray *GETLayeredFrame::GetFrames()           { return fFrames; }
    GETFrame *GETLayeredFrame::GetFrame(Int_t index) { return (GETFrame * ) fFrames -> At(index); }

void GETLayeredFrame::Clear(Option_t *) {
  if (fFrames == NULL)
    fFrames = new TClonesArray("GETFrame", 48);

  GETLayerHeader::Clear();
  fFrames -> Clear("C");
}

void GETLayeredFrame::Read(ifstream &stream) {
  Clear();

  GETLayerHeader::Read(stream);

  for (Int_t iFrame = 0; iFrame < GetNItems(); iFrame++) {
    GETFrame *frame = (GETFrame *) fFrames -> ConstructedAt(iFrame);
    frame -> Read(stream);
  }
}
