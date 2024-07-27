#include "ConvrtHK.h"
#include "ExportTreeToTxt.h"
using namespace std;
const string Default_GapFile = "/mnt/d/Desktop/SFTPTMp/ConvertTGapToTxt/GapT140_S20231024E20240512_nHTH1L64CRun38.root";
// Default path name of the Gap T .root file
const string Default_HksDirN = "/mnt/d/Desktop/SFTPTMp/ConvertTGapToTxt/HKs/*_HK.txt";
// Default target HK path + name for search and compare to the event data
const string Default_DirOperate = "./TmpOpr/";
//  Default operation file save path, if you want to run in mutiThread, please set to be difference
const string Default_OutputFilePath = "./";
// Default txt file path you want to ouput.
const string Default_OutputFileName = Default_OutputFilePath+"output.txt";
// Default txt file name you want to ouput.
const Long64_t Default_AverageTimeGap = 300;
// 500 sec for the average calculation of the HK data

void ConverCode(
// Load the default information to convert or by the input imformation.
  string GapFile = Default_GapFile,
  string HksDirN = Default_HksDirN,
  string DirOperate = Default_DirOperate,
  string OutputFilePath = Default_OutputFilePath,
  string OutputFileName = Default_OutputFileName,
  Long64_t AverageTimeGap = Default_AverageTimeGap
){
  // Create the directory of the output file path
  system(("mkdir -p "+OutputFilePath).data());
  
  // Check the UT time rage of the TGap file
  TFile *fGap = new TFile(GapFile.data(),"open");
  TTree *tGap = (TTree*) fGap->Get("t");
  Long64_t unixtime, Uts, Utf;
  tGap->SetBranchAddress("unixtime", &unixtime);
  tGap->GetEntry(0); Uts = unixtime;
  tGap->GetEntry(tGap->GetEntries()-1); Utf = unixtime;
  cout<<Uts<<"\t"<<Utf<<endl;
  
  // Convert the raw data of HK to be TH1F format;
  string HKFileName = ConvrtHK(DirOperate.data(),HksDirN.data(),Uts,Utf,AverageTimeGap);
  // Convert the TGap.root data to be Txt file
  ExportTreeToTxt(GapFile, HKFileName, OutputFileName);
}
