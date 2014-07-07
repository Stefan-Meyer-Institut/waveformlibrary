
#include"SMIROOTfileAnalyzer.hh"
#include"SMIMidasAnalyzer2012.hh"
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
  f.Open("run00001.mid");
  //f.Open("../test1/run00029.mid");
  SMIMidasAnalyzer2012 analyzer("midasconfigexample.conf");
  analyzer.setADCRange(4095);
  processOneBar demo;
  processCalculateBaseline test;
  while (1) {
    TMidasEvent event;
    if (!f.Read(&event))
        break;
    int eventId = event.GetEventId();
    if ((eventId & 0xFFFF) == 0x8000)
      {
	// begin run                                                                                                       
	event.Print();
      } else if ((eventId & 0xFFFF) == 0x8001){
      event.Print();
    } else {
      event.SetBankList();
      if (eventId == 1) { // FADC
	//event.Print();
	std::cout << "midas eventnumber: " << event.GetSerialNumber() << std::endl;
	//if( event.GetSerialNumber() >= 20) break;
	analyzer.prepareMidasEvent(event);
	
	//analyzer.loop(10,100,test); 
	//std::cout << analyzer << std::endl;
	//auto retval = test.getBaseline(analyzer);
	//demo.setBaseLine(retval);
	//analyzer.loop(10,100,demo);
	analyzer.loop(3,100,demo);
	analyzer.purgeEvents(100);
	std::cout << analyzer << std::endl;
      }
    } 
  }
  f.Close();
  demo.Print();
    //SMIROOTfileAnalyzer analyzer("examples/rootfile/test.cfg");
    //analyzer.addFile("scitil_2014jan_cosy_r077-000.root");


  

  /*  for(auto i : retval){
    std::cout << i.first  << " " << i.second << std::endl;
    }*/

  //someOtherFunction f;
  //analyzer.loop(0,10,f);
  return 0;
}
