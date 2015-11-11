// =================================================
//  GETDecoder Class
// 
//  Author:
//    Genie Jhang ( geniejhang@majimak.com )
//  
//  Log:
//    - 2013. 09. 23
//      Start writing class
// =================================================

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <arpa/inet.h>

#include "TString.h"

#include "GETDecoder.hh"

//#include "GETPlot.hh"
#include "GETFileChecker.hh"

#define DEBUG
#define MESSAGE_ORIGIN __FILE__,__FUNC__,__LINE__
#define LOG(DEBUG) cout

ClassImp(GETDecoder);

GETDecoder::GETDecoder()
:fFrameInfoArray(NULL), fHeaderBase(NULL), fBasicFrameHeader(NULL), fLayerHeader(NULL), fTopologyFrame(NULL), fBasicFrame(NULL), fCoboFrame(NULL), fLayeredFrame(NULL), fFrameInfo(NULL)
{
  /**
    * If you use this constructor, you have to add the rawdata using
    * AddData() method and set the file with SetData() method, manually.
   **/
   
  Initialize();
}

GETDecoder::GETDecoder(TString filename)
:fFrameInfoArray(NULL), fHeaderBase(NULL), fBasicFrameHeader(NULL), fLayerHeader(NULL), fTopologyFrame(NULL), fBasicFrame(NULL), fCoboFrame(NULL), fLayeredFrame(NULL), fFrameInfo(NULL)
{
  /**
    * Automatically add the rawdata file to the list
    * and set the file to read.
   **/

  Initialize();
  AddData(filename);
  SetData(0);
}

void GETDecoder::Initialize()
{
  fNumTbs = 512;

  fFrameType = kBasic;
  fFrameSize = 0;

  fDebugMode = kFALSE;
  fIsPositivePolarity = kFALSE;

  fIsDoneAnalyzing = kFALSE;
  fIsDataInfo = kFALSE;
  fDataSize = 0;
  fCurrentDataID = -1;
  fFrameInfoIdx = 0;
  fCoboFrameInfoIdx = 0;
  fTargetFrameInfoIdx = -1;

//  fGETPlot = 0;

  fBuffer = NULL;
  fWriteFile = "";

  if (fFrameInfoArray == NULL) fFrameInfoArray = new TClonesArray("GETFrameInfo", 10000);
  fFrameInfoArray -> Clear("C");

  if (      fHeaderBase == NULL) fHeaderBase = new GETHeaderBase();
  else                           fHeaderBase -> Clear();

  if (fBasicFrameHeader == NULL) fBasicFrameHeader = new GETBasicFrameHeader();
  else                           fBasicFrameHeader -> Clear();

  if (     fLayerHeader == NULL) fLayerHeader = new GETLayerHeader();
  else                           fLayerHeader -> Clear();

  if (   fTopologyFrame == NULL) fTopologyFrame = new GETTopologyFrame();
  else                           fTopologyFrame -> Clear();

  if (      fBasicFrame == NULL) fBasicFrame = new GETBasicFrame();
  else                           fBasicFrame -> Clear();

  if (       fCoboFrame == NULL) fCoboFrame = new GETCoboFrame();
  else                           fCoboFrame -> Clear();

  if (    fLayeredFrame == NULL) fLayeredFrame = new GETLayeredFrame();
  else                           fLayeredFrame -> Clear();
}

void GETDecoder::SetNumTbs(Int_t value) { fNumTbs = value; } 
void GETDecoder::SetDebugMode(Bool_t value) { fDebugMode = value; }

Bool_t GETDecoder::AddData(TString filename)
{
  /**
    * Check if there is a file named `filename`. If exists, add it to the list.
   **/

  TString nextData = GETFileChecker::CheckFile(filename);
  if (!nextData.EqualTo("")) {
    Bool_t isExist = 0;
    for (Int_t iIdx = 0; iIdx < fDataList.size(); iIdx++) {
      if (fDataList.at(0).EqualTo(nextData)) {
        std::cout << "== [GETDecoder] The file already exists in the list!" << std::endl;
        isExist = 1;
      }
    }

    if (!isExist) {
      fDataList.push_back(nextData);

      return kTRUE;
    }
  }

  return kFALSE;
}

