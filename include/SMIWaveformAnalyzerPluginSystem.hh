#ifndef __SMIWAVEFORMANALYZERPLUGINSYSTEM_HH__
#define __SMIWAVEFORMANALYZERPLUGINSYSTEM_HH__

#include<vector>

#if __cplusplus >= 201103L
#include <memory>
using std::shared_ptr;
#else
#include <tr1/memory>
using std::tr1::shared_ptr;
#endif

#include"SMIWave.hh"

class SMIAnalyzerPluginBase {
public:
  SMIAnalyzerPluginBase(){}
  virtual ~SMIAnalyzerPluginBase(){};
  virtual bool operator()(WaveForm &wave) = 0;
};

class SMIAnalyzerPluginList {
public:
  SMIAnalyzerPluginList(){};
  ~SMIAnalyzerPluginList();

  inline void add(SMIAnalyzerPluginBase* plugin) 
  {plugins.push_back(shared_ptr<SMIAnalyzerPluginBase>(plugin));}
  inline shared_ptr<SMIAnalyzerPluginBase> & operator[](size_t pos) {return plugins[pos];}
  
  inline size_t size() const {return plugins.size();}

private:
  std::vector<shared_ptr<SMIAnalyzerPluginBase> > plugins;
};

class SMIAnalyzerPluginDummy : public SMIAnalyzerPluginBase {
public:
  SMIAnalyzerPluginDummy(int i) : dummy(i) {}
  virtual bool operator()(WaveForm &wave) {
    char tmp[20];
    sprintf(tmp,"Test%i",dummy);
    wave.result[tmp] = (double)dummy/10.;
    return true;
  }

  int dummy;
};

#endif
