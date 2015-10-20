#ifndef __PROCESSDEMO_H__
#define __PROCESSDEMO_H__

#include"SMIWaveformAnalyzerProcessSystem.hh"

#include<TH1D.h>
#include<TH2D.h>
#include<TTree.h>
#include<TFile.h>
#include<stdlib.h>
#include<stdio.h>

//#include"eventDataTypes.hh"

#if __cplusplus >= 201103L
#include <memory>
using std::shared_ptr;
#else
#include <tr1/memory>
using std::tr1::shared_ptr;
#endif

class onlineProcess : public processBase {
public:
  onlineProcess(std::string outName);
  ~onlineProcess(){
    //output->cd(); tree->Write(); output->Write();
    tree->AutoSave();
    for(size_t i=0; i<CFnum; i++){
      delete [] CF[i];
    }
    delete [] CF;
  }
  virtual void operator()(SMIWaveformAnalyzer &wave);

private:
  void addToAllChannels(SMIWaveformAnalyzer &wave, double time);
  void subtractTriggerOffset(SMIWaveformAnalyzer &wave);

  unsigned int event;           //!< number of processed events
  SMIAnalyzerPluginList list;   //!< list of plugins
  SMIAnalyzerPluginList listTr; //!< list of plugins

  TFile *output;
  TTree *tree;

  double LE    [32];
  double LEToT [32];
  double A     [32];
  double Charge[32];
  double **CF;
  size_t CFnum;
  std::vector<std::string>  CFNames;
};

#endif
