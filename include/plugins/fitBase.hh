#ifndef __FITBASE_HH__
#define __FITBASE_HH__
#ifdef __HAVEROOT

#include "TMath.h"
#include "Minuit2/Minuit2Minimizer.h"
#include "Math/Functor.h"

#include"SMIWaveformAnalyzerPluginSystem.hh"
namespace plugin {

  //! pure virtual base class for fit plugins
  /*!
    This base class provides a unified interface for writing plugins that do any
    type of fitting on wavefrom data. The method used is MINUIT 2 with the 
    combined MIDGARD and SIMPLEX algorithms. This class is pure virtual.
   */
  class fitBase : public SMIAnalyzerPluginBase {
  public:
    //! constructor
    /*!
      initialises the plugin
      @param fitLow low bound on x axis domain for fitting
      @param fitHigh high bound on x axis domain for fitting
      @param NDF number of degrees of freedom for the fit
     */
    fitBase(double fitLow, double fitHigh, Int_t NDF)
      : minimiser   (          ROOT::Minuit2::kMigrad), 
    //: minimiser   (          ROOT::Minuit2::kCombined), 
    //: minimiser   (          ROOT::Minuit2::kSimplex), 
	errorFunctor(this, &fitBase::errorFunction, NDF)
    {
      fitLowBound  = fitLow;
      fitHighBound = fitHigh;
      minimiser.SetTolerance(0.001);
      minimiser.SetFunction(errorFunctor);
    }

  protected:
    //! pure virtual function, used for calculating chi^2
    virtual double errorFunction(const Double_t *par) = 0; 

    double fitLowBound;                           //!< low x axis bound
    double fitHighBound;                          //!< high x axis bound
    
    ROOT::Minuit2::Minuit2Minimizer minimiser;    //!< the Minut2 minimiser
  private:
 
    ROOT::Math   ::Functor          errorFunctor; //!< functor for error function  

  };

}
#endif
#endif
