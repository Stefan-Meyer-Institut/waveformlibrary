#include"SMIWaveform.hh"
#include<cstdlib>
#include<fstream>
#include<ctime>

SMIWaveform::SMIWaveform() : adcBits(12), adcRange(1.0)
#ifdef CPP11
			   ,dist(0,1)
#endif
 {
  initRand();
  adcDiff = adcRange/(1<<adcBits);
}

SMIWaveform::SMIWaveform(size_t _nEntries) : adcBits(12), adcRange(1.0) 
#ifdef CPP11
			   ,dist(0,1)
#endif
{
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

void SMIWaveform::addRandomness(WaveForm &wave){
  for(size_t i=0; i<wave.V.size(); i++)
    wave.V[i] += getRand(-adcDiff/2, adcDiff/2);
}

void SMIWaveform::changeTime(double time){
  std::map<std::string, WaveForm>::iterator it;
  for(it=channel.begin(); it!=channel.end(); it++)
    it->second.setTimeBins(time);
  
  for(it=trigger.begin(); it!=trigger.end(); it++)
    it->second.setTimeBins(time);  
}

inline bool SMIWaveform::shiftChannelTime(std::string name, double time){
  if(!testChannel(name)) return false;
  channel[name].shiftTime(time);
  return true;
}
inline bool SMIWaveform::shiftTriggerTime(std::string name, double time){
  if(!testTrigger(name)) return false;
  trigger[name].shiftTime(time);
  return true;
}

inline double SMIWaveform::getRand(double low, double high) {
  #ifdef CPP11
  auto val = decltype(dist)::param_type(low,high);
  return dist(gen,val);
  #else
  return low+(high-low)*rand()/RAND_MAX;
  #endif
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
  
  #ifdef CPP11
  gen.seed(seconds xor devrand xor rd());
  
  #else
  std::srand(seconds xor devrand);

  #endif
}
