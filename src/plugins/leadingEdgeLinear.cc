#include"plugins/leadingEdgeLinear.hh"

namespace plugin {
  double leadingEdgeLinear::getTime(WaveForm &wave, 
				    std::vector<double>::iterator &it){
    std::vector<double>::iterator it1 = it + 1;
    double t0 = wave.t[std::distance(wave.V.begin(), it )];
    double t1 = wave.t[std::distance(wave.V.begin(), it1)];
    if(*it1 != *it)
      return ( (t1-t0)/(*it1 - *it) ) * (threshold - *it) - t0;
    else 
      return -999999;
  }
}
