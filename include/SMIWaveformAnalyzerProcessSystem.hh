#ifndef __SMIWAVEFORMANALYZERPROCESSSYSTEM_HH__
#define __SMIWAVEFORMANALYZERPROCESSSYSTEM_HH__

#include"SMIWaveformAnalyzerPluginSystem.hh"

//stl 
#include<map>
#include<string>

class SMIWaveformAnalyzer;

//! abstract base class for processing functions
/*!
  this base class assures that the processing functions all share the same
  interface, the operator is used within the calling loop function of the
  waveform classes to process the analysis
 */
class processBase {
public:
  //! standatd constructor
  processBase(){}
  //! virtual destructor
  virtual ~processBase(){}

  //! pure virtual operator
  virtual void operator()(SMIWaveformAnalyzer &wave) = 0;

  std::map<std::string, double> result; //!< storage for results
};

//! calculates the pedestal of all channels
/*!
  the pedestal calculation is done in two steps, at first the mean and 
  standard deviation in a gated area of the waveform is calculated, then
  this information is used zo calculate a weighted avarage of all analyzed 
  waveforms. The resulting pedestal information is then returend by a call to
  @ref getBaseline(SMIWaveformAnalyzer &wave) .
 */
class processCalculateBaseline : public processBase {
public:

  //! standard constructor
  processCalculateBaseline();

  //! operator, analyzes the waveforms and collects pedestal information
  /*!
    for the single events. 
    @param wave reference to the waveform informations
   */
  virtual void operator()(SMIWaveformAnalyzer &wave);

  //! calculate the final pedestal informations
  /*!
    the final pedestal information is calculated by generating a weighted
    avarage of all analyzed events:
    \f[
    \omega_i = \frac{1}{\sigma_i^2} \,\,\, \mathrm{M} = \frac{\sum x_i \omega_i}{\sum \omega_i}
    \f]

    @param wave reference to the waveform informations
    @return standard map with channel names as keys and the pedestal as value
   */
  std::map<std::string, double> getBaseline(SMIWaveformAnalyzer &wave);

private:
  unsigned int event;         //!< number of processed events
  SMIAnalyzerPluginList list; //!< list of plugins
};

#endif
