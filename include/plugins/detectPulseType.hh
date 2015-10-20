#ifndef __DETECTPULSETYPE_HH__
#define __DETECTPULSETYPE_HH__

#include"SMIWaveformAnalyzerPluginSystem.hh"


namespace plugin {
  class detectPulseType : public SMIAnalyzerPluginBase {
  public:
    detectPulseType(){}
    ~detectPulseType(){}
    virtual bool operator()(WaveForm &wave);
  };
}

#endif
