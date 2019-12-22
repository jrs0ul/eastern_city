#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "DArray.h"
#include "SystemConf.h"


void SystemConf::SkipComments(FILE* conf){
	char simb=0;
	simb=fgetc(conf);
	while ((simb==' ')&&(simb!=-1)){
		simb=fgetc(conf);
	}
	if (simb=='#'){
		while(simb!=10)
			simb=fgetc(conf);
	}
	else
		fseek(conf,-1,0);
}

//----------------------------
bool SystemConf::load(const char* path, bool use3D){
	 FILE* conf;

 fopen_s(&conf,path,"rt");
 if (!conf)
  return false;
//-------------------

  fseek (conf , 0 , SEEK_END);
  long size = ftell (conf);
  rewind (conf);
  char * buferis=0;
  buferis = new char [size];
  int index=0;
  while (!feof(conf)){
	buferis[index]=fgetc(conf);
	index++;
  }
  buferis[index-1]=0;
  fclose(conf);

  char skirtukai[2];
  skirtukai[0]=10;
  skirtukai[1]=0;

  DArray<char*> eilutes;
  char* tokenas;
  
  tokenas=strtok(buferis,skirtukai);
  while (tokenas){
	  char * eilute = new char[strlen(tokenas)+1];
	  strcpy(eilute,tokenas);
	  eilutes.add(eilute);
	  tokenas=strtok(0,skirtukai);
  }
  delete []buferis;

  for (int i=0; i<eilutes.count(); i++){
	char* token = 0;
	char* delim = " ";
	token = strtok(eilutes[i],delim);
	while (token){
		if (token[0]=='#')
			break;

		if (strcmp("width",token)==0){
			token=strtok(0,delim);
			width=atoi(token);
		}
		else
		if (strcmp("height",token)==0){
			token=strtok(0,delim);
			height=atoi(token);
		}
		else
		if (strcmp("bits",token)==0){
			token=strtok(0,delim);
			bits=atoi(token);
		}
		else
		if (strcmp("iswindowed",token)==0){
			token=strtok(0,delim);
			int isw=atoi(token);
			(isw>0) ? windowed=true : windowed=false;
		}
		else
		if (strcmp("sfxvolume",token)==0){
			token=strtok(0,delim);
			soundFXVolume=atoi(token);
		}
		else
		if (strcmp("musicvolume",token)==0){
			token=strtok(0,delim);
			musicVolume=atoi(token);
		}
		else
		if ((strcmp("antialias",token)==0)&&(use3D)){
			token=strtok(0,delim);
			antialias=atoi(token);
		}
		else
		if ((strcmp("usezbuff",token)==0)&&(use3D)){
			token=strtok(0,delim);
			int usez=0;
			usez=atoi(token);
			(usez>0) ? enableDepthStencil=true : enableDepthStencil=false;
		}
		else
		if (strcmp("priority",token)==0){
			token=strtok(0,delim);
			priority=atoi(token);
		}

		token=strtok(0,delim);
		
	}
  }

  for (int i=0;i<eilutes.count();i++){
	  delete []eilutes[i];
  }
  eilutes.destroy();
  
  return true;
}
