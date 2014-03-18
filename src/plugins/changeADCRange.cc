#include"plugins/changeADCRange.hh"

namespace plugin {
  changeADCRange::changeADCRange(double factor) :
    scalingfactor(factor){

  }

  bool changeADCRange::operator()(WaveForm &wave){
    wave *= scalingfactor;
    wave.result["ADCRangeUnit"] = scalingfactor;
    return true;
  }
}
