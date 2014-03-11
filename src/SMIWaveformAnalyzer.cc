#include"SMIWaveformAnalyzer.hh"

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

bool SMIWaveformAnalyzer::processAllChannels(SMIAnalyzerPluginList &plugins){
  std::map<std::string, WaveForm>::iterator it;
  bool ret = true;
  for(it=channel.begin(); it!=channel.end(); it++){
    if(!processChannel(it->first, plugins)) ret = false;
  }
  return ret;
}

bool SMIWaveformAnalyzer::processAllTriggers(SMIAnalyzerPluginList &plugins){
 std::map<std::string, WaveForm>::iterator it;
  bool ret = true;
  for(it=trigger.begin(); it!=trigger.end(); it++){
    if(!processTrigger(it->first, plugins)) ret = false;
  }
  return ret;
}

bool SMIWaveformAnalyzer::processAll(SMIAnalyzerPluginList &plugins){
  bool ret = true;
  if(!processAllChannels(plugins)) ret = false;
  if(!processAllTriggers(plugins)) ret = false;
  return ret;
}
