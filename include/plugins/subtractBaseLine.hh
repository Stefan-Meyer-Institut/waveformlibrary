#ifndef __SUBTRACTBASELINE_HH__
#define __SUBTRACTBASELINE_HH__

#include"SMIWaveformAnalyzerPluginSystem.hh"

#include "TMinuit.h"
#include "Minuit2/Minuit2Minimizer.h"
#include "Math/Functor.h"

namespace plugin {
  class subtractBaseLine : public SMIAnalyzerPluginBase {
  public:
    subtractBaseLine() : m(ROOT::Minuit2::kMigrad), 
			 f(this,&subtractBaseLine::chi2_fcn,3), WavePtr(NULL),
			 lowGate(20), highGate(55) {
      m.SetMaxFunctionCalls(10000000);
      m.SetMaxIterations(1000000);
      m.SetTolerance(0.01);
      m.SetFunction(f);
    }
    virtual bool operator()(WaveForm &wave);
    
  protected:
    inline double ABexpFunction(const double &x, const double &A, 
				const double &B, const double &t) const;

  private:
    double chi2_fcn(const double *par);

    ROOT::Minuit2::Minuit2Minimizer  m; //!< The Minut2 minimizer
    ROOT::Math::Functor f;
    WaveForm *WavePtr;

    double lowGate;
    double highGate;
  };
}

#endif
