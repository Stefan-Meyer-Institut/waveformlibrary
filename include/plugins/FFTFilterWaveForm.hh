#ifndef __FFTFILTERWAVEFORM_HH__
#define __FFTFILTERWAVEFORM_HH__

#ifdef __HAVEROOT

#include"SMIWaveformAnalyzerPluginSystem.hh"

#include<TVirtualFFT.h>

namespace plugin {
  //! FFT base plugin, requires plugins from plugin::FFT:: namespace
  /*!
    This plugins performs a Fast-Fourrier-Transform of a real input signal 
    given to the plugin via WaveForm wave. While processing the FFT is computed
    once and a set of FFT plugins operates (stored in fftPlugins via the 
    @ref add function) on the waveform to do filtering for example.
   */
  class FFTFilterWaveForm : public SMIAnalyzerPluginBase {
  public: 
    //! constructor requires number of entries in waveform
    FFTFilterWaveForm(Int_t nDim);
    ~FFTFilterWaveForm();
    //! operator to execute the plugin
    /*!
      @param wave waveform to operate on
     */
    virtual bool operator()(WaveForm &wave);

    //! inline function, stires plugin in container
    /*!
      @param p pointer to FFT plugin
     */
    inline void add(SMIAnalyzerPluginBase *p)
    { fftPlugins.push_back(p); }

  private:
    TVirtualFFT *fft_forward;                        //!< make FFT
    TVirtualFFT *fft_backward;                       //!< perform back transformation
    double *valsReal;
    double *valsComp;
    
    std::vector<SMIAnalyzerPluginBase *> fftPlugins; //!< plugin container
  };
}

#endif
#endif
