#include"plugins/calculateCharge.hh"

namespace plugin {
  bool calculateCharge::operator()(WaveForm &wave){
    double charge = 0.0;
    bool ret = false;
    for(size_t i=0; i<wave.V.size()-1; i++){
      if(wave.t[i] > lowGate && wave.t[i] < highGate){
	ret = true;
	charge += (wave.t[i+1] - wave.t[i])*(wave.V[i+1]+wave.V[i])*0.5;
      }
    }
    wave.result["Q"] = charge;
    return ret;
  }
}