Bool_t GETDecoder::SetData(Int_t index)
{
  if (index >= fDataList.size()) {
    std::cout << "== [GETDecoder] End of data list!" << std::endl;

    return kFALSE;
  }

  if (fData.is_open())
    fData.close();

  TString filename = fDataList.at(index);

  fData.open(filename.Data(), std::ios::ate|std::ios::binary);

  if (!(fData.is_open())) {
    std::cout << "== [GETDecoder] Data file open error! Check it exists!" << std::endl;

    return kFALSE;
  } 

  fDataSize = fData.tellg();

  std::cout << "== [GETDecoder] " << filename << " is opened!" << std::endl;

  fData.seekg(0);
  
  if (!fIsDataInfo) {
    fHeaderBase -> Read(fData, kTRUE);

    if (fHeaderBase -> IsBlob()) {
      fTopologyFrame -> Read(fData);
      fHeaderBase -> Read(fData, kTRUE);
    }

    std::cout << "== [GETDecoder] Frame Type: ";
    if (fTopologyFrame -> IsBlob()) {
      fFrameType = kCobo;
      std::cout << "Cobo frame (Max. 4 frames)" << std::endl;
    } else {
      switch (fHeaderBase -> GetFrameType()) {
        case GETFRAMEMERGEDBYID:
          fFrameType = kMergedID;
          std::cout << "Event ID merged frame" << std::endl;
          break;

        case GETFRAMEMERGEDBYTIME:
          fFrameType = kMergedTime;
          std::cout << "Event time merged frame" << std::endl;
          break;

        default:
          fFrameType = kBasic;
          std::cout << "Basic frame" << std::endl;
          break;
      }
    }

    fIsDataInfo = kTRUE;
  }

  fCurrentDataID = index;

  return kTRUE;
}

Bool_t GETDecoder::NextData() { return SetData(fCurrentDataID + 1); }
void GETDecoder::SetPositivePolarity(Bool_t value) { fIsPositivePolarity = value; }

void GETDecoder::ShowList()
{
  std::cout << "== [GETDecoder] Index Data file" << std::endl;
  for (Int_t iItem = 0; iItem < fDataList.size(); iItem++) {
    if (iItem == fCurrentDataID)
      std::cout << " *" << std::setw(6);
    else
      std::cout << std::setw(8);

    std::cout << iItem << "  " << fDataList.at(iItem) << std::endl;
  }
}

Int_t GETDecoder::GetNumData() { return fDataList.size(); }

TString GETDecoder::GetDataName(Int_t index)
{
  if (index >= fDataList.size()) {
    std::cout << "== [GETDecoder] Size of the list is " << fDataList.size() << "!" << std::endl;

    return TString("No data!");
  }

  return fDataList.at(index);
}

Int_t GETDecoder::GetNumTbs() { return fNumTbs; }
//GETPlot *GETDecoder::GetGETPlot() { if (!fGETPlot) fGETPlot = new GETPlot(this); return fGETPlot; }
GETDecoder::EFrameType GETDecoder::GetFrameType() { return fFrameType; }

