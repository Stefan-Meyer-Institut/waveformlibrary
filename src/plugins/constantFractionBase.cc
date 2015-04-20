#include"plugins/constantFractionBase.hh"

#include<iostream>
namespace plugin {
  bool constantFractionBase::operator()(WaveForm &wave){
    WaveForm Wave = wave;
    WaveForm invWave = wave*0.4;
    Wave.shiftTime(delay);
    
    WaveForm sum = Wave-invWave;
    char tmp[128];
    sprintf(tmp,"CF-%.2f",delay);
    wave.result[tmp] = getTime(sum);
    //std::cout << tmp << " "  << wave.result[tmp]; 
    // for(int i=0; i<1024; i++){
    //   std::cout << Wave.t[i] << " " << Wave.V[i] << " " 
    // 		<< invWave.t[i] << " " << invWave.V[i] << " " 
    // 		<< sum.t[i] << " " << sum. V[i] << std::endl;
    // }
    //exit(0);
    return true;
  }


}
