#include"SMIMidasAnalyzer2012.hh"

#ifdef __HAVEMIDAS

#include"TMidasEvent.h"
#include<cstdio>
#include<algorithm>

bool SMIMidasAnalyzer2012::loop(size_t start, size_t num, processBase &func){
  return true;
}

bool SMIMidasAnalyzer2012::readConfigFile(std::string configfile){
  return true;
}

bool SMIMidasAnalyzer2012::loadEvent(size_t num){
  return true;
}

bool SMIMidasAnalyzer2012::prepareMidasEvent(TMidasEvent &event){
  std::vector<std::string> groupheaders;
  std::vector<std::string> eventheaders;
  for(std::map<std::string,std::string>::iterator it = midasChannelBanks.begin();
      it != midasChannelBanks.end(); it++){
    // get group headers
    char name[128];
    int chadcnum=0, grnum = 0, chnum = 0;
    sscanf(it->first.c_str(),"A%i%i%i",&chadcnum,&grnum, &chnum);
    sprintf(name,"A%i%iH",chadcnum,grnum);
    groupheaders.push_back(name);
    sprintf(name,"A%iEH",chadcnum);
    eventheaders.push_back(name);
  }
  std::sort(groupheaders.begin(),groupheaders.end());
  std::sort(eventheaders.begin(),eventheaders.end());
  std::vector<std::string>::iterator lastgroup = std::unique(groupheaders.begin(),groupheaders.end());
  std::vector<std::string>::iterator lastevent = std::unique(eventheaders.begin(),eventheaders.end());

  void *ptr;
  std::map<int,V1742::rawEvent> adcEvent;
  std::map<int,V1742::sheader *>  adcHeader;
  std::map<int,V1742::sgheader *> adcgroups;

  std::map<int,double **> groupdata;
  std::map<int,int> subevent;
  for(std::vector<std::string>::iterator it=eventheaders.begin(); it!=lastevent; it++){
    int c = event.LocateBank(NULL, it->c_str(), &ptr);
    int adcnum = 0;
    sscanf(it->c_str(),"A%nEH",&adcnum);
      
    // decode event header information
    adcHeader[adcnum] = decodeEventHeader(ptr,c);
    subevents = c/20;
    subevent [adcnum] = subevents;
    // decode group header information

    std::vector<std::string>::iterator git = groupheaders.begin();
    for(; git!=lastgroup; git++){ // group loop start
      if((*git)[1] != (*it)[1]) continue;
      WORD *data[9];
      int entr = event.LocateBank(NULL, git->c_str(), &ptr);
      int gnum = 0;
      sscanf(it->c_str(),"A%i%i",&adcnum, &gnum);

      int groupnumber = (adcnum<<2) + gnum;

      adcgroups[groupnumber] = decodeGroupHeader(ptr,entr);  
      
      // decode channels
      char name[128];
      for(int channel=0; channel<9; channel++){
	if(channel == 8)  sprintf(name,"A%i%iT",adcnum,gnum);
	else sprintf(name,"A%i%i%i",adcnum,gnum,channel);
	event.LocateBank(NULL, name, (void**)&data[channel]);
      }

      if(groupdata[(adcnum<<2) + gnum] == NULL){
	groupdata[(adcnum<<2) + gnum] = new double*[9];
	for(int n=0; n<9; n++) groupdata[groupnumber][n] = new double[1024*subevents];
      }
      
      for(int n=0; n<9; n++){
	for(int ent=0; ent<1024*subevents; ent++)
	  groupdata[groupnumber][n][ent] = (double)data[n][ent];
      }

      // get calibration pattern
      float *timecorr;
      short *cell, *nsample;
      sprintf(name,"A%i%iC",adcnum,gnum);
      event.LocateBank(NULL, name, (void**)&cell);
      sprintf(name,"A%i%iN",adcnum,gnum);
      event.LocateBank(NULL, name, (void**)&nsample);
      sprintf(name,"A%i%iM",adcnum,gnum);
      event.LocateBank(NULL, name, (void**)&timecorr);

      
      // build correction table
      V1742::V1742_DataCorrection_t CTable;
      for(int k=0;k<1024;k++)
	CTable.time[k] = timecorr[k];
      for(int k=0; k<9; k++){	  
	for(int l=0;l<1024;l++){
	  CTable.cell[k][l] = cell[1024*k+l];
	  CTable.nsample[k][l] = nsample[1024*k+l];
	}
      }
      
      // apply calibration pattern
      double **tmp = new double*[9];
      for(int n=0; n<subevents; n++){
	for(int ch=0; ch<9; ch++)
	  tmp[ch] = groupdata[groupnumber][ch] + n*1024;
	V1742::ApplyDataCorrection(&CTable, 0, 7, &(adcgroups[groupnumber][n]), tmp);
      }
      delete [] tmp;
    } // group loop end
  }

  // assemble events
  size_t eventnum = 999999999;
  for(std::map<int,int>::iterator it=subevent.begin(); it!=subevent.end(); it++){
    if(eventnum>it->second) eventnum=it->second;
  }
  std::map<int,V1742::rawEvent> eventmatch;
  for(size_t e=0; e<eventnum; e++){
    std::vector<std::string>::iterator it=eventheaders.begin();
    std::vector<std::string>::iterator git=groupheaders.begin();
    for(; it!=lastevent; it++){
      int adcnum = 0;
      sscanf(it->c_str(),"A%nEH",&adcnum);
      eventmatch[adcnum].header = adcHeader[adcnum][e];
      for(;git!=lastgroup; git++){
	int gnum = 0;
	sscanf(it->c_str(),"A%i%i",&adcnum, &gnum);

	int groupnumber = (adcnum<<2) + gnum;
	eventmatch[adcnum].group[gnum].groupHeader=adcgroups[groupnumber][e];
	for(int i=0; i<9; i++)
	  std::memcpy(eventmatch[adcnum].group[gnum].data[i],groupdata[groupnumber][i] + 1024*e, 1024*sizeof(double));
      }
    }
    unmatchedMidasEvent.push_back(eventmatch);
  }
  
  // free allocated memory  
  for(std::map<int,V1742::sheader *>::iterator it=adcHeader.begin(); it!=adcHeader.end(); it++)
    delete [] it->second;
  for(std::map<int,V1742::sgheader *>::iterator it=adcgroups.begin(); it!=adcgroups.end(); it++)
    delete [] it->second;
  for(std::map<int,double **>::iterator it=groupdata.begin(); it!=groupdata.end(); it++){
    if(it->second != NULL){
      for(int n=0; n<9; n++) delete [] it->second[n];
      delete [] it->second;
    }
  }

  return true;
}

