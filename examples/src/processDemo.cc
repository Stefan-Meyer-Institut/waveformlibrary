#include"examples/include/processDemo.hh"
#include"include/SMIWaveformAnalyzer.hh"
//plugins
#include"plugins/cleanWaveForm.hh"
#include"plugins/changeADCRange.hh"
#include"plugins/leadingEdgeLinear.hh"
#include"plugins/leadingEdgeBasic.hh"
#include"plugins/calculateCharge.hh"

//stl
#include<iostream>

processDemo::processDemo() : event(0) {
  list.add(new plugin::cleanWaveForm          );
  list.add(new plugin::changeADCRange(1/4095.));
  list.add(new plugin::leadingEdgeLinear(0,200,0.04)   );
  list.add(new plugin::calculateCharge(0,200)      );
}

void processDemo::operator()(SMIWaveformAnalyzer &wave){
  subtractBaseLine(wave);
  wave.processAllChannels(list);


  printEvent(wave);

  // std::cout << "Event: " << event << std::endl;
  // std::cout << "-----------------------------------------------" << std::endl;
  // std::cout << wave;
  // std::cout << "-----------------------------------------------" << std::endl;

  event++;
}

void processDemo::subtractBaseLine(SMIWaveformAnalyzer &wave){
  std::map<std::string, double>::iterator it = baseline.begin();
  while(it!=baseline.end()){
    wave.getWaveForm(it->first) -= it->second;
    it++;
  }
}

void processDemo::printEvent(SMIWaveformAnalyzer &wave){
  for(int i=0; i<1024; i++){
    for(auto it : wave.channel){
      std::cout << it.second.V[i] << " ";
    }
    std::cout << std::endl;
  }
}
