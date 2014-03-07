#include"SMIWaveform.hh"
#include<cstdlib>
#include<fstream>
#include<ctime>

SMIWaveform::SMIWaveform(){
  // initialize random seed from hardware source 
  // WARNING this is OS dependent!!!
  std::ifstream randseedgen;
  unsigned int devrand;
  randseedgen.open("/dev/random", std::ios::binary);
  if(randseedgen.is_open()){
    char *mem = new char [sizeof(int)];
    randseedgen.read(mem,sizeof(int));
    devrand = *reinterpret_cast<int*>(mem);
    delete [] mem;
  }
  else
    devrand = 0;
  randseedgen.close();
  unsigned int seconds = std::time(NULL); // current time in seconds since the epoch
  std::srand(seconds xor devrand);

}

inline double SMIWaveform::getRand(double low, double high) const {
  return low+(high-low)*rand()/RAND_MAX;
}
