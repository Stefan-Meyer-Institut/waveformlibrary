#ifndef __SMIWAVEFORMANALYZER_HH__
#define __SMIWAVEFORMANALYZER_HH__

/*! \file SMIWaveformAnalyzer.hh
    \brief base class for processing pluginslists
    \author Clemens Sauerzopf
*/ 

#include"SMIWaveform.hh"
#include"SMIWaveformAnalyzerPluginSystem.hh"
#include<vector>

//! base class for processing pluginslists
/*!
  The SMIWaveformAnalyzer class provides functions to process plugins
  in the form of a @ref SMIAnalyzerPluginList, these plugins can ba applied 
  to all magaged waveforms, to all channels, to all triggers or to single
  waveforms and triggers.
 */
class SMIWaveformAnalyzer : public SMIWaveform {
public:

  //! standard constructor, calls @ref SMIWaveform::SMIWaveform()
  SMIWaveformAnalyzer() : SMIWaveform::SMIWaveform() {}

  //! constructor with number of bins, calls baseclass constructor @ref SMIWaveform::SMIWaveform(size_t nEntries)
  SMIWaveformAnalyzer(size_t nEntries) : SMIWaveform::SMIWaveform(nEntries) {}
  //~SMIWaveformAnalyzer();

  //! apply the pluginslist to a single channel
  /*!
    this function applies the contents of a @ref SMIAnalyzerPluginList to a
    single channel name
    
    @param name of the channel to process
    @param plugins list of plugins to apply
    @return true if no error occured
  */
  bool processChannel(std::string name, SMIAnalyzerPluginList &plugins);
  
  //!  apply the pluginslist to a single trigger
  /*!
    this function applies the contents of a @ref SMIAnalyzerPluginList to a
    single trigger name
    
    @param name of the trigger to process
    @param plugins list of plugins to apply
    @return true if no error occured
  */
  bool processTrigger(std::string name, SMIAnalyzerPluginList &plugins);

  //! apply the pluginlist to all channels
  /*!
    this function applies the contents of a @ref SMIAnalyzerPluginList to 
    all channels
    
    @param plugins list of plugins to apply
    @return true if no error occured
   */
  bool processAllChannels(SMIAnalyzerPluginList &plugins);
  
  //! apply the pluginlist to all triggers
  /*!
    this function applies the contents of a @ref SMIAnalyzerPluginList to 
    all triggers

    @param plugins list of plugins to apply
    @return true if no error occured
   */
  bool processAllTriggers(SMIAnalyzerPluginList &plugins);
  
  //!  apply the pluginlist to all waveforms (channels and triggers)
  /*!
    this function applies the contents of a @ref SMIAnalyzerPluginList to 
    all waveforms (channels and triggers)

    @param plugins list of plugins to apply
    @return true if no error occured
   */
  bool processAll(SMIAnalyzerPluginList &plugins);

};
#endif
