#ifndef __SMIWAVEFORMBASE_HH__     
#define __SMIWAVEFORMBASE_HH__     

#include<vector>
#include<string>
#include<map>
#include<cstdio>
#include<ostream>

class WaveInfo {
public:
  WaveInfo() : Q(0), A(0), T(0), ToT(0), isDigital(false) {}
  WaveInfo(double _Q, double _A, double _T, double _ToT, bool _isDigital) 
    : Q(_Q), A(_A), T(_T), ToT(_ToT), isDigital(_isDigital) {}

  friend std::ostream& operator<< (std::ostream &out, WaveInfo &Info);

  double              Q;         // charge  [coulomb]
  double              A;         // amplitude [Volt]
  double              T;         // starting time [ns]
  double              ToT;       // time over threshold [ns]
  bool                isDigital; // is a digital pulse
};

class WaveForm : public WaveInfo {
public:
  WaveForm() : V(1024,0.0), t(1024,0.0) {
    for(int i=0; i<1024; i++) t[i] = i*0.2; // [ns] 1 bin = 200 ps
  }

  inline WaveInfo getInfo() const {return WaveInfo(Q,A,T,ToT,isDigital);}
  std::vector<double> V;         // voltage [Volt]
  std::vector<double> t;         // time    [ns]
};

class SMIWaveformBase {
public: 
  SMIWaveformBase() : signalPattern(0), eventnum(0), timestamp(0) {}

  bool renameChannel    (std::string newName, std::string oldName);
  bool renameTrigger    (std::string newName, std::string oldName);
  bool addChannelTrigger(std::string channel, std::string trigger);
  friend std::ostream& operator<< (std::ostream &out, SMIWaveformBase &Wave);

protected:
  bool addChannel(std::string Name);
  bool addTrigger(std::string Name);

  std::map<std::string, WaveForm>                  channel; 
  std::map<std::string, WaveForm>                  trigger; 
  std::map<std::string, std::string>               channeltrigger;
  std::map<std::string, std::vector<std::string> > triggerchannel;

  unsigned int signalPattern;
  int          eventnum;
  unsigned int timestamp;
};

#endif
