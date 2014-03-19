#include"plugins/calculateGatedMean.hh"
#include<cmath>

namespace plugin {
  calculateGatedMean::calculateGatedMean(double low, double high) :
    lowGate(low), highGate(high) {}

  bool calculateGatedMean::operator()(WaveForm &wave){
    unsigned int count = 0;
    double sampleMean = 0.0;
    double maxLiklihoodVariance = 0.0;

    // calculate sample mean
    for(size_t i=0; i<wave.t.size(); i++){
      if(lowGate < wave.t[i] && highGate>wave.t[i]){
	sampleMean += wave.V[i];
	count++;
      } 
      if(wave.t[i] > highGate) break;
    }
    if(count == 0) return false;
    sampleMean /= count;
    
    // calculate sample variance
    for(size_t i=0; i<wave.t.size(); i++){
      if(lowGate < wave.t[i] && highGate>wave.t[i]){
	maxLiklihoodVariance += 
	  (wave.V[i] - sampleMean) * (wave.V[i] - sampleMean);
      } 
      if(wave.t[i] > highGate) break;
    }
    maxLiklihoodVariance /= count;

    wave.result["BaseLineMean"  ] = sampleMean;
    wave.result["BaseLineStdDev"] = std::sqrt(maxLiklihoodVariance/count);
    return true;
  }
}
