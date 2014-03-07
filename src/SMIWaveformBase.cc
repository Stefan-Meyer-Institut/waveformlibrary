#include"SMIWaveformBase.hh"
#include<algorithm>

/**********************************************************/
/***** public functions ***********************************/
/**********************************************************/

bool SMIWaveformBase::renameChannel(std::string newName, std::string oldName){
  // channel
  std::map<std::string, WaveForm>::iterator search = channel.find(oldName);
  if(channel.find(newName) != channel.end()) return false;
  if(search == channel.end()) return false;
  
  channel[newName] = channel[oldName];
  channel.erase(search);

  // trigger to channel mapping
  std::replace(triggerchannel[channeltrigger[oldName]].begin(), 
	       triggerchannel[channeltrigger[oldName]].end(),
	       oldName, newName);

  // channel to trigger mapping
  std::map<std::string, std::string>::iterator searchct = channeltrigger.find(oldName);
  channeltrigger[newName] = channeltrigger[oldName];
  channeltrigger.erase(searchct);

  return true;
}

bool SMIWaveformBase::renameTrigger(std::string newName, std::string oldName){
  std::map<std::string, WaveForm>::iterator search = trigger.find(oldName);
  if(trigger.find(newName) != trigger.end()) return false;
  if(search == trigger.end()) return false;
  
  trigger[newName] = trigger[oldName];
  trigger.erase(search);

  // update channel to trigger mapping
  for(std::vector<std::string>::iterator it=triggerchannel[oldName].begin();
      it<triggerchannel[oldName].end(); it++) channeltrigger[*it] = newName;
  
  // update trigger to channel mapping
  std::map<std::string, std::vector<std::string> >::iterator searchtc = triggerchannel.find(oldName);
  triggerchannel[newName] = triggerchannel[oldName];
  triggerchannel.erase(searchtc);

  return true;
}

bool SMIWaveformBase::addChannelTrigger(std::string chName, std::string trgName){;
  std::map<std::string, std::string>::iterator searchct = channeltrigger.find(chName);
  std::map<std::string, std::vector<std::string> >::iterator searchtc = triggerchannel.find(trgName);
  if(searchct != channeltrigger.end()) return false;
  if(searchtc != triggerchannel.end()){
    if(std::find(searchtc->second.begin(), searchtc->second.end(), chName) != searchtc->second.end()) return false;
  } else {
    triggerchannel[trgName] = std::vector<std::string>();
  }

  addChannel(chName );
  addTrigger(trgName);

  channeltrigger[chName] = trgName;
  triggerchannel[trgName].push_back(chName);
  
  return true;
}

/**********************************************************/
/***** protected functions ********************************/
/**********************************************************/
bool SMIWaveformBase::addChannel(std::string Name){
  if(testChannel(Name)) return false;
  
  channel[Name] = WaveForm();
  if(nEntries != 1024) channel[Name].resizeWave(nEntries);
  return true;
}

bool SMIWaveformBase::addTrigger(std::string Name){
  if(testTrigger(Name)) return false;

  trigger[Name] = WaveForm();
  if(nEntries != 1024) trigger[Name].resizeWave(nEntries);
  return true;
}

bool SMIWaveformBase::testChannel(std::string Name) {
  std::map<std::string, WaveForm>::iterator search = channel.find(Name);
  if(search != channel.end()) return true;
  else return false;
}

bool SMIWaveformBase::testTrigger(std::string Name) {
std::map<std::string, WaveForm>::iterator search = trigger.find(Name);
  if(search != trigger.end()) return true;
  else return false;
}

/**********************************************************/
/***** operator functions *********************************/
/**********************************************************/

std::ostream& operator<< (std::ostream &out, SMIWaveformBase &Wave){
  out << "# Trigger to channel mapping" << std::endl;
  std::map<std::string, std::vector<std::string> >::iterator it;
  for(it=Wave.triggerchannel.begin(); it != Wave.triggerchannel.end(); it++){
    out << it->first << " ";
    for(size_t i=0; i<it->second.size(); i++)
      out << it->second[i] << " ";
    out << std::endl;
  }
  out << "# ChannelName";
  std::map<std::string, WaveForm>::iterator ch = Wave.channel.begin();
  std::map<std::string, double>::iterator it2 = ch->second.result.begin();
  for(;it2!=ch->second.result.end();it2++)
    out << it2->first << " ";
  out << "isDigital" << std::endl;

  for(;ch!=Wave.channel.end();ch++){
    out << ch->second << std::endl;
  }

  for(ch = Wave.trigger.begin(); ch!= Wave.trigger.end(); ch++)
    out << ch->second << std::endl;

  return out;
}
