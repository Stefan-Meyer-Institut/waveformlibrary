#include"plugins/templateFit.hh"

#include<TMath.h>

#include<fstream>
#include<cstring>
#include<iostream>

#ifdef __HAVEROOT
namespace plugin {


  bool templateFit::operator()(WaveForm &wave){
    currentData = &wave;

    //std::cout << "Maximum - Minimum: " << *std::max_element(currentData->V.begin(),currentData->V.end())-*std::min_element(currentData->V.begin(),currentData->V.end()) << std::endl;
    // find starting values
    Double_t par[3] = {0,0,0};
    guessStartingParameters(par,wave);
    // std::cout << "start parameters: " << par[0] << " " 
    // 	      << par[1] << " "  << par[2] << " " << std::endl; 
    // fitData
    bool converged = fitData  (par);
    // std::cout << "fit parameters: " << par[0] << " " 
    // 	      << par[1] << " "  << par[2] << " " << std::endl; 

    // perform F-Test
    //bool fTest     = makeFTest(par);
    bool rTest     = makeGoodnessOfFit(par);

    //if(converged) std::cout << "FIT CONVERGED" << std::endl;
    //if(rTest)     std::cout << "R-TEST PASSED" << std::endl;
    bool retVal    = produceOutput(converged, rTest, par, wave);
   

    currentData = NULL;
    return retVal;
  }


  // protected functions
  //////////////////////

  // par[0] time offset
  // par[1] amplitude
  // par[2] baseline
  double templateFit::errorFunction(const Double_t *par){
    if(currentData == NULL) return -1;
    if(calcLowBound)
      fitLowBound = currentData->t[0];

    WaveForm tmp = templFunc*par[1] + par[2];
    tmp.shiftTime(par[0]);
    tmp = (*currentData - tmp);
    
    double retVal = 0;
    for(size_t i=0; i<tmp.V.size(); i++){
      if(tmp.t[i] < fitLowBound || tmp.t[i] > fitHighBound) continue;
      retVal += tmp.V[i]*tmp.V[i];
    }
    return retVal;
  }

  // private functions
  ////////////////////

  bool templateFit::readWaveformTemplate(std::string &templatePath){
    std::ifstream ifile(templatePath.c_str());

    if(!ifile) return false;

    for(int i=0; i<1024; i++){
      if(ifile.eof()) return false;
      ifile >> templFunc.t[i] >> templFunc.V[i];
    }
    return true;
  }

  void templateFit::guessStartingParameters(Double_t *par, WaveForm &wave){
    // guess baseline
    for(int i=0; i<20; i++)
      par[2] += wave.V[i];
    par[2] /= 20;

    //guess amplitude
    std::vector<double>::iterator element;
    if(wave.pulseType <= 0) { // neg pulse
      element = std::min_element(wave.V.begin(), wave.V.end());
    } else {
      element = std::max_element(wave.V.begin(), wave.V.end());
    }
    par[1] = *element;    

    // guess time
    par[0] = wave.t[std::distance(wave.V.begin(), element)] - 6.5;
  }

  bool templateFit::fitData(Double_t *par){
    if(currentData == NULL) return false;
    // check if result is in range
    if(calcLowBound)
      fitLowBound = currentData->t[0];
    
    if(par[0]  < fitLowBound || par[0]  > fitHighBound) return false;
    if( *std::max_element(currentData->V.begin(),currentData->V.end())-*std::min_element(currentData->V.begin(),currentData->V.end()) < 0.01) return false;

    static Double_t step[7] = {0.001, 0.01, 0.01};

    /*std::cout << "START:" << std::endl
	      << "tOFF: " << par[0] << std::endl
	      << "ampl: " << par[1] << std::endl
	      << "base: " << par[2] << std::endl;*/

    minimiser.SetVariable(0, "tOff",      par[0], step[0]);
    minimiser.SetVariable(1, "amplitude", par[1], step[1]);
    minimiser.SetVariable(2, "baseline",  par[2], step[2]);

    minimiser.SetPrintLevel(0);
    bool retVal  = minimiser.Minimize();
    Int_t status = minimiser.Status();
    // std::cout << "STATUS: "<< status << std::endl; 
    if(!retVal || status==1) retVal = true; // status 1 means errors are not relyable 

    const Double_t* ret = minimiser.X();

    /*std::cout << "END:" << std::endl
	      << "tOFF: " << ret[0] << std::endl
	      << "ampl: " << ret[1] << std::endl
	      << "base: " << ret[2] << std::endl;
    */
    if (retVal)
      std::memcpy(par, ret, 3*sizeof(Double_t));
    return retVal;
  }

