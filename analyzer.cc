#include"SMIROOTfileAnalyzer.hh"
#include"SMIWaveformAnalyzerPluginSystem.hh"

//plugins
#include"plugins/changeADCRange.hh"

// STL 
#include<iostream>

void someFunction(SMIROOTfileAnalyzer &wave){
  static int i=0; 
  std::cout << i++ <<" " << wave.channel["IFES2"].V[11] << " " <<
    wave.channel["IFES2"].t[11] << " " << wave.getEventNum() <<
    " " << wave.getTriggerTime() << std::endl;
  
}

class someOtherFunction {
public:
  someOtherFunction(){
    list.add(new plugin::changeADCRange(1./4095));
  }
  void operator()(SMIROOTfileAnalyzer &wave){
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
  analyzer.loop(0,10,someFunction);
  someOtherFunction f;
  analyzer.loop(0,10,f);
  return 0;
}
