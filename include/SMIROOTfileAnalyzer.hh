#ifndef __SMIROOTFILEANALYZER_HH__
#define __SMIROOTFILEANALYZER_HH__

/*! 
  \file SMIROOTfileAnalyzer.hh
  \brief basic analyzer class for ROOT files from the modified wavedump converter
  \author Clemens Sauerzopf
*/ 

#ifdef __HAVEROOT
#include"SMIWaveformAnalyzer.hh"
#include"chaintools.hh"

/*! 
  \file SMIROOTfileAnalyzer.hh
  \brief basic analyzer class for ROOT files from the modified wavedump converter
  \author Clemens Sauerzopf
*/ 

// ROOT includes
#include<TChain.h>

// stdlib includes
#include<string>
#include<fstream>
#include<map>

//! ROOT file analyzer for waveforms stored in TTrees
/*!
  This class implements the funcionalitly to read waveform data from
  a ROOT file and loop over a TTree to analyze the waveforms. The 
  structure of the TTree is supplied with a configuration file with the 
  following structure: \n
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  # a comment
  TreeName: T
  # start of the channel description
  # rootname trigger channelname
  C CH1 Trig1 IFES1
  C CH2 Trig1 IFES2
  C CH3 Trig2 IFES3
  # rootname triggername
  T TR1 Trig1
  T TR2 Trig2
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  this config file is now passed to the class and everything will be set up. 
  To do analysis with this class after setting up the data structure, call
  the @ref loop function, this function takes another function as an argument
  that does the real analysis work and is user supplied. This function can be
  a \a function \a pointer a \a lambda \a expression or a \a functor. The 
  only important thing is that the function takes a \a reference to a
  \ref SMIROOTfileAnalyzer object as argument, this way the analysis function 
  has access to all parameters of the classes in a thread safe way. Typical 
  function headers would look like this:
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cc}
  void someFunction(SMIROOTfileAnalyzer &wave){}       // function (pointer)
  auto someFunction = [](SMIROOTfileAnalyzer &wave){}; // lambda function
  class someFunction {                                 // functor
  public:
    void operator()(SMIROOTfileAnalyzer &wave){} 
  };
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
class SMIROOTfileAnalyzer : public SMIWaveformAnalyzer{
public:
  //! standard constructor
  SMIROOTfileAnalyzer();
  //! construct the object with a config file
  /*!
    @param configfile filename of the configuration file
   */
  SMIROOTfileAnalyzer(std::string configfile);

  //! read the configuration file
  /*!
    this function reads the configuration file and creates
    all the nescessary mappings between ROOT branch names and 
    the desired names of the channels and their trigger association.
    \warning  If no TreeName keyword is found in the config file a warning 
    that no TChain was creates is sent to std::cerr.

    @param configfile filename of the configuration file
    @return true if successful
   */
  bool readConfigFile(std::string configfile);

  //! opens the TBranches and sets up the connections
  /*!
    @return true if TChain is open 
   */
  bool associateTTreeBranches();

  //! add a ROOT file to the TChain
  inline bool addFile(std::string name) {
    if(chain != NULL){ chain->Add(name.c_str()); return true;}
    return false;
  }

  //! loop over the events in the TTree
  /*!
    this function accepts a starting event number, a number of events
    to process and a function to execute.

    @param start starting event number
    @param num number of events to process
    @param func function to execute (see description in @ref SMIROOTfileAnalyzer)
    @return true on success
   */

  virtual bool loop(size_t start, size_t num, processBase &func){
    size_t entries = chain->GetEntries();
    size_t stop = start+num;
    if(start > entries) return false;
    if(stop  > entries) stop = entries;

    for(size_t i=start; i<stop; i++){
      chain->GetEntry(i);
      copyConvertData();
      func(*this);
    }
    return true;
  }

private:
  //! copy temporary data to the waveforms and convert to double
  void copyConvertData();

  std::map<std::string, std::string> rootmapping; //!< mapping between ROOT names and "real" channel names

  // managed by the ROOT garbage collector
  std::vector<TBranch **>          branches; //!< vectro of TBranch adresses
  std::vector<std::vector<float> > temp;     //!< temporary storage for values
  TChain                          *chain;    //!< TChain pointer
};

#endif

#endif
