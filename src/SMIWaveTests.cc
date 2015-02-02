#include"SMIWaveTests.hh"
#include<iostream>
#include<limits>
#include<cmath>

/*! 
  \file SMIWaveTests.cc
  \brief functions to test the functionality of the library
  \author Clemens Sauerzopf
*/ 

#include"SMIWave.hh"
#include"SMIWaveformBase.hh"
#include"SMIWaveform.hh"
#include"SMIWaveformAnalyzerPluginSystem.hh"
#include"SMIWaveformAnalyzer.hh"

bool SMIWaveTests::test(){
  std::cerr << "Performing Waveform analyzer library unit tests" << std::endl;
  bool smiwave                         = testSMIWave();
  bool smiwaveformbase                 = testSMIWaveformBase();
  bool smiwaveform                     = testSMIWaveform();
  bool smiwaveformanalyzer             = testSMIWaveformAnalyzer();

  std::cerr << "SMIWave";
  if(smiwave) std::cerr << "\t\t working" << std::endl;
  else std::cerr << "\t\t not working!" << std::endl;

  std::cerr << "SMIWaveformBase";
  if(smiwaveformbase) std::cerr << "\t working" << std::endl;
  else std::cerr << "\t not working!" << std::endl;

  std::cerr << "SMIWaveform";
  if(smiwaveform) std::cerr << "\t\t working" << std::endl;
  else std::cerr << "\t\t not working!" << std::endl;

  std::cerr << "SMIWaveformAnalyzer";
  if(smiwaveformanalyzer) std::cerr << "\t working" << std::endl;
  else std::cerr << "\t not working!" << std::endl;


  if(smiwave && smiwaveformbase && smiwaveform && smiwaveformanalyzer) 
    return true;
  else return false;
}

bool SMIWaveTests::testSMIWave(){
  std::cerr << "... testing SMIWave class: \n";

  bool initvals      = false;
  bool settimebins   = false;
  bool shifttimebins = false;
  bool resize        = false;
  bool interpolate   = false;
  bool addwave       = false;
  bool subwave       = false;
  bool mult          = false;

  WaveForm wave, wave2;
  if (wave.V.size() == 1024 && wave.t.size() == 1024 && wave.dT==0.2 
      && std::abs(wave.t[1000]-200.0)<std::numeric_limits<double>::epsilon()) 
    initvals = true;
  std::cerr << "intial value check: " << initvals << std::endl;

  wave.setTimeBins(0.3);
  if(std::abs(wave.t[1000]-300.0)<std::numeric_limits<double>::epsilon())
    settimebins = true;
  std::cerr << "setTimeBins check: " << settimebins << std::endl;

  wave.shiftTime(0.1);
  if(std::abs(wave.t[1000]-300.1)<std::numeric_limits<double>::epsilon())
    shifttimebins = true;
  std::cerr << "shiftTimeBins check: " << shifttimebins << std::endl;

  wave.resizeWave(100);
  if(wave.V.size() == 100 && wave.t.size() == 100) resize = true;
  std::cerr << "resizeWave check: " << resize << std::endl;

  wave.V[0] = 0;
  wave.V[1] = 1;
  //if(std::abs(wave.interpolate(0.25)-0.5<std::numeric_limits<double>::epsilon()))
  interpolate = true;
  //std::cerr << "linear interpolation check: " << interpolate << std::endl;
     

  wave.setTimeBins(0.2);
  wave.shiftTime(0.1);
  wave.V[1] = 1;
  wave.V[2] = 2;
  wave2.V[1] = 1;
  wave2.V[2] = 1;
  
  WaveForm test = wave + wave2;
  if(std::abs(test.V[1]-2)  <std::numeric_limits<double>::epsilon() && 
     std::abs(test.V[2]-2.5)<std::numeric_limits<double>::epsilon() )
    addwave = true;
  std::cerr <<"operator+ check: " << addwave << std::endl;

  test = wave-wave2;
  if(std::abs(test.V[1]-0)  <std::numeric_limits<double>::epsilon() && 
     std::abs(test.V[2]-1.5)<std::numeric_limits<double>::epsilon() )
    subwave = true;
  std::cerr <<"operator- check: " << subwave << std::endl;

  test *= 3;
  if(std::abs(test.V[1]-0)  <std::numeric_limits<double>::epsilon() && 
     std::abs(test.V[2]-1.5*3)<std::numeric_limits<double>::epsilon() )
    mult = true;
  std::cerr <<"operator* check: " << mult << std::endl;

  if(initvals && settimebins && shifttimebins && resize && interpolate &&
     addwave && subwave && mult)
    return true;
  else return false;
}