/*
GETBasicFrame *GETDecoder::GetFrame(Int_t frameID)
{
  if (frameID == -1)
    fCurrentFrameID++;
  else {
    if (frameID > fCurrentFrameID)
      SkipFrames(frameID);

    fCurrentFrameID = frameID;
  }

  fFrameInfo = (GETFrameInfo *) fFrameInfoArray -> ConstructedAt(fCurrentFrameID);
  if (fFrameInfo -> IsFill()) {
    if (fFrameInfo -> GetDataID() != fCurrentDataID)
      SetData(fFrameInfo -> GetDataID());

    fData.seekg(fFrameInfo -> GetStartByte());
    fBasicFrame -> Read(fData);

    return fBasicFrame;
  } else {
    if (fData.tellg() == fDataSize) {
      if (SetNextData() && fIsAutoReload)
        return GetFrame(frameID);
      else {
        fFrameInfoArray -> RemoveAt(fCurrentFrameID);
        return NULL;
      }
    }

    fBasicFrame -> Read(fData);

    fFrameInfo -> SetEventID(fBasicFrame -> GetEventID());
    fFrameInfo -> SetStartByte((ULong64_t) fData.tellg() - fBasicFrame -> GetFrameSize());
    fFrameInfo -> SetEndByte(fData.tellg());
    fFrameInfo -> SetDataID(fCurrentDataID);

    return fBasicFrame;
  }
}
*/


GETBasicFrame *GETDecoder::GetBasicFrame(Int_t frameID)
{
  fData.clear();

  if (frameID == -1)
    fTargetFrameInfoIdx++;
  else
    fTargetFrameInfoIdx = frameID;

  if (fIsDoneAnalyzing)
    if (fTargetFrameInfoIdx > fFrameInfoArray -> GetLast())
      return NULL;

  if (fFrameInfoIdx > fTargetFrameInfoIdx)
    fFrameInfoIdx = fTargetFrameInfoIdx;

  fFrameInfo = (GETFrameInfo *) fFrameInfoArray -> ConstructedAt(fFrameInfoIdx);
  while (fFrameInfo -> IsFill()) {

#ifdef DEBUG
    cout << "fFrameInfoIdx: " << fFrameInfoIdx << " fTargetFrameInfoIdx: " << fTargetFrameInfoIdx << endl;
#endif

    if (fFrameInfoIdx == fTargetFrameInfoIdx) {
      if (fFrameInfo -> GetDataID() != fCurrentDataID)
        SetData(fFrameInfo -> GetDataID());

      fData.seekg(fFrameInfo -> GetStartByte());
      fBasicFrame -> Read(fData);

#ifdef DEBUG
    cout << "Returned event ID: " << fBasicFrame -> GetEventID() << endl;
#endif

      return fBasicFrame;
    } else
      fFrameInfo = (GETFrameInfo *) fFrameInfoArray -> ConstructedAt(++fFrameInfoIdx);
  }

  fBasicFrameHeader -> Read(fData);
  fData.ignore(fBasicFrameHeader -> GetFrameSkip());

  fFrameInfo -> SetDataID(fCurrentDataID);
  fFrameInfo -> SetStartByte((ULong64_t) fData.tellg() - fBasicFrameHeader -> GetFrameSize());
  fFrameInfo -> SetEndByte(fData.tellg());
  fFrameInfo -> SetEventID(fBasicFrameHeader -> GetEventID());

  if (fFrameInfo -> GetEndByte() == fDataSize)
    if (!NextData())
      fIsDoneAnalyzing = kTRUE;

  return GetBasicFrame(fTargetFrameInfoIdx);
}

/*
GETCoboFrame *GETDecoder::GetCoboFrame(Int_t frameID)
{
}
*/

