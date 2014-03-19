#include"plugins/cleanWaveForm.hh"

namespace plugin {
  cleanWaveForm::cleanWaveForm(double cut, double good) :
    cutVal(cut), goodVal(good) {}

  bool cleanWaveForm::operator()(WaveForm &wave){
    // remove chattering
    if(wave.pulseType <= 0) { // negative or digital pulse
      for(size_t i=0; i<wave.V.size(); i++){
	if(wave.V[i] > cutVal) continue;
	if(i==0){
	  size_t j = 5;
	  for(; j<wave.V.size(); j++){
	    if(wave.V[j] > goodVal) break;	    
	  }
	  wave.V[i] = wave.V[j];
	}
	if(i>=1 &&  wave.V[i-1] > goodVal){
	  wave.V[i] = wave.V[i-1];
	}
      }
    } // negative pulse end 

    if(wave.pulseType > 0) { // positive pulse
      for(size_t i=0; i<wave.V.size(); i++){
	if(wave.V[i] < cutVal) continue;
	if(i==0){
	  size_t j = 5;
	  for(; j<wave.V.size(); j++){
	    if(wave.V[j] < goodVal) break;	    
	  }
	  wave.V[i] = wave.V[j];
	}
	if(i>=1 &&  wave.V[i-1] < goodVal){
	  wave.V[i] = wave.V[i-1];
	}
      }      
    } // positive pulse end
    return true;
  }
 
}
