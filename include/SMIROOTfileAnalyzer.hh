#ifndef __SMIROOTFILEANALYZER_HH__
#define __SMIROOTFILEANALYZER_HH__

#ifdef __HAVEROOT
#include"SMIWaveformAnalyzer.hh"
#include"chaintools.hh"

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
  inline bool addFile(std::string name) {
    if(chain != NULL){ chain->Add(name.c_str()); return true;}
    return false;
  }

  template<typename Func>
  bool loop(size_t start, size_t num, Func func){
    size_t entries = chain->GetEntries();
    size_t stop = start+num;
    if(start > entries) return false;
    if(stop > entries) stop = entries;

    for(size_t i=start; i<stop; i++){
      chain->GetEntry(i);
      func(*this);
    }
    return true;
  }

private:
  std::map<std::string, std::string> rootmapping;

  // managed by the ROOT garbage collector
  std::vector<TBranch **> branches;
  TChain *chain;
};

#endif

#endif
