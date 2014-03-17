#ifndef __SMIWAVETESTS_HH__
#define __SMIWAVETESTS_HH__

/*! 
  \file SMIWaveTests.hh
  \brief this file contains headers for unit tests
  \author Clemens Sauerzopf
*/ 

//! unit test class
class SMIWaveTests {
public:
  //! test all modules
  bool test();

  //! test functinality of SMIWave class
  bool testSMIWave();

  //! test functinality of SMIWaveformBase class
  bool testSMIWaveformBase();

  //! test functinality of SMIWaveform class
  bool testSMIWaveform();

  //! test functinality of SMIWaveformAnalyzer class
  bool testSMIWaveformAnalyzer();
};

#endif
