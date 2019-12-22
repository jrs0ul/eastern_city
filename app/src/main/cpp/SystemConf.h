#ifndef SYSCONF_H
#define SYSCONF_H

/*

Jrs0ul
2007 04 28

*/


class SystemConf{

	void SkipComments(FILE* conf);

public:
 bool usespecular;
 bool enableDepthStencil;
 int antialias;
 bool tnl;

 int width;
 int height;
 int bits;
 bool windowed;

 bool shutdown;
 int maxmodes;
 int currmode;
 int devindex;

 long soundFXVolume;
 long musicVolume;

 bool soundCardWorks;
 bool joypresent;

 int priority;

 
 SystemConf(){
  usespecular=false;
  antialias=0;
  width=640;
  height=480;
  bits=16;
  soundCardWorks=false;
  shutdown=false;
  maxmodes=0;
  currmode=0;
  devindex=0;
  soundFXVolume=0;
  musicVolume=0;
  windowed=true;
  joypresent=false;
  enableDepthStencil=false;
  priority=0;
 }
//------------------------------------
 bool load(const char* path, bool use3D);
};

#endif //SYSCONF_H