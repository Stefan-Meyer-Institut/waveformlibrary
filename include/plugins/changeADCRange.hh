#ifndef __CHANGEADCRANGE_HH__
#define __CHANGEADCRANGE_HH__

#include"SMIWaveformAnalyzerPluginSystem.hh"
#include<string>

namespace plugin {

  //! simple plugin to rescale the ADC range
  /*!
    this plugin rescales all values of a waveform with a given factor
    and stores the scaling factor as "ADCRangeUnit" in the results.
   */
  class changeADCRange : public SMIAnalyzerPluginBase {
  public: 
    //! constructor
    /*!
      @param factor to scale the waveform with
     */
    changeADCRange(double factor);

    //! operator to apply the plugin
    virtual bool operator()(WaveForm &wave);

  private:
    //! standard constructor, made private to prohibit use
    changeADCRange(){}

    double scalingfactor; //!< private storage for scaling factor
  };

}

#endif
