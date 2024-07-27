#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

void ExportTreeToTxt(
  const string inputFilename, 
  const string HKFilename,
  const string outputFilename
) {
    // 打開ROOT文件
    TFile* inputFile = TFile::Open(inputFilename.data(), "READ");
    if (!inputFile || inputFile->IsZombie()) {
        std::cerr << "Error: cannot open input file " << inputFilename << std::endl;
        return;
    }
    
    // Open the histogram of average HK data
    TFile* HKFile = TFile::Open(HKFilename.data(), "READ");
    TH1F *h1HuAvg = (TH1F*)HKFile->Get("h1HuAvg");
    TH1F *h1ThAvg = (TH1F*)HKFile->Get("h1ThAvg");
    // 獲取TChain
    TChain* tree = new TChain("t");
    tree->Add(inputFilename.data());
    
    // 設定變量來存儲樹的分支
    Long64_t unixtime;
    Int_t nH, nH0, nH1, nH2, nH3;
    std::vector<int> *channel = nullptr, *board = nullptr, *pwidth = nullptr, *pcnt = nullptr;
    std::vector<Long64_t> *tcnt = nullptr;

    // 設定分支地址
    tree->SetBranchAddress("unixtime", &unixtime);
    tree->SetBranchAddress("nH", &nH);
    tree->SetBranchAddress("nH0", &nH0);
    tree->SetBranchAddress("nH1", &nH1);
    tree->SetBranchAddress("nH2", &nH2);
    tree->SetBranchAddress("nH3", &nH3);
    tree->SetBranchAddress("channel", &channel);
    tree->SetBranchAddress("board", &board);
    tree->SetBranchAddress("pwidth", &pwidth);
    tree->SetBranchAddress("pcnt", &pcnt);
    tree->SetBranchAddress("tcnt", &tcnt);
    tree->SetBranchStatus("dtimeInt", 0);
    // Debug for the branch problem

    // 打開輸出文件
    std::ofstream outputFile(outputFilename.data());
    if (!outputFile.is_open()) {
        std::cerr << "Error: cannot open output file " << outputFilename << std::endl;
        inputFile->Close();
        return;
    }
    std::cout << "Start to convert the TGap.root file!" << std::endl;
    // 寫入數據到輸出文件
    Long64_t nEntries = tree->GetEntries();
    for (Long64_t i = 0; i < nEntries; ++i) {
        tree->GetEntry(i);
        if(unixtime%86400==0) cout<<"\r"<<i*100./(nEntries*1.)<<"%"<<flush;
        // output the unixtime and search the humidity and temperature by unixtime
        outputFile << unixtime << '\t' 
                << h1ThAvg->GetBinContent(h1ThAvg->FindBin(unixtime))<< '\t' 
                << h1HuAvg->GetBinContent(h1HuAvg->FindBin(unixtime))<< '\t' ;
        outputFile << nH << '\t' 
                   << nH0 << '\t' 
                   << nH1 << '\t' 
                   << nH2 << '\t' 
                   << nH3;
        for (int j = 0; j < nH; ++j) {
            outputFile << '\t' << (*board)[j];
        }
        for (int j = 0; j < nH; ++j) {
            outputFile << '\t' << (*channel)[j];
        }
        for (int j = 0; j < nH; ++j) {
            outputFile << '\t' << (*pcnt)[j];
        }
        for (int j = 0; j < nH; ++j) {
            outputFile << '\t' << (*tcnt)[j];
        }
        for (int j = 0; j < nH; ++j) {
            outputFile << '\t' << (*pwidth)[j];
        }
        outputFile << '\n';
    }

    // 關閉文件
    outputFile.close();
    inputFile->Close();

    std::cout << "\nExport completed successfully!" << std::endl;
    std::cout << "Save as: " << outputFilename.data() << std::endl;
    
}
