#include"SMIWave.hh"

std::ostream& operator<< (std::ostream &out, WaveInfo &Info){
  out << Info.Q << " " << Info.A << " " << Info.T << " " 
      << Info.ToT << " " << Info.isDigital;
  return out;
}