GETLayeredFrame *GETDecoder::GetLayeredFrame(Int_t frameID)
{
  fData.clear();

  if (frameID == -1)
    fTargetFrameInfoIdx++;
  else
    fTargetFrameInfoIdx = frameID;

  if (fIsDoneAnalyzing)
    if (fTargetFrameInfoIdx > fFrameInfoArray -> GetLast())
      return NULL;

  if (fFrameInfoIdx > fTargetFrameInfoIdx)
    fFrameInfoIdx = fTargetFrameInfoIdx;

  fFrameInfo = (GETFrameInfo *) fFrameInfoArray -> ConstructedAt(fFrameInfoIdx);
  while (fFrameInfo -> IsFill()) {

#ifdef DEBUG
    cout << "fFrameInfoIdx: " << fFrameInfoIdx << " fTargetFrameInfoIdx: " << fTargetFrameInfoIdx << endl;
#endif

    if (fFrameInfoIdx == fTargetFrameInfoIdx) {
      if (fFrameInfo -> GetDataID() != fCurrentDataID)
        SetData(fFrameInfo -> GetDataID());

      fData.seekg(fFrameInfo -> GetStartByte());
      fLayeredFrame -> Read(fData);

#ifdef DEBUG
    cout << "Returned event ID: " << fLayeredFrame -> GetEventID() << endl;
#endif

      return fLayeredFrame;
    } else
      fFrameInfo = (GETFrameInfo *) fFrameInfoArray -> ConstructedAt(++fFrameInfoIdx);
  }

  fLayerHeader -> Read(fData);
  fData.ignore(fLayerHeader -> GetFrameSkip());

  fFrameInfo -> SetDataID(fCurrentDataID);
  fFrameInfo -> SetStartByte((ULong64_t) fData.tellg() - fLayerHeader -> GetFrameSize());
  fFrameInfo -> SetEndByte(fData.tellg());
  switch (fFrameType) {
    case kMergedID:
      fFrameInfo -> SetEventID(fLayerHeader -> GetEventID());
      break;

    case kMergedTime:
      fFrameInfo -> SetEventTime(fLayerHeader -> GetEventTime());
      fFrameInfo -> SetDeltaT(fLayerHeader -> GetDeltaT());
      break;
  }

  if (fFrameInfo -> GetEndByte() == fDataSize)
    if (!NextData())
      fIsDoneAnalyzing = kTRUE;

  return GetLayeredFrame(fTargetFrameInfoIdx);
}

void GETDecoder::SkipFrames(Int_t frameID) {
  Int_t entriesInArray  = fFrameInfoArray -> GetEntriesFast();

  if (frameID <= entriesInArray) {
#ifdef DEBUG
  LOG(DEBUG) << "There're " << entriesInArray << " entries in fFrameInfoArray. Exit " << __FUNCTION__ << endl;
#endif

    return;
  }

  fData.seekg(((GETFrameInfo *) fFrameInfoArray -> Last()) -> GetEndByte());

  switch (fFrameType) {
    case kBasic:
      while (1) {
        fBasicFrameHeader -> Read(fData);

        fFrameInfo = (GETFrameInfo *) fFrameInfoArray -> ConstructedAt(entriesInArray++);
        fFrameInfo -> SetEventID(fBasicFrameHeader -> GetEventID());
        fFrameInfo -> SetStartByte((ULong64_t) fData.tellg() - fBasicFrameHeader -> GetHeaderSize());
        fFrameInfo -> SetEndByte(fFrameInfo -> GetStartByte() + fBasicFrameHeader -> GetFrameSize());
        fFrameInfo -> SetDataID(fCurrentDataID);

        fData.ignore(fBasicFrameHeader -> GetFrameSkip());

#ifdef DEBUG
  LOG(DEBUG) << "Skipping frames. entriesInArray: " << entriesInArray << endl;
#endif

        if (entriesInArray == frameID)
          break;

//        if (CheckEOF()) {
//          if (fIsAutoReload) { if (!SetNextData()) break; }
//          else                 break;
//        }
      }
      break;

    case kMergedTime:
      while (1) {
        fLayerHeader -> Read(fData);

        fFrameInfo = (GETFrameInfo *) fFrameInfoArray -> ConstructedAt(entriesInArray++);
        fFrameInfo -> SetEventTime(fLayerHeader -> GetEventTime());
        fFrameInfo -> SetDeltaT(fLayerHeader -> GetDeltaT());
        fFrameInfo -> SetStartByte((ULong64_t) fData.tellg() - fLayerHeader -> GetHeaderSize());
        fFrameInfo -> SetEndByte(fFrameInfo -> GetStartByte() + fLayerHeader -> GetFrameSize());
        fFrameInfo -> SetDataID(fCurrentDataID);

        fData.ignore(fLayerHeader -> GetFrameSkip());

#ifdef DEBUG
  LOG(DEBUG) << "Skipping frames. entriesInArray: " << entriesInArray << endl;
#endif

        if (entriesInArray == frameID)
          break;

//        if (CheckEOF()) {
//          if (fIsAutoReload) { if (!SetNextData()) break; }
//          else                 break;
//        }
      }
      break; 

    case kMergedID:
      while (1) {
        fLayerHeader -> Read(fData);

        fFrameInfo = (GETFrameInfo *) fFrameInfoArray -> ConstructedAt(entriesInArray++);
        fFrameInfo -> SetEventID(fLayerHeader -> GetEventID());
        fFrameInfo -> SetStartByte((ULong64_t) fData.tellg() - fLayerHeader -> GetHeaderSize());
        fFrameInfo -> SetEndByte(fFrameInfo -> GetStartByte() + fLayerHeader -> GetFrameSize());
        fFrameInfo -> SetDataID(fCurrentDataID);

        fData.ignore(fLayerHeader -> GetFrameSkip());

#ifdef DEBUG
  LOG(DEBUG) << "Skipping frames. entriesInArray: " << entriesInArray << endl;
#endif

        if (entriesInArray == frameID)
          break;

//        if (CheckEOF()) {
//          if (fIsAutoReload) { if (!SetNextData()) break; }
//          else                 break;
//        }
      }
      break; 
  }
}

