#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <vector>
#if defined (__MAKECINT__) 
#pragma link C++ class vector<Long64_t>+; 
#endif

#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TLegend.h>
class HouseKeepingData{
public:
  Long64_t  unixtime , dunixtime;
  Long64_t  dtimeInt0, dtimeInt1;
  int boardID;
  int TempCen, TempFPG;
  int Temps1, Temps2, Temps3, Temps4;
  int Humidity, Pressure, Altitude;
  int degX, degY, degZ;
  int accX, accY, accZ;
  int magX, magY, magZ;;
  
  HouseKeepingData(){
    unixtime = -1; dunixtime = -1;
    dtimeInt0 = -1; dtimeInt1 = -1;
    boardID = -1;
    TempCen = -1; TempFPG = -1;
    Temps1 = -1; Temps2 = -1; Temps3 = -1; Temps4 = -1;
    Humidity = -1; Pressure = -1; Altitude = -1;
    degX = -1; degY = -1; degZ = -1;
    accX = -1; accY = -1; accZ = -1;
    magX = -1; magY = -1; magZ = -1;
  };
  
};

string ConvrtHK(
  const char* DirOperate,
  const char* HksDirName,
  const Long64_t UTs,
  const Long64_t UTf,
  const Long64_t AverageTimeGap
) {
  // Create the tmpPath for the HK data
  system(Form("mkdir -p %s",DirOperate));
  // Create the TFile for save histograms
  TFile *outfile = new TFile(Form("%sHK.root",DirOperate), "recreate");
  
  // Find out the HK datas and output to be table in load in program
  char HKFileList[200];
  sprintf(HKFileList, "%sHKTxtNameAna.dat",DirOperate);
  system(Form("rm %s",HKFileList));
  system(Form("ls %s >> %s",HksDirName,HKFileList));
  ifstream infilename(HKFileList);
  vector<TString> filename;
  int Numfiles = 0;
  for(char fileNameTmp[200];infilename>>fileNameTmp;) filename.push_back(fileNameTmp);
  Numfiles = int (filename.size());
  infilename.close();
  
  // Set the time range of the histogram
  Long64_t UT0 = 86400*(UTs/86400),UTN = 86400*(UTf/86400+1);
  int nBins = (UTN-UT0)/AverageTimeGap;
  TH2F *h2Hu = new TH2F("h2Hu","h2Hu",nBins,UT0,UTf,1000,0,100);
  TH2F *h2Th = new TH2F("h2Th","h2Th",nBins,UT0,UTf,1000,0,100);
  
  cout<<"Start to convert HK raw data to be TH1F format"<<endl;
  // load the Raw-HK files by the UT range
  for(int i=0;i<Numfiles; i++){
    ifstream in(filename[i].Data());
    cout<<filename[i]<<endl;
    HouseKeepingData HKD;
    Long64_t UtLast = 0;
    
    while(in.good()){
      // Load on line
      string Tmp_str,stmp;
      bool skipThisFile = false;
      getline(in, Tmp_str);
      std::istringstream iss(Tmp_str);
      
      // Load the line by the format
      int cnts[4]={0};
      iss>>stmp>>HKD.boardID>>stmp;
      iss>>cnts[0]>>cnts[1]>>cnts[2]>>cnts[3];
      iss>>HKD.TempCen>>HKD.Humidity;
      iss>>HKD.Temps1>>HKD.Temps2>>HKD.Temps3>>HKD.Temps4;
      iss>>HKD.Pressure>>HKD.Altitude>>HKD.TempFPG;
      iss>>HKD.degX>>HKD.degY>>HKD.degZ;
      iss>>HKD.accX>>HKD.accY>>HKD.accZ;
      iss>>HKD.magX>>HKD.magY>>HKD.magZ;
      iss>>stmp>>stmp>>HKD.unixtime;
      HKD.Humidity = HKD.Humidity<0 ? HKD.Humidity+127:HKD.Humidity;
      // h1UT->Fill(HKD.unixtime);
      
      // Skip the unreliable data
      if(HKD.unixtime<UT0) continue;
      if(HKD.Humidity<=0) continue;
      if(HKD.TempCen<=0) continue;
      
      //Fill the H/T data into the TH2F for calculate the time average
      h2Hu->Fill(HKD.unixtime,HKD.Humidity);
      h2Th->Fill(HKD.unixtime,(HKD.TempCen+HKD.Temps1+HKD.Temps2+HKD.Temps3+HKD.Temps4)/5.);
      UtLast = HKD.unixtime;
    }
    if(UtLast>UTf) break;
    in.close();
  }
  
  // Calculate the time average 
  TH1F *h1HuAvg = new TH1F("h1HuAvg","h1Hu",nBins,UT0,UTf);
  TH1F *h1ThAvg = new TH1F("h1ThAvg","h1Th",nBins,UT0,UTf);
  for(int i = 1 ; i <= nBins ; i++){
    h2Hu->GetXaxis()->SetRange(i,i+1);
    h2Th->GetXaxis()->SetRange(i,i+1);
    h1HuAvg->SetBinContent(i,h2Hu->GetMean(2));
    h1ThAvg->SetBinContent(i,h2Th->GetMean(2));
    h1HuAvg->SetBinError(i,h2Hu->GetRMS(2));
    h1ThAvg->SetBinError(i,h2Th->GetRMS(2));
  }
  
  // Output
  outfile->Write();
  outfile->Close();
  cout<<"Finish convert and save as: "<<outfile->GetName()<<endl;
  return outfile->GetName();
}