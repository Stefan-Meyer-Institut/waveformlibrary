#ifndef __SMIWAVE_HH__
#define __SMIWAVE_HH__

/*! \file SMIWave.hh
    \brief class defintion of a single waveform
*/ 

#include<vector>
#include<string>
#include<map>
#include<cstdio>
#include<ostream>

//! class definition for representing a single waveform
/*!
  The WaveForm class stores a single waveform and supplies 
  functions and operators to modify this waveform. Additinally
  the results of operations from plugins on this waveform 
  can be stored in the @result map.
 */
class WaveForm {
public:
  //! constructor
  /*!
    The standard constructor initializes the Values to 0 and the 
    time axis to 1024 bins with 0.2 ns binwidth. It also assumes
    that the pulse is negative
   */
  WaveForm() : V(1024,0.0), t(1024,0.0), dT(0.2), pulseType(-1) {
    for(size_t i=0; i<t.size(); i++) t[i] = i*0.2; // [ns] 1 bin = 200 ps
  }

  //! change the time axis bin width
  /*!
    the bin width of the time axis is change to i*_t
    @param _t new time bin width
   */
  inline void setTimeBins(double _t){
    for(size_t i=0; i<t.size(); i++) t[i] = i*_t; dT=_t;}

  //! shift the time axis
  /*!
    this function shifts the time axis by _t, this function can be used
    for example to implement a CFD analysis
    @param _t time to shift the time axis [standard time  = ns]
   */
  inline void shiftTime(double _t){
    for(size_t i=0; i<t.size(); i++) t[i] = t[i] + _t;}

  //! resize the length of the wave (number of bins)
  /*!
    the resizeWave function changes the number of entries in V and t
    by truncation the vectors
   */
  inline void resizeWave(size_t _s) {V.resize(_s); t.resize(_s);}

  //! this function returns a linear interpolated point
  /*!
    the interpolate function does a simple linear interpolation between 
    to neighboring time bins.

    @param t desired point on the time axis
    @return the corresponding V value to time t
   */
  double interpolate(double t) const;

  std::vector<double> V;         //!< voltage [Volt]
  std::vector<double> t;         //!< time    [ns]
  double dT;                     //!< time bin width 

  std::map<std::string, double> result;    //!< map to store the results
  int                           pulseType; //!< is a digital pulse

  WaveForm& operator=(const WaveForm  &right);
  WaveForm& operator+=(const WaveForm &right);
  WaveForm& operator+=(const double   &right);
  WaveForm& operator-=(const WaveForm &right);
  WaveForm& operator-=(const double   &right);
  WaveForm& operator*=(const double   &right);

  friend WaveForm operator+(const WaveForm &left, const WaveForm &right);
  friend WaveForm operator+(const WaveForm &left, const double &right);
  friend WaveForm operator-(const WaveForm &left, const WaveForm &right);
  friend WaveForm operator-(const WaveForm &left, const double &right);
  friend WaveForm operator*(const WaveForm &left, const double &right);
  friend std::ostream& operator<< (std::ostream &out, WaveForm &Info);

};

#endif
