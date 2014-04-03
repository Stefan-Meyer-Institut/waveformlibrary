#include"plugins/calculateGatedMean.hh"
#include<cmath>
#include<iostream>
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
    
    //std::cout << "count " << count  << " sampleMean: "  << sampleMean << std::endl;

    // calculate sample variance
    for(size_t i=0; i<wave.t.size(); i++){
      if(lowGate < wave.t[i] && highGate>wave.t[i]){
	//std::cout << wave.V[i] << " " << sampleMean <<" vi-mean: "<<wave.V[i] - sampleMean << std::endl;
	maxLiklihoodVariance += 
	  (wave.V[i] - sampleMean) * (wave.V[i] - sampleMean);
      } 
      if(wave.t[i] > highGate) break;
    }
    //std::cout << maxLiklihoodVariance << std::endl;
    maxLiklihoodVariance /= count;
    //std::cout << maxLiklihoodVariance << std::endl;

    wave.result["BaseLineMean"  ] = sampleMean;
    wave.result["BaseLineStdDev"] = std::sqrt(maxLiklihoodVariance/count);
    return true;
  }
}
