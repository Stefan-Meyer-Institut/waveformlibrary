#ifndef __LEADINGEDGEBASIC_HH__
#define __LEADINGEDGEBASIC_HH__

#include"SMIWaveformAnalyzerPluginSystem.hh"

namespace plugin {

  //! this plugin calculates the leading edge time of a signal without interpolation
  /*! 

   */
  class leadingEdgeBasic : public SMIAnalyzerPluginBase {
  public:
    //! standard constructor
    leadingEdgeBasic() : lowGate(20), highGate(40), threshold(40) {}

    leadingEdgeBasic(double low, double high, double thresh);
    //! operator that acts on the waveform
    /*!
     */
    virtual bool operator()(WaveForm &wave);

  protected:
    virtual double getTime(WaveForm &wave, std::vector<double>::iterator &it);

    double lowGate;   //!< lower bound value
    double highGate;  //!< higher bound value
    double threshold; //!< trigger threshold

  };

}

#endif
