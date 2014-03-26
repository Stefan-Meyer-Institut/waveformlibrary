#ifndef __LEADINGEDGELINEAR_HH__
#define __LEADINGEDGELINEAR_HH__

#include"plugins/leadingEdgeBasic.hh"

namespace plugin {
  class leadingEdgeLinear : public leadingEdgeBasic {
  public:
    leadingEdgeLinear() : leadingEdgeBasic() {}
    leadingEdgeLinear(double low, double high, double thresh) : 
      leadingEdgeBasic(low,high,thresh) {}

  protected:
    virtual double getTime(WaveForm &wave, std::vector<double>::iterator &it);    
  };
}

#endif
