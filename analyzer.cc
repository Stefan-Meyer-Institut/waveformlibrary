
#include"SMIROOTfileAnalyzer.hh"
#include"SMIWaveformAnalyzerPluginSystem.hh"
#include"SMIWaveformAnalyzerProcessSystem.hh"

#include"examples/include/processDemo.hh"

//plugins
#include"plugins/changeADCRange.hh"

#include"SMIWaveformAnalyzerProcessSystem.hh"

// STL 
#include<iostream>

void someFunction(SMIROOTfileAnalyzer &wave){
  static int i=0; 
  std::cout << i++ <<" " << wave.channel["IFES2"].V[11] << " " <<
    wave.channel["IFES2"].t[11] << " " << wave.getEventNum() <<
    " " << wave.getTriggerTime() << std::endl;
  
}

class someOtherFunction : public processBase {
public:
  someOtherFunction(){
    list.add(new plugin::changeADCRange(1./4095));
  }
  virtual void operator()(SMIWaveformAnalyzer &wave){
    wave.processAll(list);

    static int i=0; 
    std::cout << i++ <<" " << wave.channel["IFES2"].V[11] << " " <<
      wave.channel["IFES2"].t[11] << " " << wave.getEventNum() <<
      " " << wave.getTriggerTime() << std::endl;
  } 
private:
  SMIAnalyzerPluginList list;
};

int main(int argc, char *argv[]){

  SMIROOTfileAnalyzer analyzer("examples/rootfile/test.cfg");
  analyzer.addFile("scitil_2014jan_cosy_r077-000.root");
  analyzer.setADCRange(4095);
  bool a = analyzer.associateTTreeBranches();
  std::cout << a << std::endl;

  processCalculateBaseline test;
  analyzer.loop(200,100,test);
  auto retval = test.getBaseline(analyzer);
  processDemo demo;
  demo.setBaseLine(retval);
  analyzer.loop(200,100,demo);

  /*  for(auto i : retval){
    std::cout << i.first  << " " << i.second << std::endl;
    }*/

  //someOtherFunction f;
  //analyzer.loop(0,10,f);
  return 0;
}
