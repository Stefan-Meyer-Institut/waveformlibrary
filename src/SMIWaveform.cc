#include"SMIWaveform.hh"
#include<cstdlib>
#include<fstream>
#include<ctime>

SMIWaveform::SMIWaveform() : adcBits(12), adcRange(1.0) {
  initRand();
  adcDiff = adcRange/(1<<adcBits);
}

SMIWaveform::SMIWaveform(size_t _nEntries) : adcBits(12), adcRange(1.0) {
  initRand();
  nEntries = _nEntries;
  adcDiff = adcRange/(1<<adcBits);
}

bool SMIWaveform::fillChannel(std::string name, double *data){
  if(!testChannel(name)) return false;
  for(size_t i=0; i<channel[name].V.size(); i++)
    channel[name].V[i] = data[i] + getRand(-adcDiff/2, adcDiff/2);
  return true;
}

bool SMIWaveform::fillTrigger(std::string name, double *data){
  if(!testTrigger(name)) return false;
  for(size_t i=0; i<trigger[name].V.size(); i++)
    trigger[name].V[i] = data[i] + getRand(-adcDiff/2, adcDiff/2);
  return true;
}

inline double SMIWaveform::getRand(double low, double high) const {
  return low+(high-low)*rand()/RAND_MAX;
}

void SMIWaveform::initRand(){
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
