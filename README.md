# TGap-HKFileToTxtFileConvertor
For the convert the TGap*.root and the HK-raw.txt to be the txt file

The main controller is ConverCode.C

Load the default information to or by the input value to be the setting for convert the data.

    void ConverCode(
      string GapFile = Default_GapFile,
      string HksDirN = Default_HksDirN,
      string DirOperate = Default_DirOperate,
      string OutputFilePath = Default_OutputFilePath,
      string OutputFileName = Default_OutputFileName,
      string AverageTimeGap = Default_AverageTimeGap
    )

PS: If you want to use the scripe to do the muti-Tread conversion, you need to make the "DirOperate" and "OutputFileName" to be differnet.

Some detail of the setting value and the default for thew expample:

Default path name of the Gap T .root file

    const string Default_GapFile = "/mnt/d/Desktop/SFTPTMp/ConvertTGapToTxt/GapT140_S20231024E20240512_nHTH1L64CRun38.root";

Default target HK path + name for search and compare to the event data

    const string Default_HksDirN = "/mnt/d/Desktop/SFTPTMp/ConvertTGapToTxt/HKs/*_HK.txt";

Default operation file save path, if you want to run in mutiThread, please set to be difference
    
    const string Default_DirOperate = "./TmpOpr/";

Default txt file path you want to ouput.

    const string Default_OutputFilePath = "./";

Default txt file name you want to ouput.

    const string Default_OutputFileName = Default_OutputFilePath+"output.txt";

500 sec for the average calculation of the HK data

    const Long64_t Default_AverageTimeGap = 300;

