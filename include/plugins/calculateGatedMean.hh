#ifndef __CALCULATEGATEDMEAN_HH__
#define __CALCULATEGATEDMEAN_HH__

#include"SMIWaveformAnalyzerPluginSystem.hh"

namespace plugin {

  //! this plugin removes the chattering from the channels
  /*! 
    calculates the mean value of all entries within the gate, assuming a
    standard normal distribution an maximum likelihood error estimate is given. 
    This plugin adds the keys \a "BaseLineMean" and \a "BaseLineStdDev" to 
    the results.    
   */
  class calculateGatedMean : public SMIAnalyzerPluginBase {
  public:
    //! standard constructor
    calculateGatedMean() : lowGate(0), highGate(20){}
    //! constructor with explicit low and high bounding value for the gate
    /*!
      @param low bounding value of the gate
      @param high bounding value of the gate
     */
    calculateGatedMean(double low, double high);

    //! operator that acts on the waveform
    /*!
      at this point the maximum likelihood estimates for the mean and the
      variance are calculated, the results are stured within the keys:
      \a "BaseLineMean" and \a "BaseLineStdDev"
     */
    virtual bool operator()(WaveForm &wave);

  private:
    double lowGate;  //!< lower bound value
    double highGate; //!< higher bound value
  };

}

#endif
