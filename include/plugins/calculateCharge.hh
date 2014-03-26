#ifndef __CALCULATECHARGE_HH__
#define __CALCULATECHARGE_HH__

#include"SMIWaveformAnalyzerPluginSystem.hh"

namespace plugin {
  class calculateCharge : public SMIAnalyzerPluginBase {
  public:
    calculateCharge() : lowGate(10), highGate(70) {}
    calculateCharge(double low, double high) : lowGate(low), highGate(high) {}

    virtual bool operator()(WaveForm &wave);
  protected:
    double lowGate;
    double highGate;
  };

}

#endif
