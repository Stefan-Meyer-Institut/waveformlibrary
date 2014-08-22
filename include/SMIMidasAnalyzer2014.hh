#ifndef __SMIMIDASANALYZER2014_H__
#define __SMIMIDASANALYZER2014_H__

#ifdef __HAVEMIDAS

#include"SMIWaveformAnalyzer.hh"
// stdlib includes
#include<string>
#include<fstream>
#include<vector>
#include<map>

#include"SMIMidasAnalyzerDataTypes.hh"

class TMidasEvent;

class SMIMidasAnalyzer2014 : public SMIWaveformAnalyzer{
public:
  SMIMidasAnalyzer2014() : tablesLoaded(false) {}
  SMIMidasAnalyzer2014(std::string configfile) :  tablesLoaded(false)
  {readConfigFile(configfile);}

  inline virtual bool loop(processBase &func){
    return loop(0,1,func); }
    
  inline virtual bool loop(size_t start, size_t num, processBase &func){
    func(*this);
    return true;}

  bool readConfigFile(std::string configfile);
  bool prepareMidasEvent(TMidasEvent &event);
  bool prepareCorrectionTables(TMidasEvent &event);

  //bool loadEvent(size_t num);

private:
  void decodeADCData(DWORD *data, unsigned int ADCnum);
  void prepareADCBuffers(unsigned int adcNum, std::string nameScheme, int pulse);

  V1742::V1742_DataCorrection_t correctionTable[16][4];
  std::map<std::string,std::string> chnames;
  std::vector<std::string> adcBankList;

  bool tablesLoaded;
};

#endif

#endif