  bool templateFit::makeGoodnessOfFit(Double_t *par){
    if(currentData == NULL) return false;
    // check if result is in range
    if(calcLowBound)
      fitLowBound = currentData->t[0];
    
    if(par[0]  < fitLowBound || par[0]  > fitHighBound) return false;
   
    double SSres  = 0;
    double totErr = 0;

    WaveForm fit = templFunc*par[1] + par[2];
    fit.shiftTime(par[0]);
    fit = *currentData-fit;

    size_t size = 0;
    double mean     = 0;
    for(size_t i=0; i<currentData->V.size(); i++){
      if(currentData->t[i] < fitLowBound || currentData->t[i]  > fitHighBound) continue;
      size++;
      mean += currentData->V[i];
    }
    mean /= size;

    for(size_t i=0; i<currentData->V.size(); i++){
      //if(currentData->t[i] < fitLowBound || currentData->t[i]  > fitHighBound) continue;
      size++;
      SSres  += fit.V[i]*fit.V[i];
      totErr += (currentData->V[i]-mean)*(currentData->V[i]-mean);
    }

    //std::cout << SSres << " " << totErr << std::endl;

    double Rsquare = 1 - SSres/totErr;
    double S       = std::sqrt(SSres/(size-1));

    
    //std::cout << Rsquare << " " << S << " " << par[1] << " ";
    if (Rsquare > 0.5 && S < 0.1 && par[1]>0.01){
      //std::cout << "TRUE" << std::endl;
      return true;
    }
    else {
      //std::cout << "FALSE" << std::endl;
      return false;    
    }
  }

  bool templateFit::makeFTest(Double_t *par){
    if(currentData == NULL) return false;
    // check if result is in range
    if(calcLowBound)
      fitLowBound = currentData->t[0];

    if(par[0]  < fitLowBound || par[0]  > fitHighBound) return false;
    
    // just a simple line
    double chi2line = 0;
    double mean     = 0;
    int size = 0;
    for(size_t i=0; i<currentData->V.size(); i++){
      if(currentData->t[i] < fitLowBound || currentData->t[i]  > fitHighBound) continue;
      size++;
      mean += currentData->V[i];
    }
    mean /= size;;
    for(size_t i=0; i<currentData->V.size(); i++){
      if(currentData->t[i] < fitLowBound || currentData->t[i]  > fitHighBound) continue;
      chi2line += (currentData->V[i]-mean)*(currentData->V[i]-mean);
    }

    // fitted data
    double chi2fit = 0;
    WaveForm fit = templFunc*par[1] + par[2];
    fit.shiftTime(par[0]);
    fit = (*currentData - fit);
    for(size_t i=0; i<fit.V.size(); i++){
      if(fit.t[i] < fitLowBound ||  fit.t[i]  > fitHighBound) continue;
      chi2fit += fit.V[i]*fit.V[i];
    }

    // performing the F-Test
    if(chi2fit > chi2line) return false;
    double F = ( (chi2line-chi2fit)/2 ) / ( (chi2fit)/(1024-3) );
    double p = TMath::FDistI(F,2,1024-3);
    std::cout << chi2line << " " << chi2fit << std::endl;
    std::cout << "F-VALUE: " << F << std::endl; 
    std::cout << "P-VALUE: " << p << std::endl; 
    // probability to falsely reject is 0.5%, 
    if(p>0.005) return true;
    else        return false;
  }

  bool templateFit::produceOutput(bool converged, bool fTest, const Double_t *par, WaveForm &wave){
    if(converged){
      if(fTest){
	// intermediate storage for old results
	std::map<std::string, double> resultOld = wave.result;

	// replace measured signal with theoretical one with baseline set to 0
	wave = templFunc*par[1];
	wave.shiftTime(par[0]);
	// restore old results
	wave.result           = resultOld;
	
	// store Amplitude and fitted time stamp (start of rising edge)
	wave.result["LEtime"] = par[0];
	wave.result["A"]      = par[1];
	return true;
      }
    }

    wave.result["LEtime"] = -999999;
    wave.result["A"]      = par[1];
    return false;
  }
}
#endif
