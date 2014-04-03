#ifndef __CONSTANTFRACTIONBASE_HH__
#define __CONSTANTFRACTIONBASE_HH__

#include"SMIWaveformAnalyzerPluginSystem.hh"

namespace plugin {
  class constantFractionBase : public SMIAnalyzerPluginBase {
  public:
    constantFractionBase() : delay(-3) {}
    virtual bool operator()(WaveForm &wave);

  protected:
    virtual double getTime(WaveForm &sumwave) = 0;
    double delay;
  };
}

#endif
