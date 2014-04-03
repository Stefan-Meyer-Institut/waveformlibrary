#include"plugins/constantFractionLinear.hh"
#include<cmath>
#include<algorithm>
#include<vector>
#include<iostream>
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

namespace plugin {
  double constantFractionLinear::getTime(WaveForm &sumwave){
    const double xprec = 1e-3;
    const double yprec = 1e-3;

    // find staring value
    std::vector<double>::iterator res;
    if(sumwave.pulseType>=0) 
      res = std::max_element(sumwave.V.begin(),sumwave.V.end());
    else
      res = std::min_element(sumwave.V.begin(),sumwave.V.end());
    
    int sign = sgn(*res);
    double x1 = 0.0;
    for(size_t i=std::distance(sumwave.V.begin(),res); i>0;i--){
      if(sign != sgn(sumwave.V[i])){
	x1 = sumwave.t[i];
	break;
      }      
    }   

    double x2 = -9999.0;
    double f = sumwave.interpolate(x1);
    // newton method

    //std::cout << "starting pos: " << x1;
    // std::cout << " value: " << sumwave.interpolate(x1) << std::endl;;
     int counter = 0;
    if(sumwave.result["LEtime"] == -999999) return -999999;
    while(std::abs(x2-x1) > xprec || std::abs(f) > yprec ){
      x2 = x1;
      f = sumwave.interpolate(x1);
      // std::cout << "old pos: " << x1;
      // std::cout << " value: " << f << std::endl;;
      // std::cout << std::abs(x2-x1) << " " << f << std::endl;

      double fprime =  (sumwave.interpolate(x1+sumwave.dT) - sumwave.interpolate(x1-sumwave.dT))/(2*sumwave.dT);
      
      x1 = x1-(f/fprime);

      f = sumwave.interpolate(x1);
      // std::cout << "new pos: " << x1;
      // std::cout << " value: " << sumwave.interpolate(x1) << std::endl;;
      // std::cout << std::abs(x2-x1) << " " << f << std::endl;
      counter++;
      if(counter > 100) return -999999;
      
    }
    return x2;
  }

}
