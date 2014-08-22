#include"SMIMidasAnalyzer2014.hh"

#ifdef __HAVEMIDAS

#include"TMidasEvent.h"
#include<cstdio>
#include<algorithm>
#include<iostream>
#include<cstring>

bool SMIMidasAnalyzer2014::readConfigFile(std::string configfile){
  // check if file is existing and is not empty
  std::ifstream ifile(configfile.c_str());
  if(!ifile) return false;
  ifile.seekg (0, ifile.end);
  int length = ifile.tellg();
  ifile.seekg (0, ifile.beg);
  if(length == 0){
    std::cerr << "File is empty!" << std::endl;
    return false;
  }

  char line[256];
  std::string command, ch, trig, bankname, nameScheme;
  int adcNum, pulse;

  // read used ADC configuration
  // format: A [adcnum] [namingscheme] [pulsetype]
  // nameingscheme is a string, with added -%i with channel number
  do {
    if(ifile.peek()=='#') ifile.getline(line,255);
    else {
      ifile >> command;
      if(command == "A"){
	ifile >> adcNum >> nameScheme >> pulse;
	prepareADCBuffers(adcNum,nameScheme,pulse);
      } else { // ignore
	ifile.getline(line,255);
      } 
    }
  } while(!ifile.eof());
  ifile.clear();
  ifile.seekg(0, ifile.beg);

  // now change individual channel configurations
  // format C [adcnum] [groupnum] [chnum] [name] [pulsetype]
  // format T [adcnum] [groupnum] [name] [pulsetype]
  int groupNum, channelNum;
  do {
    if(ifile.peek()=='#') ifile.getline(line,255);
    else {
      ifile >> command;
      if(command == "C"){ // channel
	ifile >> adcNum >> groupNum >> channelNum >> nameScheme >> pulse;
	char buf[5];
	sprintf(buf,"A%X%i%i",adcNum, groupNum, channelNum);
	renameChannel(chnames[buf], nameScheme);
	channel[nameScheme].pulseType = pulse;
      } else  if(command == "T"){ // trigger
	ifile >> adcNum >> groupNum >> nameScheme >> pulse;
	char buf[5];
	sprintf(buf,"A%X%iT",adcNum, groupNum);
	renameTrigger(chnames[buf], nameScheme);
	trigger[nameScheme].pulseType = pulse;
      } else { // ignore
	ifile.getline(line,255);
      } 
    }
  } while(!ifile.eof());

  ifile.close();
  return true;
}

bool SMIMidasAnalyzer2014::prepareMidasEvent(TMidasEvent &event){
  bool retVal = true;
  for(std::vector<std::string>::iterator i=adcBankList.begin();
      i!=adcBankList.end(); i++) {
    DWORD *data = NULL;
    int c = event.LocateBank(NULL, i->c_str(), (void**)&data);
   
    if(c==0 || data == NULL){
      retVal = false;
      return retVal;
    }
    //std::cout << c << std::endl;
    unsigned int adcNum = 0;
    char tmp[5];
    std::strcpy(tmp,i->c_str());
    tmp[2]='X';
    sscanf(tmp, "A%XXA", &adcNum);
    decodeADCData(data,adcNum);
  }
  
  // for(int i=0; i<1024; i++){
  //   for(auto j : channel){
  //     std::cout << j.second.V[i] << " ";}
  //   std::cout << std::endl;

  // }

  return retVal;
}

bool SMIMidasAnalyzer2014::prepareCorrectionTables(TMidasEvent &event){
  //  std::cout << "CorrTab " << 1 << std::endl;
  bool retVal = true;
  for(std::vector<std::string>::iterator i=adcBankList.begin();
      i!=adcBankList.end(); i++) {
    //std::cout << "CorrTab " << 2 << std::endl;
    unsigned int adcNum = 0;
    char tmp[5];
    std::strcpy(tmp,i->c_str());
    tmp[2]='X';
    sscanf(tmp, "A%XXA", &adcNum);
    //std::cout << "ADCNUM=" << adcNum << std::endl;
    for(unsigned int g=0; g<4; g++){
      float *timecorr;
      short *cell, *nsample;
      char buf[5];
      // open midas banks
      sprintf(buf,"A%X%uC",adcNum, g);
      int c = event.LocateBank(NULL, buf, (void**)&cell);
      if(c==0){	retVal = false;	break;}
      //std::cout << buf << std::endl;
      //std::cout << "c=" << c<< std::endl;
      sprintf(buf,"A%X%uN",adcNum, g);
      c     = event.LocateBank(NULL, buf, (void**)&nsample);
      if(c==0){	retVal = false;	break;}
      sprintf(buf,"A%X%uM",adcNum, g);
      //std::cout << buf << std::endl;
      c     = event.LocateBank(NULL, buf, (void**)&timecorr);
      if(c==0){	retVal = false;	break;}
      //std::cout << "c=" << c<< std::endl;
      // fill tables

      // for(int i=0; i<1024; i++){
      // 	std::cout << "timecorr: " << timecorr[i] << std::endl;
      // }
      // std::memcpy(correctionTable[adcNum][g].time, timecorr, 1024*sizeof(float));
      for(int i=0; i<1024; i++){
	correctionTable[adcNum][g].time[i] = timecorr[i];
      }
      for(size_t i=0; i<9; i++){
	for(int j=0; j<1024; j++){
	  correctionTable[adcNum][g].cell   [i][j] = cell   [1024*i + j];
	  correctionTable[adcNum][g].nsample[i][j] = nsample[1024*i + j];
	}
	// std::memcpy(correctionTable[adcNum][g].cell   [i], cell   +1024*i, 1024*sizeof(short) );
	// std::memcpy(correctionTable[adcNum][g].nsample[i], nsample+1024*i, 1024*sizeof(short) );
      }
    } // group loop end
  } // ADC loop end
  tablesLoaded = retVal;
  return retVal;
}

