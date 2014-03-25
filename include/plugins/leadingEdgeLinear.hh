#ifndef __LEADINGEDGELINEAR_HH__
#define __LEADINGEDGELINEAR_HH__

#include"plugins/leadingEdgeBasic.hh"

namespace plugin {
  class leadingEdgeLinear : public leadingEdgeBasic {
  protected:
    virtual double getTime(WaveForm &wave, std::vector<double>::iterator &it);    
  };
}

#endif
