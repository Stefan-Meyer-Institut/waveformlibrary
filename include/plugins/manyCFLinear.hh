#ifndef __MANYCFLINEAR_HH__
#define __MANYCFLINEAR_HH__

#include"SMIWaveformAnalyzerPluginSystem.hh"
#include"plugins/constantFractionLinear.hh"


#if __cplusplus >= 201103L
#include <memory>
using std::shared_ptr;
#else
#include <tr1/memory>
using std::tr1::shared_ptr;
#endif

namespace plugin {
  class manyCFLinear : public SMIAnalyzerPluginBase {
  public:
    manyCFLinear(double start, double end, unsigned int n);
    ~manyCFLinear(){}
    virtual bool operator()(WaveForm &wave);
    inline void add(plugin::constantFractionLinear* p) 
    {cf.push_back(shared_ptr<plugin::constantFractionLinear>(p));}

    std::vector<std::string> names;
  protected:
    unsigned int num;
    std::vector<shared_ptr<plugin::constantFractionLinear> > cf; //!< storage container for the plugin smart pointers

  };
}

#endif
