#ifndef __FFTFILTERWAVEFORMLOWPASS_HH__
#define __FFTFILTERWAVEFORMLOWPASS_HH__

#include"SMIWaveformAnalyzerPluginSystem.hh"

namespace plugin {
  namespace FFT {
    //! add a lowpass filter to an FFT plugin
    /*!
      performs a simple FFT lowpass filter by removing all frequencies 
      above a cut off
    */
    class LowPassFilter : public SMIAnalyzerPluginBase {
    public:
      //! constructor
      /*!
	sets the cut off frequency
	@param _cutOff frequency for cut off, is the timescale of the
	initial waveform is in ns the frequency unit is MHz
      */
      LowPassFilter(double _cutOff) : cutOff(_cutOff) {}
      
      //! operator to execute the plugin
      /*!
	@param wave waveform to operate on
      */
      virtual bool operator()(WaveForm &wave);

    private:
      double cutOff; //!< storage for cut off frequency

    };
  }
}

#endif
