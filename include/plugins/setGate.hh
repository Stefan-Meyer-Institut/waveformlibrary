#ifndef __SETGATE_HH__
#define __SETGATE_HH__

#include"SMIWaveformAnalyzerPluginSystem.hh"

namespace plugin {

  class setGate : public SMIAnalyzerPluginBase {
  public:
    setGate();
    virtual bool operator()(WaveForm &wave);
  private:
  };

}

#endif
