#ifndef __FITBASE_HH__
#define __FITBASE_HH__
#ifdef __HAVEROOT

#include "TMath.h"
#include "Minuit2/Minuit2Minimizer.h"
#include "Math/Functor.h"

#include"SMIWaveformAnalyzerPluginSystem.hh"
namespace plugin {

  class fitBase : public SMIAnalyzerPluginBase {
  public:
    fitBase(double fitLow, double fitHigh, Int_t NDF)
    //: minimiser   (          ROOT::Minuit2::kMigrad), 
      : minimiser   (          ROOT::Minuit2::kCombined), 
	errorFunctor(this, &fitBase::errorFunction, NDF)
    {
      fitLowBound  = fitLow;
      fitHighBound = fitHigh;
      minimiser.SetFunction(errorFunctor);
    }

  protected:
    virtual double errorFunction(const Double_t *par) = 0; 
    double fitLowBound;
    double fitHighBound;
    
    ROOT::Minuit2::Minuit2Minimizer minimiser;    //!< the Minut2 minimiser
  private:
 
    ROOT::Math   ::Functor          errorFunctor; //!< functor for error function  

  };

}
#endif
#endif
