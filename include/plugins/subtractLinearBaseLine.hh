#ifndef __SUBTRACTBASELINE_HH__
#define __SUBTRACTBASELINE_HH__

#include "plugins/calculateGatedMean.hh"
#include "TMinuit.h"
#include "Minuit2/Minuit2Minimizer.h"
#include "Math/Functor.h"

namespace plugin {
  class subtractLinearBaseLine : public plugin::calculateGatedMean {
  public:
    subtractLinearBaseLine() : calculateGatedMean() {}
    subtractLinearBaseLine(double low, double high) 
      : calculateGatedMean(low, high) {}
    virtual bool operator()(WaveForm &wave);
  };
}

#endif
