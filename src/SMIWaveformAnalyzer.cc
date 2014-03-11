#include"SMIWaveformAnalyzer.hh"

SMIWaveformAnalyzer::SMIWaveformAnalyzer(){
}

SMIWaveformAnalyzer::~SMIWaveformAnalyzer(){
}

bool SMIWaveformAnalyzer::processChannel(std::string name, 
					 SMIAnalyzerPluginList &plugins) {
  bool ret = true;
  if(!testChannel(name)) return false;
  for(size_t i=0; i<plugins.size(); i++){
    if(plugins[i] != NULL){
      if(!(*plugins[i])(channel[name])) ret = false;
    }
  }
  return ret;  
}

bool SMIWaveformAnalyzer::processTrigger(std::string name, 
					 SMIAnalyzerPluginList &plugins) {
  bool ret = true;
  if(!testTrigger(name)) return false;
  for(size_t i=0; i<plugins.size(); i++){
    if(plugins[i] != NULL){
      if(!(*plugins[i])(trigger[name])) ret = false;
    }
  }
  return ret;  
}
