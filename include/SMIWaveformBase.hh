#ifndef __SMIWAVEFORMBASE_HH__     
#define __SMIWAVEFORMBASE_HH__     

#include<vector>
#include<string>
#include<map>
#include<cstdio>
#include<ostream>

#include"SMIWave.hh"

class SMIWaveformBase {
public: 
  SMIWaveformBase() : signalPattern(0), eventnum(0), timestamp(0), 
		      nEntries(1024) {}

  bool renameChannel    (std::string newName, std::string oldName);
  bool renameTrigger    (std::string newName, std::string oldName);
  bool addChannelTrigger(std::string channel, std::string trigger);

  friend std::ostream& operator<< (std::ostream &out, SMIWaveformBase &Wave);

  std::map<std::string, WaveForm>                  channel; 
  std::map<std::string, WaveForm>                  trigger; 
  std::map<std::string, std::string>               channeltrigger;
  std::map<std::string, std::vector<std::string> > triggerchannel;

protected:
  bool addChannel(std::string Name);
  bool addTrigger(std::string Name);

  bool testChannel(std::string Name);
  bool testTrigger(std::string Name);



  unsigned int signalPattern;
  int          eventnum;
  unsigned int timestamp;

  size_t       nEntries;
};

#endif
