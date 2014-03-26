#include"SMIWaveformAnalyzerProcessSystem.hh"
#include"SMIWaveformAnalyzer.hh"
//plugins
#include"plugins/cleanWaveForm.hh"
#include"plugins/calculateGatedMean.hh"

#include<iostream>

processCalculateBaseline::processCalculateBaseline() : event(0){
  list.add(new plugin::cleanWaveForm     );
  list.add(new plugin::calculateGatedMean());
}

void processCalculateBaseline::operator()(SMIWaveformAnalyzer &wave){
  wave.processAll(list);
  char tmp[128];
  std::map<std::string,WaveForm>::iterator it;
  for(it = wave.channel.begin(); it!=wave.channel.end(); it++){
    sprintf(tmp,"Mean-%s-%i",it->first.c_str(),event);
    result[tmp] = it->second.result["BaseLineMean"];
    sprintf(tmp,"StdDev-%s-%i",it->first.c_str(),event);
    result[tmp] = it->second.result["BaseLineStdDev"];
  }
  for(it = wave.trigger.begin(); it!=wave.trigger.end(); it++){
    sprintf(tmp,"Mean-%s-%i",it->first.c_str(),event);
    result[tmp] = it->second.result["BaseLineMean"];
    sprintf(tmp,"StdDev-%s-%i",it->first.c_str(),event);
    result[tmp] = it->second.result["BaseLineStdDev"];
  }
  event++;
}

std::map<std::string, double> processCalculateBaseline::getBaseline(SMIWaveformAnalyzer &wave){
  std::map<std::string, double> val;

  std::map<std::string,WaveForm>::iterator it;

  for(it = wave.channel.begin(); it!=wave.channel.end(); it++){
    char tmpm[128];
    char tmps[128];
    double meanweight = 0.0;
    double sumweight  = 0.0;
    for(int i=0; i<event; i++){
      sprintf(tmpm,"Mean-%s-%i"  ,it->first.c_str(),i);
      sprintf(tmps,"StdDev-%s-%i",it->first.c_str(),i);
      double weight = 1/(result[tmps]*result[tmps]);
      meanweight += result[tmpm]*weight;
      sumweight  += weight;
    }
    val[it->first] = meanweight/sumweight;
  }

  for(it = wave.trigger.begin(); it!=wave.trigger.end(); it++){
    char tmpm[128];
    char tmps[128];
    double meanweight = 0.0;
    double sumweight  = 0.0;
    for(int i=0; i<event; i++){
      sprintf(tmpm,"Mean-%s-%i"  ,it->first.c_str(),i);
      sprintf(tmps,"StdDev-%s-%i",it->first.c_str(),i);
      double weight = 1/(result[tmps]*result[tmps]);
      meanweight += result[tmpm]*weight;
      sumweight  += weight;
    }
    val[it->first] = meanweight/sumweight;
  }

  return val;
    
}