int SMIMidasAnalyzer2012::matchEvents(){

  if(unmatchedMidasEvent.size()==0) return true;
  if(unmatchedMidasEvent[0].size()==1){ // don't do matching of we only have on ADC
    for(int i=0; i<unmatchedMidasEvent.size(); i++){
      rawMidasEvent.push_back(unmatchedMidasEvent[i]);
    }
    unmatchedMidasEvent.clear();
    return true;
  }

  typedef std::map<int,V1742::rawEvent>::iterator mapit;
  mapit it;
  std::vector<std::map<int,mapit> > matchList;
  std::map<int,V1742::rawEvent> match;
  for(int i=0; i<unmatchedMidasEvent.size(); i++){ // matchloop start
    int up  = i+srad;
    int low = i-srad;
    up  = up>unmatchedMidasEvent.size() ? unmatchedMidasEvent.size() : up;
    low = low<0 ? 0 : low;

    // try to match event
    // adc to match against
    int adc = unmatchedMidasEvent[i].begin()->first;
    match[adc] = unmatchedMidasEvent[i].begin()->second;
    double time = match[adc].group[0].groupHeader.grtime;
    for(int j=low; j<up; j++){ // searchloop
      it = unmatchedMidasEvent[j].begin();
      for(;it!=unmatchedMidasEvent[j].end(); it++){
	//if(adc == it->first) continue;
	if(match.find(it->first) != match.end()) continue;
	if(std::abs(time-it->second.group[0].groupHeader.grtime) < timeDiff){
	  match[it->first] = it->second;
	  unmatchedMidasEvent[j].erase(it);
	  continue;
	}      
      }
    } // searchloop end
    if(match.size() == ADCNum){
      rawMidasEvent.push_back(match);
    } else {
      unmatchedMidasEvent.push_back(match);
      match.clear();
    }

  } // matchloop end

  return true;
}

V1742::sheader *SMIMidasAnalyzer2012::decodeEventHeader(void *ptr, size_t size){
  char *data = (char *)ptr;
  size_t event=0;
  V1742::sheader *retval = new V1742::sheader[size];
  while(event < size){
    retval[event].total_size = *(unsigned int*)(data);
    data += sizeof(unsigned int);
    retval[event].board_id = *(unsigned short int*)(data);
    data += sizeof(unsigned short int);
    retval[event].pattern = *(unsigned short int*)(data);
    data += sizeof(unsigned short int);
    retval[event].group_mask = *(unsigned short int*)(data);
    data += sizeof(unsigned int);
    retval[event].event_counter = *(unsigned int*)(data);
    data += sizeof(unsigned int);
    retval[event].event_time_tag = *(unsigned int*)(data);
    data += sizeof(unsigned int);
    event++;
  }
  return retval;
}

V1742::sgheader *SMIMidasAnalyzer2012::decodeGroupHeader(void *ptr, size_t size){
  char *data = (char *)ptr;
  size_t event=0;
  V1742::sgheader *retval = new V1742::sgheader[size];
  while(event < size){
    retval[event].is_active = *(unsigned short int*)(data);
    data += sizeof(unsigned short int);
    retval[event].start_index = *(unsigned short int*)(data);
    data += sizeof(unsigned short int);
    retval[event].freq = *(unsigned short int*)(data);
    data += sizeof(unsigned short int);
    retval[event].tr = *(unsigned short int*)(data);
    data += sizeof(unsigned short int);
    retval[event].ch_size = *(unsigned short int*)(data);
    data += sizeof(unsigned int);
    retval[event].grtime = *(unsigned int*)(data);
    data += sizeof(unsigned int);
    event++;
   }
  return retval;
}

#endif
