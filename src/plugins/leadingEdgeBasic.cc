#include"plugins/leadingEdgeBasic.hh"
#include<cmath>
#include<algorithm>
#include<iostream>
namespace plugin {
  leadingEdgeBasic::leadingEdgeBasic(double low, double high, double thresh):
    lowGate(low), highGate(high), threshold(thresh) {

  }

  bool leadingEdgeBasic::operator()(WaveForm &wave){
    std::vector<double>::iterator element, start, end;
    if(wave.pulseType <= 0) { // neg pulse
      element = std::min_element(wave.V.begin(), wave.V.end());
    } else {
      element = std::max_element(wave.V.begin(), wave.V.end());
    }

    // store amplitude information
    wave.result["A"]      = *element;    

    // check against threshold
    if(std::abs(*element) < threshold || 
       wave.t[std::distance(wave.V.begin(), element)] < lowGate ||
       wave.t[std::distance(wave.V.begin(), element)] > highGate   ){
      // std::cout << std::abs(*element) << " " 
      // 		<<  wave.t[std::distance(wave.V.begin(), element)] << " "
      // 		<< lowGate << " " << highGate << std::endl;
      // std::cout <<"maxtime=" << wave.t[std::distance(wave.V.begin(), element)]
      // 		<< std::endl;
      wave.result["LEtime"] = -999999;
      wave.result["LEToT" ] = -999999;
      return false;
    }
    start = element;
    end = element;

    while(wave.t[std::distance(wave.V.begin(), start)] > lowGate && 
	  start != wave.V.begin()) {
      start--;
      // just below threshold
      if(std::abs(*start) > threshold) continue;
      else break;
    }
    if(std::abs(*start) > threshold){
      wave.result["LEtime"] = -999999;
      wave.result["LEToT" ] = -999999; 
      return false;
    }

    while(wave.t[std::distance(wave.V.begin(), end)] < highGate &&
	  end != wave.V.end()) {
      end++;
      // just  threshold
      if(std::abs(*end) > threshold) continue;
      else break;
 
    }
    // just above threshold for use with interpolation algorithm
    end--;
    
    wave.result["LEtime"] = getTime(wave, start);
    wave.result["LEToT" ] = getTime(wave, end) - wave.result["LEtime"];

    return true;
  }

  double leadingEdgeBasic::getTime(WaveForm &wave, 
				   std::vector<double>::iterator &it){
    return wave.t[std::distance(wave.V.begin(), it)];
  }

}
