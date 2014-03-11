#ifndef __SMIWAVEFORMANALYZER_HH__
#define __SMIWAVEFORMANALYZER_HH__

#include"SMIWaveform.hh"
#include"SMIWaveformAnalyzerPluginSystem.hh"
#include<vector>


class SMIWaveformAnalyzer : public SMIWaveform {
public:
  SMIWaveformAnalyzer() : SMIWaveform::SMIWaveform() {}
  SMIWaveformAnalyzer(size_t nEntries) : SMIWaveform::SMIWaveform(nEntries) {}
  //~SMIWaveformAnalyzer();

  bool processChannel(std::string name, SMIAnalyzerPluginList &plugins);
  bool processTrigger(std::string name, SMIAnalyzerPluginList &plugins);

  bool processAllChannels(SMIAnalyzerPluginList &plugins);
  bool processAllTriggers(SMIAnalyzerPluginList &plugins);

  bool processAll(SMIAnalyzerPluginList &plugins);

};
#endif
