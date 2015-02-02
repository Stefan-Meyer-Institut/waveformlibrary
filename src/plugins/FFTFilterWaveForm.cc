#ifdef __HAVEROOT

#include"plugins/FFTFilterWaveForm.hh"

#include<cmath>
#include<iostream>


#include<TMath.h>
#include<TAxis.h>

#include<iostream>

namespace plugin {
  FFTFilterWaveForm::FFTFilterWaveForm(Int_t nDim) {  
    fft_forward  = TVirtualFFT::FFT(1, &nDim, "R2C EX K");
    fft_backward = TVirtualFFT::FFT(1, &nDim, "C2R EX K");
    valsReal = new double[1024];
    valsComp = new double[1024];
  }

  FFTFilterWaveForm::~FFTFilterWaveForm() {  
    delete [] valsReal;
    delete [] valsComp;
  }

  bool FFTFilterWaveForm::operator()(WaveForm &wave){
    WaveForm FFT; 
    FFT.resizeWave(wave.V.size()*2);
    //WaveForm FFT2; // compiler bug in clang++ 3.3
  
    //FFT.shiftTime(wave.t[0]);
    //FFT.result["ASD"] = 0;
    if(!fft_forward || !fft_backward) return false;

    bool retVal = true;
    
    fft_forward->SetPoints(&(wave.V[0]));
    fft_forward->Transform();
    fft_forward->GetPointsComplex(valsReal, valsComp);

    for(size_t i=0; i<FFT.t.size()/4; i++){
      FFT.t[i] = i/FFT.dT; // gives units of MHz if t axis is in ns
      FFT.t[FFT.t.size()/2-i-1] = FFT.t[i];
      }

    for(int i=0; i<1024; i++){
      valsReal[i] *= 1/TMath::Sqrt(wave.V.size());
      valsComp[i] *= 1/TMath::Sqrt(wave.V.size());
      FFT.V[i] = valsReal[i];
      FFT.V[i+1024] = valsComp[i];
      FFT.t[i+1024] = FFT.t[i];  
      //std::cout << "Ts: " <<  FFT.t[i+1024] << " " <<  FFT.t[i] << std::endl;
    }
    
    // do some work on the fourier transform
    for(size_t i=0; i<fftPlugins.size(); i++){
      retVal &= (*fftPlugins[i])(FFT);
      }

    for(int i=0; i<1024; i++){
      valsReal[i] = FFT.V[i];
      valsComp[i] = FFT.V[i+1024];      
    }

    // back transform result
    fft_backward->SetPointsComplex(valsReal, valsComp);
    fft_backward->Transform();
    fft_backward->GetPointsComplex(valsReal, valsComp);
    for(int i=0; i<1024; i++){
      valsReal[i] *= 1/TMath::Sqrt(wave.V.size());
      valsComp[i] *= 1/TMath::Sqrt(wave.V.size());
      wave.V[i] = TMath::Sqrt(valsReal[i]*valsReal[i]+valsComp[i]*valsComp[i]);

      //std::cout <<FFT.V[i] << " " << wave.V[i] << std::endl;
    }
    return retVal;   
  }
}

#endif
