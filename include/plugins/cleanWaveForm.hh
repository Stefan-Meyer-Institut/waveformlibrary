#ifndef __CLEANWAVEFORM_HH__
#define __CLEANWAVEFORM_HH__

#include"SMIWaveformAnalyzerPluginSystem.hh"

namespace plugin {

  //! this plugin removes the chattering from the channels
  /*! 
    sometimes when digitising pulses that are outside of the ADC range
    a chattering appears, this can be removed by applying this plugin
   */
  class cleanWaveForm : public SMIAnalyzerPluginBase {
  public:
    //! standard constructor
    cleanWaveForm() : cutVal(1), goodVal(3900){}
    //! constructor with cut value and good value
    /*!
      @param cut cutoff value, when processing a positive pulse it V smaller 
      than cut and when processing a negative pulse V greater than cut
      @param good value that is considered "good" for positive pulse,
      accepted if V<good and for negative pulse V>good
     */
    cleanWaveForm(double cut, double good);

    //! operator that acts on the waveform
    virtual bool operator()(WaveForm &wave);

  private:
    double cutVal;  //!< cut value
    double goodVal; //!< good value
  };

}

#endif
