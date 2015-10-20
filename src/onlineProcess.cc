#include"include/onlineProcess.hh"

#include"SMIWaveformAnalyzer.hh"

//plugins
#include"plugins/cleanWaveForm.hh"
#include"plugins/changeADCRange.hh"
#include"plugins/leadingEdgeLinear.hh"
#include"plugins/leadingEdgeBasic.hh"
#include"plugins/calculateCharge.hh"
#include"plugins/constantFractionLinear.hh"
#include"plugins/subtractLinearBaseLine.hh"
#include"plugins/FFTFilterWaveForm.hh"
#include"plugins/FFTLowPassFilter.hh"
#include"plugins/templateFit.hh"
#include"plugins/manyCFLinear.hh"
#include"plugins/detectPulseType.hh"

//stl
#include<iostream> 
#include<fstream>   
#include<map> 
#include<string>
#include<stdlib.h>
onlineProcess::onlineProcess(std::string outName) : event(0) {
  CFnum = 10;

  plugin::FFTFilterWaveForm* fft = new plugin::FFTFilterWaveForm(1024);
  fft->add(new plugin::FFT::LowPassFilter(205)); 

  plugin::manyCFLinear *cfs = new plugin::manyCFLinear(1,4.5,CFnum);

  CFNames = cfs->names;

  //list.add(new plugin::cleanWaveForm          );
  list.add(new plugin::changeADCRange        (1/4095.)   ); 
  list.add(fft     ); 
  list.add(new plugin::subtractLinearBaseLine()          ); 
  list.add(new plugin::detectPulseType());
  list.add(new plugin::leadingEdgeLinear     (0,400,0.05));
  
  //list.add(new plugin::templateFit             ("./onlineDataConfig/waveformTemplate.dat",0,150)     );
  list.add(new plugin::calculateCharge       (0,400)     );
  list.add(cfs);

  listTr.add(new plugin::changeADCRange        (1/4095.)   );  
  listTr.add(new plugin::subtractLinearBaseLine()          );
  listTr.add(new plugin::leadingEdgeLinear     (0,200,0.1));
  listTr.add(new plugin::constantFractionLinear()          );


  CF = new double*[CFnum];
  for(size_t i=0; i<CFnum; i++){
    CF[i] = new double[32];
  }

  output = new TFile(outName.c_str(), "RECREATE");
  tree   = new TTree("T_LE",   "a TTree holding cosy data with LE and CF");
  tree->Branch("LE",     LE,     "LE[32]/D"     );
  
  for (size_t i=0; i<CFnum; i++){
    char t[128];
    char n[128];
    sprintf(t,"%s[32]/D",CFNames[i].c_str());
    t[2] = 'm';
    sprintf(n,"%s",CFNames[i].c_str());
    n[2] = 'm';
    tree->Branch(n, CF[i], t);
  }

  tree->Branch("LEToT",  LEToT,  "LEToT[32]/D"  );
  tree->Branch("A",      A,      "A[32]/D"      );
  tree->Branch("Charge", Charge, "Charge[32]/D" );

  tree->SetAutoSave(200);
}

void onlineProcess::operator()(SMIWaveformAnalyzer &wave){
  //subtractBaseLine(wave);
  //printWaveforms(wave);
  
  // process all triggers for timing correction
  wave.processAllTriggers(listTr);
  
  addToAllChannels(wave,200); // add 200 ns
  subtractTriggerOffset(wave);
  
  wave.processAllChannels(list);

  //std::cout << wave << std::endl;
  /*std::string chs[13] = {"SCITILTL","SCITILTR","SCITILBR","SCITILBL", "SCI1", 
			 "SCI2", "SCI1Analogue", "SCI2Analogue", "IFES1", 
			 "IFES2", "IFES3", "IFES4", "SCI3"};*/
  /*std::string chs[14] = {"SCITILTL","SCITILTR","SCITILBR","SCITILBL","IFES1", 
			 "IFES2", "IFES3", "IFES4", "SCI1l", "SCI1r", "SCI2l",
			 "SCI2r", "SCI3l", "SCI3r"};*/

  

  for(unsigned int i=0; i<32; i++){
    char tmp[20];
    sprintf(tmp,"CH-%u",i);
    LE[i]     = wave.channel[tmp].result["LEtime"];
    LEToT[i]  = wave.channel[tmp].result["LEToT"];
    for (size_t j=0; j<CFnum; j++){
    CF[j][i]     = wave.channel[tmp].result[CFNames[j]];
    }
    
    Charge[i] = wave.channel[tmp].result["Q"];
    A[i]      = wave.channel[tmp].result["A"];
  }
  tree->Fill();
  if(event%100 == 0){
    //output->cd();
    //tree->Write();
    //output->cd();
    //tree->AutoSave();
    std::cout << "EVENT: " << event << std::endl;
    std::cout << wave << std::endl;
  }
  event++;

  //  if(event > 100) exit(0);
}


void onlineProcess::addToAllChannels(SMIWaveformAnalyzer &wave, double time){
  std::map<std::string, WaveForm>::iterator it;
  // shift all channels by 200 ns
  for(it=wave.channel.begin(); it!=wave.channel.end(); it++){
    //std::cout << wave.channel[it->first].t[0] << std::endl;
    wave.channel[it->first].shiftTime(time);
    //std::cout << wave.channel[it->first].t[0] << std::endl;
  }
}

void onlineProcess::subtractTriggerOffset(SMIWaveformAnalyzer &wave){
  std::map<std::string, WaveForm>::iterator it;
  for(it=wave.trigger.begin(); it!=wave.trigger.end(); it++){
    if(it->second.result["LEtime"] > 0 ){
      std::vector<std::string> &mapping = wave.triggerchannel[it->first];
      for(size_t i=0; i<mapping.size(); i++){
	wave.channel[mapping[i]].shiftTime(-(it->second.result["LEtime"]));
      }
    }
  }
}

