#include"SMIWave.hh"
#include <exception>

/*! 
  \file SMIWave.cc
  \brief functions to work on @ref WaveForm objects
  \author Clemens Sauerzopf
*/ 

double WaveForm::interpolate(double _t) const {
  size_t above = 0;
  if(_t<=t[0]) return V[0];
  if(_t>=t[t.size()-1]) return V[V.size()-1];
  for(size_t i=1; i<V.size(); i++){
    if(_t>=t[i-1] && _t<= t[i]){
      above=i;
      break;
    }
  }
  return V[above-1]+(_t-t[above-1])*(V[above]-V[above-1])/dT;
}

WaveForm& WaveForm::operator=(const WaveForm &right){
  if(this == &right) return *this;
  V = right.V;
  t = right.t;
  result = right.result;
  isDigital = right.isDigital;
  return *this;
}

WaveForm& WaveForm::operator+=(const WaveForm &right){
  if(this == &right) {
    for(size_t i=0; i<V.size(); i++) V[i] *= 2;
    return *this;
  }
  if(t == right.t)
    for(size_t i=0; i<V.size(); i++) V[i] += right.V[i];
  else{
    for(size_t i=0; i<V.size(); i++) V[i] += right.interpolate(t[i]);
    }
  return *this;
}

WaveForm& WaveForm::operator+=(const double &right){
  for(size_t i=0; i<V.size(); i++) V[i] += right;
  return *this;
}

WaveForm& WaveForm::operator-=(const WaveForm &right){
    if(this == &right) {
    for(size_t i=0; i<V.size(); i++) V[i] = 0;
    return *this;
  }
  if(t == right.t)
    for(size_t i=0; i<V.size(); i++) V[i] -= right.V[i];
  else{
    for(size_t i=0; i<V.size(); i++) V[i] -= right.interpolate(t[i]);
    }
  return *this;
}

WaveForm& WaveForm::operator-=(const double &right){
  for(size_t i=0; i<V.size(); i++) V[i] -= right;
  return *this;
}

WaveForm& WaveForm::operator*=(const double &right){
  for(size_t i=0; i<V.size(); i++) V[i] *= right;
  return *this;
}

WaveForm operator+(const WaveForm &left, const WaveForm &right){
  WaveForm wave = left;
  wave += right;
  return wave;
}

WaveForm operator+(const WaveForm &left, const double &right){
  WaveForm wave = left;
  wave += right;
  return wave;
}

WaveForm operator-(const WaveForm &left, const WaveForm &right){
  WaveForm wave = left;
  wave -= right;
  return wave;
}

WaveForm operator-(const WaveForm &left, const double &right){
  WaveForm wave = left;
  wave -= right;
  return wave;
}

WaveForm operator*(const WaveForm &left, const double &right){
  WaveForm wave = left;
  wave *= right;
  return wave;
}


std::ostream& operator<< (std::ostream &out, WaveForm &Info){
  std::map<std::string, double>::iterator it;
  for(it=Info.result.begin(); it!=Info.result.end(); it++)
    out << it->second << " ";
  out << Info.isDigital;
  return out;
}
