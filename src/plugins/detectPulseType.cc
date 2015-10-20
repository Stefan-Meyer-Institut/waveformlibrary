#include"plugins/detectPulseType.hh"
#include<iostream>
namespace plugin {
  bool detectPulseType::operator()(WaveForm &wave){
    bool retVal = true;
    if(wave.result["BaseLineMean"  ] > 0.5)
      wave.pulseType = -1;
    else
      wave.pulseType =  1;
    return retVal;
  }
}
