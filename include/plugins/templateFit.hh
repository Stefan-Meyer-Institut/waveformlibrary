#ifndef __TEMPLATEFIT_HH__
#define __TEMPLATEFIT_HH__
#ifdef __HAVEROOT

#include<string>

#include"SMIWaveformAnalyzerPluginSystem.hh"
#include"plugins/fitBase.hh"
namespace plugin {

  class templateFit : public fitBase {
  public:
    templateFit(std::string templatePath, double fitLow, double fitHigh):
      fitBase(fitLow, fitHigh, 3), currentData(NULL)
    {
      readWaveformTemplate(templatePath);
    }

    virtual bool operator()(WaveForm &wave);

  protected:
    virtual double errorFunction(const Double_t *par); 

  private:
    bool readWaveformTemplate(std::string &templatePath);
    void guessStartingParameters(Double_t *par, WaveForm &wave);
    bool fitData(Double_t *par);
    bool makeFTest(Double_t *par);
    bool produceOutput(bool converged, bool fTest, const Double_t *par, WaveForm &wave);

    WaveForm  templFunc;
    WaveForm *currentData;
    //    double    baseline;
    
  };

}
#endif
#endif
