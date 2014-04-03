#include"plugins/subtractBaseLine.hh"
#include<cmath>
#include<vector>
#include<algorithm>

namespace plugin {
  
  bool subtractBaseLine::operator()(WaveForm &wave){
    WavePtr = &wave;
    static double step[3] = {0.001, 0.001, 0.001};

    // find gate iterator
    std::vector<double>::iterator it=wave.V.begin();
    size_t i = 0;
    for(i=0; i<wave.t.size() &&  wave.t[i]<highGate; i++) it++;

    // guess starting parameters

    double high = wave.interpolate(highGate);
    double low  = wave.interpolate(0);

    double B = low-high;
    double Bdt = B*0.36787944117144233;

    for(i=0; i<wave.t.size() && wave.t[i]<highGate && wave.V[i]-high>Bdt; i++);
    double t = wave.t[i];

    m.SetVariable(0, "A", high, step[0]);
     if(low/high < 1.1){
       m.SetFixedVariable(1,"B", 0);
       m.SetFixedVariable(2,"t", 1);
     } else {
      m.SetVariable(1, "B", B, step[1]);
      m.SetVariable(2, "t", t, step[1]);
     }
    //std::cout << high <<" "<<low << " " << B << " " << t << std::endl;
    m.Minimize();
    //m.PrintResults();
    const double *results = m.X();
    for(i=0; i<wave.V.size(); i++){
      wave.V[i] -= ABexpFunction(wave.t[i],results[0], results[1], results[2]);
    }
    wave.result["DCBGcomp"]     = results[0];
    wave.result["ExpBGcomp"]    = results[1];
    wave.result["ExpDecaycomp"] = results[2];
    return true;
  }
  
  /*!
    par[0] = A ... offset (DC component)
    par[1] = B ... strength of exp function
    par[2] = t ... decay time
  */
  double subtractBaseLine::chi2_fcn(const double *par){
    double chisq = 0.0;
    const double &A = par[0];
    const double &B = par[1];
    const double &t = par[2];

    if(WavePtr == NULL) return 0;

    for(size_t i=0; i<WavePtr->V.size(); i++){
      if(WavePtr->t[i] > lowGate && WavePtr->t[i] < highGate){
	double tmp = ABexpFunction(WavePtr->t[i], A, B, t);
	chisq += (WavePtr->V[i]-tmp) * (WavePtr->V[i]-tmp) ;
      }
    }

    return chisq;
  }
 
  //! f = A + B*exp(-x/t)
  inline double subtractBaseLine::ABexpFunction(const double &x, 
						const double &A, 
						const double &B, 
						const double &t) const {
    return A + B*std::exp(-x/t);
  }
  
}
