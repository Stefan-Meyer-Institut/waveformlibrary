#ifndef __SMIWAVEFORM_HH__
#define __SMIWAVEFORM_HH__

#include<map>
#include<string>

#include"SMIWaveformBase.hh"

class SMIWaveform : public SMIWaveformBase {
public:
  SMIWaveform();

  inline double getRand(double low, double high) const;

//protected:
};

#endif
