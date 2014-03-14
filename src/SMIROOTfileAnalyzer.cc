#ifdef __HAVEROOT

#include"SMIROOTfileAnalyzer.hh"
#include<iostream>
#include<cstdlib>
SMIROOTfileAnalyzer::SMIROOTfileAnalyzer(){

}

SMIROOTfileAnalyzer::SMIROOTfileAnalyzer(std::string configfile){
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
  std::string channel, trigger, rootname;
  do {
    if(ifile.peek()=='#') ifile.getline(line,255);
    else {
      ifile >> rootname >> trigger >> channel;
      rootmapping[channel] = rootname;
      addChannelTrigger(channel,trigger);
    }
  } while(!ifile.eof());

  ifile.close();
  return true;
}

#endif
