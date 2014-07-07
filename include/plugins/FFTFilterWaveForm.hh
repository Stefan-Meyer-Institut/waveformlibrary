#ifndef __FFTFILTERWAVEFORM_HH__
#define __FFTFILTERWAVEFORM_HH__

#include"SMIWaveformAnalyzerPluginSystem.hh"

#include<TVirtualFFT.h>
#include<Minuit2/Minuit2Minimizer.h>
#include<Math/Functor.h>


namespace plugin {
  class FFTFilterWaveForm : public SMIAnalyzerPluginBase {
  public: 
FFTFilterWaveForm();
    virtual bool operator()(WaveForm &wave);

  private:
    TVirtualFFT *fft_forward;           //!< make FFT
    TVirtualFFT *fft_backward;          //!< perform back transformation
    ROOT::Minuit2::Minuit2Minimizer  m; //!< The Minut2 minimizer
  };
}

#endif
