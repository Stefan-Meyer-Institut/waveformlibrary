#ifndef __SMIROOTFILEANALYZER_HH__
#define __SMIROOTFILEANALYZER_HH__

#ifdef __HAVEROOT
#include"SMIWaveformAnalyzer.hh"

// ROOT includes
#include<TChain.h>

// stdlib includes
#include<string>
#include<fstream>
#include<map>

class SMIROOTfileAnalyzer : public SMIWaveformAnalyzer{
public:
  SMIROOTfileAnalyzer();
  SMIROOTfileAnalyzer(std::string configfile);
  bool readConfigFile(std::string configfile);
  bool associateTTreeBranches();
private:
  std::map<std::string, std::string> rootmapping;
};

#endif

#endif
