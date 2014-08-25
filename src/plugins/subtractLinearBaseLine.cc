#include"plugins/subtractLinearBaseLine.hh"
#include<cmath>
#include<vector>
#include<algorithm>

namespace plugin {
  
  bool subtractLinearBaseLine::operator()(WaveForm &wave){
    bool retVal = calculateGatedMean::operator()(wave);
    for(size_t i=0; i<wave.V.size(); i++){
      wave.V[i] -= wave.result["BaseLineMean"] ;
    }

    return retVal;
  }

}
