#include"plugins/manyCFLinear.hh"
#include<iostream>
namespace plugin {
  manyCFLinear::manyCFLinear(double start, double end, unsigned int n){
    num = n;

    for(double i=start; i<=end; i+=(end-start)/(num)){
      add(new plugin::constantFractionLinear(i));
      char tmp[128];
      sprintf(tmp,"CF-%.2f",i);
      names.push_back(tmp);
    }
  }

  bool manyCFLinear::operator()(WaveForm &wave){
    bool retVal = true;
    for(unsigned int i=0; i<num; i++){
      retVal &= (*cf[i])(wave);
    }
    return retVal;
  }
}
