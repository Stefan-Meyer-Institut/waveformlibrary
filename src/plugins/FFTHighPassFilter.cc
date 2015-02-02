#include"plugins/FFTHighPassFilter.hh"
namespace plugin {
  namespace FFT {
    bool HighPassFilter::operator()(WaveForm &wave){
      if(wave.t.size()==0) return false;
      for(size_t i=0; i<wave.t.size(); i++){
	if(wave.t[i] <= cutOff) wave.V[i]=0;
      }
      return true;
    }
  }
}
