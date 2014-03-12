#ifndef __SMIWAVEFORM_HH__
#define __SMIWAVEFORM_HH__

#include<map>
#include<string>

#if __cplusplus >= 201103L
#define CPP11
#include <random>
#else

#endif


#include"SMIWaveformBase.hh"

class SMIWaveform : public SMIWaveformBase {
public:
  SMIWaveform();
  SMIWaveform(size_t _nEntries);
  
  bool fillChannel(std::string name, double *data);
  bool fillTrigger(std::string name, double *data);

  void changeTime(double time);
  inline bool shiftChannelTime(std::string name, double time);
  inline bool shiftTriggerTime(std::string name, double time);

  inline void setADCBits (size_t val) {adcBits  = val; adcDiff = adcRange/(1<<adcBits);}
  inline void setADCRange(size_t val) {adcRange = val; adcDiff = adcRange/(1<<adcBits);}

protected:
  inline double getRand(double low, double high);

  size_t adcBits;
  double adcRange;
  double adcDiff;

private:
  void initRand();
#ifdef CPP11
  std::random_device rd;
  std::mt19937 gen;
  std::uniform_real_distribution<double> dist;
#endif
};

#endif
