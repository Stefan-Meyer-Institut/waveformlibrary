#ifndef __SMIWAVE_HH__
#define __SMIWAVE_HH__

#include<vector>
#include<string>
#include<map>
#include<cstdio>
#include<ostream>

class WaveForm {
public:
  WaveForm() : V(1024,0.0), t(1024,0.0), dT(0.2), isDigital(false) {
    for(size_t i=0; i<t.size(); i++) t[i] = i*0.2; // [ns] 1 bin = 200 ps
  }

  inline void setTimeBins(double _t){
    for(size_t i=0; i<t.size(); i++) t[i] = i*_t; dT=_t;}
  inline void shiftTime(double _t){
    for(size_t i=0; i<t.size(); i++) t[i] = t[i] + _t;}

  inline void resizeWave(size_t _s) {V.resize(_s); t.resize(_s);}

  double interpolate(double t) const;

  std::vector<double> V;         // voltage [Volt]
  std::vector<double> t;         // time    [ns]
  double dT;

  std::map<std::string, double> result;
  bool                          isDigital; // is a digital pulse

  WaveForm& operator=(const WaveForm &right);
  WaveForm& operator+=(const WaveForm &right);
  WaveForm& operator+=(const double &right);
  WaveForm& operator-=(const WaveForm &right);
  WaveForm& operator-=(const double &right);
  WaveForm& operator*=(const double &right);

  friend WaveForm operator+(const WaveForm &left, const WaveForm &right);
  friend WaveForm operator+(const WaveForm &left, const double &right);
  friend WaveForm operator-(const WaveForm &left, const WaveForm &right);
  friend WaveForm operator-(const WaveForm &left, const double &right);
  friend WaveForm operator*(const WaveForm &left, const double &right);
  friend std::ostream& operator<< (std::ostream &out, WaveForm &Info);

  
};

#endif
