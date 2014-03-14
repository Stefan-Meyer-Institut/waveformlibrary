#ifndef __SMIWAVEFORM_HH__
#define __SMIWAVEFORM_HH__

/*! \file SMIWaveform.hh
    \brief class for handling bunches of waveforms
*/ 

#include<map>
#include<string>

#if __cplusplus >= 201103L
#define CPP11
#include <random>
#else

#endif

#include"SMIWaveformBase.hh"

//! class for handling bunches of waveforms
/*!
  This class adds the possibilities to shift all waveforms at the same time,
  work with ADC information and randomize the binning effect of the ADC.

  If you use C++11 the appropriate STL classes from <random> are used, if you
  use the old C++ standard the C style std::rand() function is used.
 */
class SMIWaveform : public SMIWaveformBase {
public:
  //! standard constructor
  SMIWaveform();

  //! constrictor for waveforms that don't have 1024 entries
  /*!
    @param _nEntries number of entries in the waveforms
  */
  SMIWaveform(size_t _nEntries);
  
  //! copy data to correct waveform storage and randomize values
  /*!
    @param name channel name
    @param data double array with @ref nEntries entries
    @return true is name exists
   */
  bool fillChannel(std::string name, double *data);

  //! copy data to correct waveform storage and randomize values
  /*!
    @param name trigger name
    @param data double array with @ref nEntries entries
    @return true is name exists
   */
  bool fillTrigger(std::string name, double *data);

  //! add the appropriate randomness to a waveform
  /*!
    in case the waveforms can be directly filled from a file
    use this function to compensate the ADC binning effect.

    @param wave reference to waveform where randomness is needed
   */
  void addRandomness(WaveForm &wave);

  //! change the timescale of the event
  /*!
    @param time new timescale, all eventdata will be aligned to this new timebin
   */
  void changeTime(double time);

  //! shift the time axis of a single channel
  /*!
    @param name of the channel
    @param time time to shift the channel time axis (pos or neg)
    @return true if the channel name is found
   */
  inline bool shiftChannelTime(std::string name, double time);

  //! shift the time axis of a single trigger
  /*!
    @param name of the trigger
    @param time time to shift the trigger time axis (pos or neg)
    @return true if the trigger name is found
   */
  inline bool shiftTriggerTime(std::string name, double time);

  //! sets the number of bits of the ADC
  /*! 
    this function also calculates the distance between two ADC bins 
    for a compensation by randomization
    
    @param val new ADC bit number
  */
  inline void setADCBits (size_t val) {adcBits  = val; adcDiff = adcRange/(1<<adcBits);}

  //! sets the ADC range
  /*! 
    this function also calculates the distance between two ADC bins 
    for a compensation by randomization
    
    @param val new ADC range
  */
  inline void setADCRange(size_t val) {adcRange = val; adcDiff = adcRange/(1<<adcBits);}

protected:
  // get a random number between low and high
  inline double getRand(double low, double high);

  size_t adcBits;  //!< ADC bit
  double adcRange; //!< total range of the ADC
  double adcDiff;  //!< distance between two ADC bins

private:
  //! initialize random number generator
  /*!
    if C++11 is used the mersenne twister engine with a uniform distribution
    is used. otherwise the C type std::rand function
   */
  void initRand(); 

#ifdef CPP11
  std::random_device rd;                        //!< hardware random device for seed
  std::mt19937_64 gen;                          //!< 64 bit mersenne twister engine
  std::uniform_real_distribution<double> dist;  //! uniform distribution
#endif
};

#endif

