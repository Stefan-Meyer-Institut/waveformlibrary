#include"plugins/FFTFilterWaveForm.hh"

#include<cmath>
#include<iostream>


#include<TMath.h>
#include<TAxis.h>



namespace plugin {
  FFTFilterWaveForm::FFTFilterWaveForm() {  
    m.SetMaxFunctionCalls(1000000);
    m.SetMaxIterations(100000);
    m.SetTolerance(0.001);
  }

  bool FFTFilterWaveForm::operator()(WaveForm &wave){
    WaveForm FFT = wave;
    
  }
}