Bool_t GETDecoder::CheckEOF() { return (fDataSize == fFrameInfo -> GetEndByte()); }

void GETDecoder::PrintFrameInfo(Int_t frameID) {
  if (frameID == -1) {
    for (Int_t iEntry = 0; iEntry < fFrameInfoArray -> GetEntriesFast(); iEntry++)
      ((GETFrameInfo *) fFrameInfoArray -> At(iEntry)) -> Print();
  } else
    ((GETFrameInfo *) fFrameInfoArray -> At(frameID)) -> Print();
}

/*
Bool_t GETDecoder::SetWriteFile(TString filename, Bool_t overwrite)
{
  fWriteFile = GETFileChecker::CheckFile(filename);
  if (!fWriteFile.IsNull() && !overwrite) {
    std::cout << "== [GETDecoder] The file you specified already exists!" << std::endl;
    std::cout << "                If you want to overwrite it, give kTRUE as a second argument." << std::endl;

    fWriteFile = "";

    return kFALSE;
  }

  fWriteFile = filename;
  std::ofstream dummy(fWriteFile.Data(), std::ios::trunc);
  dummy.close();

  if (fBuffer == NULL)
    fBuffer = new Char_t[14000000];

  return kTRUE;
}

void GETDecoder::WriteFrame()
{
  if (fWriteFile.IsNull()) {
    std::cout << "== [GETDecoder] Write file is not set. Use SetWriteFile() first!" << std::endl;

    return;
  }

  if (fFrame == NULL) {
    std::cout << "== [GETDecoder] No frame loaded! Read a frame before write." << std::endl;

    return;
  }

  std::ofstream outFile(fWriteFile.Data(), std::ios::ate|std::ios::binary|std::ios::app);
  if (GetFrameType() == GETDecoder::kNormal) {
    fData.seekg((ULong64_t) fData.tellg() - fFrameSize);
    fData.read(fBuffer, fFrameSize);
    outFile.write(fBuffer, fFrameSize);
  } else {
    ULong64_t currentPosition = fData.tellg();
    fData.seekg(fMergedFrameStartPoint);
    fData.read(fBuffer, fFrameSize);
    outFile.write(fBuffer, fFrameSize);
    fData.seekg(currentPosition);
  }
  outFile.close();
}
*/
