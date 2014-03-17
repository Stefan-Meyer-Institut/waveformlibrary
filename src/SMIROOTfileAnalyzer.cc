#ifdef __HAVEROOT

/*! 
  \file SMIROOTfileAnalyzer.cc
  \brief analyze TTrees in CERN ROOT files
  \author Clemens Sauerzopf
*/ 

#include"SMIROOTfileAnalyzer.hh"
#include<iostream>
#include<cstdlib>

#include"chaintools.hh"


SMIROOTfileAnalyzer::SMIROOTfileAnalyzer() : chain(NULL){

}

SMIROOTfileAnalyzer::SMIROOTfileAnalyzer(std::string configfile) : chain(NULL){
  if(!readConfigFile(configfile)){
    std::cerr << "Couldn't open channel config file: " << configfile 
	      << std::endl;
    std::exit(-1);
  }
}

bool SMIROOTfileAnalyzer::readConfigFile(std::string configfile){

  std::ifstream ifile(configfile.c_str());
  if(!ifile) return false;
  ifile.seekg (0, ifile.end);
  int length = ifile.tellg();
  ifile.seekg (0, ifile.beg);
  if(length == 0){
    std::cerr << "File is empty!" << std::endl;
    return false;
  }

  char line[256];
  std::string command, channel, trigger, rootname;
  bool chaincreated = false;
  do {
    if(ifile.peek()=='#') ifile.getline(line,255);
    else {
      ifile >> command;
      if(command == "C"){
	ifile >> rootname >> trigger >> channel;
	rootmapping[channel] = rootname;
	addChannelTrigger(channel,trigger);
      } else if(command == "T"){
	ifile >> rootname >> trigger;
	rootmapping[trigger] = rootname;
      } else if(command == "TreeName:" && !chaincreated) {
	ifile >> channel;
	chain = new TChain(channel.c_str());
	chaincreated = true;
      }	
    }
  } while(!ifile.eof());

  ifile.close();
  if(!chaincreated) std::cerr << "WARNING no TChain was created!"
			      << std::endl;
  return true;
}

bool SMIROOTfileAnalyzer::associateTTreeBranches(){
  if(chain == NULL){ return false;}
  std::map<std::string, std::string>::iterator i;
  branches = std::vector<TBranch **>(rootmapping.size(), NULL);
  size_t num=0;
  for(i=rootmapping.begin();i!=rootmapping.end(); i++){
    WaveForm &wave = getWaveForm(i->second);
    chain->SetBranchAddress(i->first.c_str(),&(wave.V[0]),branches[num++]);
  }  
  return true;
}

#endif
