#include"SMIROOTfileAnalyzer.hh"
#include"SMIMidasAnalyzer2014.hh"
#include"SMIWaveformAnalyzerPluginSystem.hh"
#include"SMIWaveformAnalyzerProcessSystem.hh"

#include"examples/include/processonebar.hh"

//plugins
#include"plugins/changeADCRange.hh"

#include"SMIWaveformAnalyzerProcessSystem.hh"

// STL 
#include<iostream>

// midas
#include <TMidasFile.h>
#include <TMidasEvent.h>

int main(int argc, char *argv[]){

  TMidasFile f;
  f.Open("run00061.mid");
  //f.Open("../test1/run00029.mid");
  SMIMidasAnalyzer2014 analyzer("midasconfigexample.conf");
  analyzer.setADCRange(4095);
  processOneBar demo;
  while (1) {
    TMidasEvent event;
    if (!f.Read(&event))
        break;
    int eventId = event.GetEventId();
    if ((eventId & 0xFFFF) == 0x8000)
      {
	// begin run                                                                                                       
	//event.Print();
      } else if ((eventId & 0xFFFF) == 0x8001){
      //event.Print();
    } else {
      event.SetBankList();
      if (eventId == 10) { // clock trigger data
	if(!analyzer.prepareMidasEvent(event)) continue;
	analyzer.loop(demo); 
	std::cout << analyzer << std::endl;
      }
      if (eventId == 20) { // correction data
	//event.Print();
	//std::cout << "midas eventnumber: " << event.GetSerialNumber() << std::endl;
	if( event.GetSerialNumber() >= 1) continue;
	analyzer.prepareCorrectionTables(event);
	if(!analyzer.prepareMidasEvent(event)) continue;
	
	analyzer.loop(demo); 
	
      }
    } 
  }
  f.Close();
  //demo.Print();
  return 0;
}
