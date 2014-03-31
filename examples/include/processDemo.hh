#ifndef __PROCESSDEMO_H__
#define __PROCESSDEMO_H__

#include"SMIWaveformAnalyzerProcessSystem.hh"

class processDemo : public processBase {
public:
  processDemo();
  virtual void operator()(SMIWaveformAnalyzer &wave);
  inline void setBaseLine(std::map<std::string, double> &_baseline){
    baseline = _baseline;}

private:
  void subtractBaseLine(SMIWaveformAnalyzer &wave);
  void printEvent(SMIWaveformAnalyzer &wave);

  unsigned int event;         //!< number of processed events
  SMIAnalyzerPluginList list; //!< list of plugins
  std::map<std::string, double> baseline;
};

#endif
