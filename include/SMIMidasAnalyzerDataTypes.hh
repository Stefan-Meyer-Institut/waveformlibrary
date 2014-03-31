#ifndef __SMIMIDASANALYZERDATATYPES_HH__
#define __SMIMIDASANALYZERDATATYPES_HH__

typedef unsigned long int  DWORD;
typedef unsigned short int WORD;
typedef long int INT;

#if __cplusplus >= 201103L
#include<cstdint>
#else
namespace std {
#include<stdint.h>
}
#endif
#include<cstring>
#include<string>
#include<map>
namespace V1742 {

  struct sgheader {
    WORD  is_active;
    //WORD tr_present
    WORD  start_index;
    WORD  freq;
    WORD  tr;
    WORD  ch_size;
    DWORD grtime;
  };
  
  struct  V1742_DataCorrection_t {
    int16_t cell   [9][1024];
    int8_t  nsample[9][1024];
    float   time      [1024];
  };
  
  struct sheader {
    DWORD total_size;
    WORD  board_id;
    WORD  pattern;
    WORD  group_mask;
    DWORD event_counter;
    DWORD event_time_tag;
  };
  
  struct rawGroup {
    sgheader groupHeader;
    V1742_DataCorrection_t correction;
    double data[9][1024];
  };

  struct rawEvent {
    sheader header;
    std::map<int,rawGroup> group;
  };

  void PeakCorrection(struct sgheader *header, float **dataout);
  void ApplyDataCorrection(V1742_DataCorrection_t* CTable, 
			   uint32_t frequency, int CorrectionLevelMask, 
			   struct sgheader *header, double **cdata);

}

#endif