void SMIMidasAnalyzer2014::decodeADCData(DWORD *data, unsigned int ADCnum){
  //std::cout << "start" << std::endl; 
  unsigned int off = 4; // first dword in group data
  // std::cout << data[0] << std::endl 
  // 	    << data[1] << std::endl 
  // 	    << data[2] << std::endl
  // 	    << data[3] << std::endl 
  // 	    << data[4] << std::endl 
  // 	    << data[5] << std::endl 
  // 	    << data[6] << std::endl 
  // 	    << data[7] << std::endl 
  // 	    << data[8] << std::endl 
  // 	    << data[9] << std::endl;
  // std::cout << std::endl;

  // event header mask defintions
  //static const unsigned int tsmask        = 0x0FFFFFFF;
  //static const unsigned int bmask         = 0xF8000000;
  static const unsigned int grmask        = 0x0000000F;
  //static const unsigned int evcountermask = 0x003FFFFF;

  // group header mask definitons
  static const unsigned int trmask        = 0x00000001; // + 12 bit
  static const unsigned int freqmask      = 0x00000003; // + 16 bit
  static const unsigned int indexmask     = 0x000003FF; // + 20 bit
  static const unsigned int ch0mask       = 0x00000FFF;

  static const unsigned int grtimemask    = 0x3FFFFFFF;

  // decode event header
  // unsigned int total_size     =  data[0] & tsmask;
  // unsigned int board_id       = (data[1] & bmask)>>27;
  unsigned int group_mask     = (data[1] & grmask);
  // unsigned int pattern        = (data[1] & bmask)>>8;
  // unsigned int event_counter  = (data[2] & evcountermask);
  // unsigned int event_time_tag =  data[3];
  //std::cout << "groupmask: " << group_mask << std::endl;
  // check which groups participate in the event

  //printf("0x%X\n",data[0]);
  //printf("0x%X\n",data[1]);
  //printf("0x%X\n",data[2]);
  //printf("0x%X\n",data[3]);

  int gr[4]={1,1,1,1};
  for(int i=0; i<4; ++i)
    gr[i] = ((group_mask >> i) & 0x1);

  for (unsigned int g = 0; g < 4; ++g) {
    //std::cout << std::endl << "Group start " << g << std::endl << std::endl; 
    //std::cout << "g=" << gr[g] << std::endl; 
    /* test if group is active */
    if(!gr[g]) continue;
    //static const double freq[3] = { 5, 2.5, 1 };
    //printf("0x%X\n",data[off]);
    unsigned int ghdr = data[off++]; // group header
    
    // decode group header 
    struct V1742::sgheader groupHeader;
    groupHeader.start_index =      (ghdr >> 20) & indexmask ;
    //groupHeader.freq        = freq[(ghdr >> 16) & freqmask ];
    groupHeader.freq        =      (ghdr >> 16) & freqmask  ;
    groupHeader.tr          =     ((ghdr >> 12) & trmask   ) ? 1 : 0;
    groupHeader.ch_size     =       ghdr        & ch0mask;
    //std::cout <<   groupHeader.start_index << std::endl;   
    // read actual values 
    /* groups of 8 * 12 bit data
     * so read   3 * 32 bit at once
     */
    for (size_t i = 0; i < groupHeader.ch_size/3; ++i) {	
      // read raw data int temporary buffer
      unsigned int tempData[3];
      //printf("0x%X\n",data[off]);
      tempData[0] = data[off++];
      //printf("0x%X\n",data[off]);
      tempData[1] = data[off++];
      //printf("0x%X\n",data[off]);
      tempData[2] = data[off++];
      //std::cout << "OFF=" << off << std::endl;
      /* extract */
      /* 7776 6655 | 5444 3332 | 2211 1000 */
      unsigned int ch[8];
      ch[0] =  (tempData[0] >>  0) & 0xfff;
      ch[1] =  (tempData[0] >> 12) & 0xfff;
      ch[2] = ((tempData[0] >> 24) & 0x0ff)
	|                (((tempData[1] >>  0) & 0x00f)) << 8;
      ch[3] =  (tempData[1] >>  4) & 0xfff;
      ch[4] =  (tempData[1] >> 16) & 0xfff;
      ch[5] = ((tempData[1] >> 28) & 0x00f)
	|                (((tempData[2] >>  0) & 0x0ff)) << 4;
      ch[6] =  (tempData[2] >>  8) & 0xfff;
      ch[7] =  (tempData[2] >> 20) & 0xfff;

      char name[5];
      //std::cout << "ASDASDASDAS " << tempData[0] <<" "<< ch[0] <<" "<< ch[1] <<  std::endl;
      // store data in analyzer structures
      for(unsigned int c=0; c<8; c++){
	sprintf(name, "A%X%u%u",ADCnum,g,c);
	//std::cout << ch[c] << " ";
	channel[chnames[name]].V[i] = (double)ch[c];
	//std::cout << chnames[name] << " " << i << " "<< channel[chnames[name]].V[i] << std::endl;
      }
      //std::cout << std::endl;
    } // channel readout loop end

    /* Tigger signal */
    /* 7776 6655 | 5444 3332 | 2211 1000*/
    /* ... 8 times -> 1024 samples*/
    if(groupHeader.tr){
      for (size_t i = 0; i < groupHeader.ch_size/3; i += 8)
	{
	  unsigned int tempData[3];
	  tempData[0] = data[off++];
	  tempData[1] = data[off++];
	  tempData[2] = data[off++];

	  char name[5];
	  sprintf(name, "A%X%u%c",ADCnum,g,'T');
	  std::string trg = chnames[name];

	  trigger[trg].V[i]   =  (tempData[0] >>  0) & 0xfff;
	  trigger[trg].V[i+1] =  (tempData[0] >> 12) & 0xfff;
	  trigger[trg].V[i+2] = ((tempData[0] >> 24) & 0x0ff)
	    |                  (((tempData[1] >>  0) & 0x00f)) << 8;
	  trigger[trg].V[i+3] =  (tempData[1] >>  4) & 0xfff;
	  trigger[trg].V[i+4] =  (tempData[1] >> 16) & 0xfff;
	  trigger[trg].V[i+5] = ((tempData[1] >> 28) & 0x00f)
	    |                  (((tempData[2] >>  0) & 0x0ff)) << 4;
	  trigger[trg].V[i+6] =  (tempData[2] >>  8) & 0xfff;
	  trigger[trg].V[i+7] =  (tempData[2] >> 20) & 0xfff;
	}
    }
    // now assemble groupdata for applying correction tables
    double *cdata[9];
    char name[5];
    for(int c=0; c<8; c++){
      sprintf(name, "A%X%u%u",ADCnum,g,c);
      cdata[c] = &(channel[chnames[name]].V[0]);
    }
    sprintf(name, "A%X%u%c",ADCnum,g,'T');
    cdata[8] = &(trigger[chnames[name]].V[0]);
    
    // std::cout << "TEST" << std::endl;
    // for(int i=0;i<1024;i++)
    //   std::cout << cdata[1][i] << std::endl;
    // std::cout << "TEST END" << std::endl;
    /* timestamp */
    groupHeader.grtime = data[off++] & grtimemask;

    // perform data correction
    // for(int i=0; i<1024; i++){
    //   std::cout << ADCnum << " " << g <<" ";
    //   std::cout << "timecorr: " << correctionTable[ADCnum][g].time[i] << std::endl;
    // }
    ApplyDataCorrection(&(correctionTable[ADCnum][g]), groupHeader.freq, 0x7, 
    			&groupHeader, cdata);

  } // group loop end

}

void SMIMidasAnalyzer2014::prepareADCBuffers(unsigned int adcNum, std::string nameScheme, int pulse){
  char name[5], trigName[5];
  int chNum = 0;
  int trNum = 0;
  sprintf(name,"A%XDA", adcNum);
  adcBankList.push_back(name);
  for(size_t group=0; group<4; group++){
    char buf[5];
    sprintf(trigName, "A%X%zu%c",adcNum,group,'T');
    sprintf(buf,"%u",trNum++);
    chnames[trigName] = nameScheme + "-Trigger-" + buf;

    for(size_t ch=0; ch<8; ch++){
      sprintf(name, "A%X%zu%zu",adcNum,group,ch);
      sprintf(buf,"%u",chNum++);
      chnames[name] = nameScheme + "-" + buf;
      addChannelTrigger(chnames[name],chnames[trigName]);
      channel[chnames[name]].pulseType = pulse;
    }
    trigger[chnames[trigName]].pulseType = pulse;

  } // group loop end
}

#endif
