#ifndef __CONTANTFRACTIONLINEAR_HH__
#define __CONTANTFRACTIONLINEAR_HH__

#include"plugins/constantFractionBase.hh"

namespace plugin {
  class constantFractionLinear : public constantFractionBase {
  public:
    constantFractionLinear() : constantFractionBase() {}

  protected:
    virtual double getTime(WaveForm &sumwave);
  };
}


#endif
