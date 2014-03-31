#include"SMIMidasAnalyzerDataTypes.hh"

namespace V1742 {

  void PeakCorrection(struct sgheader *header, double **dataout){
    int offset;
    int chaux_en;
    int i;
    int j;

    //chaux_en = (dataout->ChSize[8] == 0)? 0:1;                                                                               
    chaux_en = header->tr;
    for(j=0; j<(8+chaux_en); j++){
      dataout[j][0] = dataout[j][1];
    }
    for(i=1; i<header->ch_size/3; i++){
      offset=0;
      for(j=0; j<8; j++){
	if (i==1){
	  if ((dataout[j][2]- dataout[j][1])>30){
	    offset++; }
	  else {
	    if (((dataout[j][3]- dataout[j][1])>30)&&
		((dataout[j][3]- dataout[j][2])>30)){
	      offset++; }
	  }
	}
	else{
	  if ((i==header->ch_size/3-1)&&((dataout[j][header->ch_size/3-2]- dataout[j][header->ch_size/3-1])>30)){
	    offset++;  }
	  else{
	    if ((dataout[j][i-1]- dataout[j][i])>30){
	      if ((dataout[j][i+1]- dataout[j][i])>30)
		offset++;
	      else {
		if ((i==header->ch_size/3-2)||((dataout[j][i+2]-dataout[j][i])>30))
		  offset++; }
	    }  
	  }
	}
      }

      if (offset==8){
	for(j=0; j<(8+chaux_en); j++){
	  if (i==1){
	    if ((dataout[j][2]- dataout[j][1])>30) {
	      dataout[j][0]=dataout[j][2];
	      dataout[j][1]=dataout[j][2];
	    }  
	    else{
	      dataout[j][0]=dataout[j][3];
	      dataout[j][1]=dataout[j][3];
	      dataout[j][2]=dataout[j][3];
	    }
	  }
	  else{
	    if (i==header->ch_size/3-1){
	      dataout[j][header->ch_size/3-1]=dataout[j][header->ch_size/3-2];
	    }
	    else{
	      if ((dataout[j][i+1]- dataout[j][i])>30)
		dataout[j][i]=((dataout[j][i+1]+dataout[j][i-1])/2);
	      else {
		if (i==header->ch_size/3-2){
		  dataout[j][header->ch_size/3-2]=dataout[j][header->ch_size/3-3];
		  dataout[j][header->ch_size/3-1]=dataout[j][header->ch_size/3-3];
		}
		else {
		  dataout[j][i]=((dataout[j][i+2]+dataout[j][i-1])/2);
		  dataout[j][i+1]=( (dataout[j][i+2]+dataout[j][i-1])/2);
		}
	      }
	    }
	  }
	}
      }
    }
  
  }

  void ApplyDataCorrection(V1742_DataCorrection_t* CTable, 
			   uint32_t frequency, int CorrectionLevelMask, 
			   struct sgheader *header, double **cdata){
    int i, j, size1, trg = 0,k;
    float Time[1024],t0;
    float Tsamp;
    float vcorr;
    uint16_t st_ind=0;
    double wave_tmp[1024];
    int cellCorrection =CorrectionLevelMask & 0x1;
    int nsampleCorrection = (CorrectionLevelMask & 0x2) >> 1;
    int timeCorrection = (CorrectionLevelMask & 0x4) >> 2;

    switch(frequency) { // CHECK !!!!!                                                                                         
    case 1:
      Tsamp =(float)((1.0/2500.0)*1000.0);
      break;
    case 2:
      Tsamp =(float)((1.0/1000.0)*1000.0);
      break;
    default:
      Tsamp =(float)((1.0/5000.0)*1000.0);
      break;
    }
    //printf("Tsamp=%f\n",Tsamp);                                                                                              
    //if (data->ChSize[8] != 0) trg = 1;                                                                                       

    trg = header->tr;

    st_ind =(uint16_t)(header->start_index);
    for (i=0;i<8+trg;i++) {
      size1  = header->ch_size/3;

      for (j=0;j<size1;j++) {
	//printf("cdata[%i][%i] = %f \n",i,j,cdata[i][j] - CTable->cell[i][((st_ind+j) % 1024)]);                              
	if (cellCorrection) cdata[i][j] -=  CTable->cell[i][((st_ind+j) % 1024)];
	if (nsampleCorrection) cdata[i][j] -= CTable->nsample[i][j];
      }
    }
    //printf("starting PeakCorrection\n");                                                                                     
    if (cellCorrection) PeakCorrection(header,cdata);
    //printf("ending PeakCorrection\n");                                                                                       
    if (!timeCorrection) return;

    t0 = CTable->time[st_ind];
    Time[0]=0.0;

    for(j=1; j < 1024; j++) {
      t0= CTable->time[(st_ind+j)%1024]-t0;
      if  (t0 >0)
	Time[j] =  Time[j-1]+ t0;
      else
	Time[j] =  Time[j-1]+ t0 + (Tsamp*1024);

      t0 = CTable->time[(st_ind+j)%1024];
    }
    for (j=0;j<8+trg;j++) {
      cdata[j][0] = cdata[j][1];
      wave_tmp[0] = cdata[j][0];
      vcorr = 0.0;
      k=0;
      i=0;

      for(i=1; i<1024; i++)  {
	while ((k<1024-1) && (Time[k]<(i*Tsamp)))  k++;
	vcorr =(((float)(cdata[j][k] - cdata[j][k-1])/(Time[k]-Time[k-1]))*((i*Tsamp)-Time[k-1]));
	wave_tmp[i]= cdata[j][k-1] + vcorr;
	k--;
      }
      memcpy(cdata[j],wave_tmp,1024*sizeof(double));
    }

  }

}