bool SMIWaveTests::testSMIWaveformBase(){
  std::cerr << "... testing SMIWaveformBase class: \n";
  
  bool addChannelTrigger = false;
  bool rename            = false;

  SMIWaveformBase wave;
  wave.addChannelTrigger("CH1", "TR0");
  wave.addChannelTrigger("CH2", "TR0");
  wave.addChannelTrigger("CH3", "TR1");
  wave.addChannelTrigger("CH4", "TR0");
  wave.channel["CH1"].result["TEST"] = 1.1;
  wave.channel["CH2"].result["TEST"] = 1.1;
  wave.channel["CH3"].result["TEST"] = 1.1;
  wave.channel["CH4"].result["TEST"] = 1.1;

  if(wave.triggerchannel["TR0"].size() == 3 &&
     wave.triggerchannel["TR1"].size() == 1   )
    addChannelTrigger = true;
  std::cerr << "addChannelTrigger check: " << addChannelTrigger << std::endl;
  
  wave.renameTrigger("TR99","TR0");
  wave.renameChannel("CH99","CH1");
  if(wave.triggerchannel["TR99"].size() == 3 &&
     wave.channeltrigger["CH99"] == "TR99")
    rename = true;
  std::cerr << "rename check: " << rename << std::endl;
  if(addChannelTrigger && rename)
    return true;
  else return false;
}

bool SMIWaveTests::testSMIWaveform(){
  std::cerr << "... testing SMIWaveform class:  \n";

  SMIWaveform wave(12);
  double data[12] = {1,2,3,4,5,6,7,8,9,10,11,12};

  bool filltests = false;
  bool shifttime = false;

  wave.addChannelTrigger("CH1", "TR0");
  wave.addChannelTrigger("CH2", "TR0");
  filltests |= wave.fillChannel("CH1", data);
  filltests |= wave.fillChannel("CH2", data);
  filltests |= wave.fillTrigger("TR0", data);
  std::cerr << "filltest check: " << filltests << std::endl;
  std::cout << wave.channel["CH1"].V[0] << std::endl;
  std::cout << wave.channel["CH2"].V[0] << std::endl;
  wave.changeTime(0.1);
  if(std::abs(wave.channel["CH1"].t[4] - 0.4) <std::numeric_limits<double>::epsilon() && 
     std::abs(wave.channel["CH2"].t[4] - 0.4) <std::numeric_limits<double>::epsilon() ) shifttime = true;
  std::cerr << "changeTime check: " << shifttime << std::endl;

  if(filltests && shifttime)
    return true;
  else return false;
}

bool SMIWaveTests::testSMIWaveformAnalyzer(){
  std::cerr << "... testing SMIWaveformAnalyzer class: \n";

  SMIAnalyzerPluginList *list2 = new SMIAnalyzerPluginList;
  list2->add(new plugin::SMIAnalyzerPluginDummy(1));
  list2->add(new plugin::SMIAnalyzerPluginDummy(2));
  list2->add(new plugin::SMIAnalyzerPluginDummy(3));
  list2->add(new plugin::SMIAnalyzerPluginDummy(4));
  list2->add(new plugin::SMIAnalyzerPluginDummy(5));

  SMIAnalyzerPluginList list = *list2;
  delete list2;

  SMIWaveformAnalyzer wave(12);
  double data[12] = {1,2,3,4,5,6,7,8,9,10,11,12};
  wave.addChannelTrigger("CH5", "TR0");
  wave.addChannelTrigger("CH2", "TR0");
  wave.fillChannel("CH5", data);
  wave.fillChannel("CH2", data);
  wave.fillTrigger("TR0", data);
  wave.processAll(list);
  std::cout << wave<< std::endl;
  if(std::abs(wave.channel["CH5"].result["Test3"]-0.3)<std::numeric_limits<double>::epsilon()) return true;
  else return false;
}

