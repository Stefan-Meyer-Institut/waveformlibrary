#ifndef __SMIWAVEFORMBASE_HH__     
#define __SMIWAVEFORMBASE_HH__     

/*! \file SMIWaveformBase.hh
    \brief base class for the waveform analyzer
*/ 

#include<vector>
#include<string>
#include<map>
#include<cstdio>
#include<ostream>

#include"SMIWave.hh"

//! base class for the waveform analyzer
/*!
  this base class defines the basic functionality like adding
  channels and triggers and renaming them.
 */
class SMIWaveformBase {
public: 
//! constructor
/*!
  standard constrictor, initilazies the signal pattern, the event 
  number and the time stamp to 0 and the number of bins in one 
  waveform to 1024
 */
  SMIWaveformBase() : signalPattern(0), eventnum(0), timestamp(0), 
		      nEntries(1024) {}

//! renames a channel
/*!
  this functions renames a channel with name oldName to newName
  and updates the channel to trigger mappings

  @param newName the new name of the channel
  @param oldName the current name of the channel
  @return true is oldName exists otherwise false
 */
  bool renameChannel    (std::string newName, std::string oldName);

//! renames a trigger 
/*!
  this functions renames a trigger with name oldName to newName
  and updates the channel to trigger mappings

  @param newName the new name of the trigger
  @param oldName the current name of the trigger
  @return true is oldName exists otherwise false
 */
  bool renameTrigger    (std::string newName, std::string oldName);

//! adds a channel and a trigger
/*!
  this function creates a channel/trigger pair in ll relevant databases
  also a new channel is created and a new trigger if the trigger name
  is not found in the database

  @param channel name of the channel to add
  @param trigger of the trigger for this channel
  @return false id channel already exists, true otherwise
 */
  bool addChannelTrigger(std::string channel, std::string trigger);

  //! return the eventnumber from the waveform digitiser
  inline int getEventNum()    const {return eventnum; }
  
  //! return the trigger time tag from the waveform digitiser
  inline int getTriggerTime() const {return timestamp;}


//! simple output operator
/*!
  this operator allows to dump a complete event including all results 
  ta stdout our a file
 */
  friend std::ostream& operator<< (std::ostream &out, SMIWaveformBase &Wave);


  std::map<std::string, WaveForm>                  channel;        //!< channel database
  std::map<std::string, WaveForm>                  trigger;        //!< trigger database
  std::map<std::string, std::string>               channeltrigger; //!< mapping of channels to triggers
  std::map<std::string, std::vector<std::string> > triggerchannel; //!< mapping of triggers to channels

//! returns a reference to a Waveform with Name
/*!
  use with care!

  @param Name of the waveform (channel or trigger)
  @return reference to the waveform
 */
  WaveForm& getWaveForm(std::string Name);

protected:
//! adds a channel to the database (don't use, use @ref addChannelTrigger instead)
  bool addChannel(std::string Name);

//! adds a trigger to the database (don't use, use @ref addChannelTrigger instead)
  bool addTrigger(std::string Name);

//! tests if a channel exists
/*!
  @param Name of the channel
  @return true is the name is found in the database otherwise false
 */
  bool testChannel(std::string Name);
//! tests if a trigger exists
/*!
  @param Name of the trigger
  @return true is the name is found in the database otherwise false
 */
  bool testTrigger(std::string Name);


  unsigned int signalPattern; //!< signal pattern of the V1742 waveform digitiser
  int          eventnum;      //!< eventnumber   
  unsigned int timestamp;     //!< event time stamp

  size_t       nEntries;      //!< number of bins per waveform
};

#endif
