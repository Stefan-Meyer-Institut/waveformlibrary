#include"SMIWave.hh"

std::ostream& operator<< (std::ostream &out, WaveForm &Info){
  std::map<std::string, double>::iterator it;
  for(it=Info.result.begin(); it!=Info.result.end(); it++)
    out << it->second << " ";
  out << Info.isDigital;
  return out;
}
