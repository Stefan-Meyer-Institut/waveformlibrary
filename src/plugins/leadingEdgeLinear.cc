#include"plugins/leadingEdgeLinear.hh"
#include<iostream>
#include<cmath>
namespace plugin {
  double leadingEdgeLinear::getTime(WaveForm &wave, 
				    std::vector<double>::iterator &it){
    std::vector<double>::iterator it1 = it + 1;
    double t0 = wave.t[std::distance(wave.V.begin(), it )];
    double t1 = wave.t[std::distance(wave.V.begin(), it1)];
    double a0 = std::abs(*it);
    double a1 = std::abs(*it1);
    
    // std::cout << t0 << " " << t1 << " " << a0 << " " <<a1 << " " 
    // 	      << threshold << std::endl;
    // std::cout << (t1-t0) << " " << (a1 - a0) << " " << (threshold - a0)
    // 	      << std::endl;
    // std::cout << ( (t1-t0)/(a1 - a0) ) * (threshold - a0) + t0 << std::endl;
    if(*it1 != *it)
      return ( (t1-t0)/(a1 - a0) ) * (threshold - a0) + t0;
    else 
      return -999999;
  }
}
