#ifndef __SMIWAVEFORM_HH__
#define __SMIWAVEFORM_HH__

#include<map>
#include<string>

#include"SMIWaveformBase.hh"

class SMIWaveform : public SMIWaveformBase {
public:
  SMIWaveform();
  SMIWaveform(size_t _nEntries);
  
  bool fillChannel(std::string name, double *data);
  bool fillTrigger(std::string name, double *data);

  inline void setADCBits (size_t val) {adcBits  = val;}
  inline void setADCRange(size_t val) {adcRange = val;}

protected:
  inline double getRand(double low, double high) const;

  size_t adcBits;
  double adcRange;
  double adcDiff;

private:
  void initRand();
};

#endif
