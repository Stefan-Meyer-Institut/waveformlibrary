#include "chaintools.hh"

namespace ChainTools {
  TChain *openchain(std::string chainname,std::vector<std::string> filename){
    TChain *chain = new TChain(chainname.c_str());
    addfiletochain(chain,filename);
    return chain;  
  }

  bool addfiletochain(TChain *chain, std::vector<std::string> filename){
    if(chain == NULL) return false;
    for(std::vector<std::string>::iterator i=filename.begin(); i<filename.end(); i++){
      chain->Add((*i).c_str());
    }  
    return true;
  }

#if __cplusplus >= 201103L
  std::vector<std::array<size_t, 2> > getfileboundry(TChain *chain){
    std::vector<std::array<size_t, 2> > output;
    size_t totalentries = chain->GetEntries();
  
    size_t old = 0;
    for(int i=0; i<chain->GetNtrees(); i++){ // loop over TTrees start
      chain->GetEntry(old);
      TTree *tree = chain->GetTree();
      size_t n = old+tree->GetEntries();
      // divide it into junks of 2000 entries
      size_t diff = n-old;
      while(diff>2000){
	std::array<size_t, 2> tmp = {{old, old+2000}};
	output.push_back(tmp);
	old += 2000;
	diff = n-old;
      }
      std::array<size_t, 2> tmp = {{old, n}};
      output.push_back(tmp);

      if(n<totalentries) old = n;
      tree = NULL;
    }// loop over TTrees start

    return output;
  }
#endif
}
