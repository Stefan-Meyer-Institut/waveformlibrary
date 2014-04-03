#ifndef __PROCESSDEMO_H__
#define __PROCESSDEMO_H__

#include"SMIWaveformAnalyzerProcessSystem.hh"

#include<TH1D.h>
#include<TH2D.h>

class processDemo : public processBase {
public:
  processDemo();
  virtual void operator()(SMIWaveformAnalyzer &wave);
  inline void setBaseLine(std::map<std::string, double> &_baseline){
    baseline = _baseline;}

  void Print();
private:
  void subtractBaseLine(SMIWaveformAnalyzer &wave);
  void printEvent(SMIWaveformAnalyzer &wave);

  unsigned int event;         //!< number of processed events
  SMIAnalyzerPluginList list; //!< list of plugins
  std::map<std::string, double> baseline;
  TH1D **h;
  TH1D **h3;
  TH2D **h2;
};

#endif
