/*
 * linux.cpp
 *
 *  Created on: Jun 21, 2017
 *      Author: xieliming
 */
#include "linux.h"

void setMousePos(unsigned int y,unsigned int x)
{
	printf("\033[%d;%dH\033[0m",y,x);
}
void clearScreen()
{
	printf("\033[2J");
}
int GetKeyWord()
{
	int in;                                    //通过tcsetattr函数设置terminal的属性来控制需不需要回车来结束输入
	struct termios new_settings;
	struct termios stored_settings;
	tcgetattr(0,&stored_settings);
	new_settings = stored_settings;
	new_settings.c_lflag &= (~ICANON);         //屏蔽整行缓存。那就只能单个了
	new_settings.c_lflag &= ~ECHO;
	new_settings.c_cc[VTIME] = 0;
	new_settings.c_cc[VMIN] = 1;
	tcsetattr(0,TCSANOW,&new_settings);
	in = getchar();
	tcsetattr(0,TCSANOW,&stored_settings);
	return in;
}
int Rand_value()
{
	struct timeval tv;
	memset(&tv,0,sizeof(tv));
	gettimeofday(&tv,NULL);
	srand(tv.tv_sec*1000000+tv.tv_usec);
	return rand();
}
bool RegisteSignal(int sig,void (*handler)(int _sig))
{
	struct sigaction sa;
	memset(&sa,0,sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_flags=0;
	sa.sa_handler=handler;
	if(sigaction(sig,&sa,NULL)==-1)
		return false;
	return true;
}
int Bitap(const char *text, const char *find)  //0~
{
    int text_len = strlen(text);
    int find_len = strlen(find);
    if (text_len==0||find_len==0||(text_len < find_len))    return -1;
    int i = 0 ,j=find_len - 1;
    char *map=(char*)malloc(find_len + 1);
    *(map+0) = 1;
    for (i=1; i<=find_len; ++i)
    {
        *(map+i) = 0;
    }
    for (i=0; i< text_len; ++i)
    {
        for (j=find_len-1; j>=0; --j)
        {
            *(map+j+1) = *(map+j) & (*(text+i) == *(find+j));
        }
        if (*(map+find_len) == 1)
            return i - find_len + 1;
    }
    free(map);
    return -1;
}
bool Is_two_n(int num)
{
	if (!(num&(num-1)))
	{
		printf("%d是2的n次方。\n", num);
		return true;

	}
	else
	{
		printf("%d不是2的n次方。\n", num);
	}
	return false;
}
int CreateThread(void *(*Dealfunction) (void *),void * aParm,int priority)
{
	pthread_t t;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	struct sched_param param;
	param.__sched_priority=priority;                                       //set thread priority
	pthread_attr_setschedpolicy(&attr,SCHED_RR);                     //set thread scheduler
	pthread_attr_setschedparam(&attr,&param);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);      //set pthread detach state
	pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED);      //the step is very important
	pthread_create(&t,&attr,Dealfunction,aParm);
	pthread_attr_destroy(&attr);
	return t;
}

int ReplaceStr(char * sSrc,const char * sMatchStr,const char * sReplaceStr)
{
	int StringLen;
	char caNewString[256];
	char *FindPos=strstr(sSrc,sMatchStr);
	if((!FindPos)||(!sMatchStr)) return -1;

	while(FindPos)
	{
		memset(caNewString,0,sizeof(caNewString));
		StringLen=FindPos-sSrc;
		strncpy(caNewString,sSrc,StringLen);
		strcat(caNewString,sReplaceStr);
		strcat(caNewString,FindPos+strlen(sMatchStr));
		strcpy(sSrc,caNewString);
		FindPos=strstr(sSrc,sMatchStr);
	}
	return 0;
}
int GetPrivateProfileInt(const char *key,const char * cField,int nDefault,char *filename)
{
	int nRet=nDefault;
	char str[1024],cTmp[200],*pc=NULL;
	FILE*fp=NULL;
	int nLen,len=0;
	if(!key||!cField)
	{
		return nRet;
	}
	if((fp=fopen(filename,"r"))==NULL)
	{
		return nRet;
	}
	str[0]='\0';
	sprintf(cTmp,"[%s]",key);
	while(fgets(str,1024,fp))
	{
		ReplaceStr(str," ","");
		ReplaceStr(str,"\r","");
		ReplaceStr(str,"\n","");
		len=strlen(str);
		if(len<=1)continue;
		if(strcmp(str,cTmp)!=0)
		{
			continue;
		}
		while(fgets(str,1024,fp))
		{
			ReplaceStr(str," ","");
			ReplaceStr(str,"\r","");
			ReplaceStr(str,"\n","");
			if(len<=1)
			{
				continue;
			}
			if(str[0]==';'||str[0]=='\0')
			{
				continue;
			}
			if(str[0]=='[')
			{
				break;
			}
			pc=strstr(str,"=");
			if(pc==NULL)
			{
				continue;
			}
			if((pc-str)<1)
			{
				continue;
			}
			if(strncmp(cField,str,pc-str)==0)
			{
				len=strlen(str)-1;
				if((pc-str)<len)
				{
					if(pc[2]=='x')
					{
						nLen=sscanf(pc+1,"0x%x",&nRet);
					}
					else if(pc[2]=='X')
					{
						nLen=sscanf(pc+1,"0X%x",&nRet);
					}
					else
					{
						nLen=sscanf(pc+1,"%d",&nRet);
					}
				}
				break;
			}
		}
		break;
	}
	fclose(fp);
	return nRet;
}
int GetPrivateProfileString(const char* key,const char *cField,char * cDefault,char * cRet,int dRetLen,char * filename )
{
	char str[1024],cTmp[200],*pc=NULL;
	FILE* fp=NULL;
	int len=0;
	cRet[0]=0;
	if(cDefault)
	{
		strcpy(cRet,cDefault);
	}
	if(!key||!cField)
	{
		return 0;
	}
	if((fp=fopen(filename,"r"))==NULL)
	{
		return 0;
	}
	str[0]='\0';
	sprintf(cTmp,"[%s]",key);
	while(fgets(str,1024,fp))
	{
		ReplaceStr(str," ","");
		ReplaceStr(str,"\r","");
		ReplaceStr(str,"\n","");
		len=strlen(str);
		if(len<=1) continue;
		if(str[0]==';'||str[0]=='\0')
		{
			continue;
		}
		if(strcmp(str,cTmp)!=0)
		{
			continue;
		}
		while(fgets(str,1024,fp))
		{
			ReplaceStr(str," ","");
			ReplaceStr(str,"\r","");
			ReplaceStr(str,"\n","");
			len=strlen(str);
			if(len<=1) continue;
			if(str[0]==';'||str[0]=='\0')
			{
				continue;
			}
			if(str[0]=='[')
			{
				break;
			}
			pc=strstr(str,"=");
			if(pc==NULL)
			{
				continue;
			}
			if((pc-str)<1)
			{
				continue;
			}
			if(strncmp(cField,str,pc-str)==0)
			{
				len=strlen(pc+1);
				if(len<dRetLen)
				{
					strcpy(cRet,pc+1);
				}
				else
				{
					memcpy(cRet,pc+1,dRetLen-1);
				}
				break;
			}
		}
		break;
	}
	fclose(fp);
	return (strlen(cRet)-1);
}
///////////////////////////////////////////////////////////////////
int GetFileSize(char * path,DWORD *len)
{
	int filesize=-1;
	struct stat statbuff;
	if(stat(path,&statbuff)<0)
	{
		return filesize;
	}

	filesize=statbuff.st_size;
	return filesize;
}
int FindKey(const char *cKey,char * filename)
{
	int nFind=0;
	char str[1024],cTmp[100];
	FILE*fp=NULL;
	DWORD nLen;
	DWORD nhLen;
	int len=0;
	int lastpos=0;
	if(!cKey)
	{
		return nFind;
	}
	if((fp=fopen(filename,"r"))==NULL)
	{
		return nFind;
	}
	nLen=GetFileSize(filename,&nhLen);
	if(nLen<=0)
	{
		fclose(fp);
		return nFind;
	}
	str[0]='\0';
	sprintf(cTmp,"[%s]",cKey);
	while(fgets(str,1024,fp))
	{
		len=strlen(str);
		str[len-1]=0;
		if(str[0]!=';'&&str[0]!='\0')
		{
			if(nFind==0)
			{
				if(strcmp(str,cTmp)==0)
				{
					nFind=1;
				}
			}
			else
			{
				if(str[0]=='[')
				{
					break;
				}
			}
		}
		lastpos=ftell(fp);
	}
	fclose(fp);
	return nFind;

}
void *CBuf(unsigned int nSize)
{
	char *m_pAddr;
	int m_nlen=nSize;
	m_pAddr=NULL;
	if((m_nlen)>0)
	{
		m_pAddr=(char *)malloc(nSize);
		if(!m_pAddr)
		{
			m_nlen=0;
			return NULL;
		}
		memset(m_pAddr,0,sizeof(nSize));
	}
	return (m_pAddr);
}
///////////////////////////////////////////////////////////////////
bool WritePrivateProfileInt(const char * key,const char *cField,int nValue,char * filename)
{
	FILE* fp=NULL;
	int len,newlen,hinewlen;
	char str[1024],cNewStr[200],cTmp[100],*pc=NULL;
	char *m_pAddr;
	int nFind=0;
	int nFindNextKey=0;
	char tmpstr[100];
	if(!key||!cField)
	{
		return false;
	}
	sprintf(cNewStr,"%s=%d\n",cField,nValue);
	sprintf(cTmp,"[%s]",key);
	//new
	if(!FindKey(key,filename))
	{
		if((fp=fopen(filename,"a+"))==NULL)
		{
			return false;
		}
		fprintf(fp,"\n[%s]\n",key);
		fprintf(fp,"%s",cNewStr);
		fclose(fp);
		return true;
	}
	//exist
	if((fp=fopen(filename,"r+"))==NULL)
	{
		return false;
	}
	newlen=GetFileSize(filename,(DWORD*)&hinewlen)+strlen(cNewStr)+1000;
	m_pAddr=(char*)CBuf(newlen);
	if(m_pAddr==NULL)
	{
		return false;
	}
	//key past part
	while(fgets(str,1024,fp))
	{
		len=strlen(str);
		str[len-1]=0;
		strcat(m_pAddr,str);
		strcat(m_pAddr,"\n");
		if(strcmp(str,cTmp)==0)
		{
			break;
		}
	}

	//key part
	while(fgets(str,1024,fp))
	{
		len=strlen(str);
		str[len-1]=0;
		if(str[0]=='\0')
		{
			continue;
		}
		if(str[0]=='['&&strcmp(str,cTmp))
		{
			nFindNextKey=1;
			break;
		}
		pc=strstr(str,"=");
		if(pc&&(pc-str)>=1)
		{
			memset(tmpstr,0,100);
			strncpy(tmpstr,str,pc-str);
		}

		if(pc&&(pc-str)>=1&&strcmp(cField,tmpstr)==0)
		{
			nFind=1;
			strcat(m_pAddr,cNewStr);
		}
		else
		{
			strcat(m_pAddr,str);
			strcat(m_pAddr,"\n");
		}
	}
	if(nFind==0)
	{
		strcat(m_pAddr,cNewStr);
		strcat(m_pAddr,"\n");
	}
	//key after part
	if(nFindNextKey)
	{
		strcat(m_pAddr,"\n");
		strcat(m_pAddr,str);
		strcat(m_pAddr,"\n");
		while(fgets(str,1024,fp))
		{
			len=strlen(str);
			str[len-1]=0;
			strcat(m_pAddr,str);
			strcat(m_pAddr,"\n");
		}
	}
	fclose(fp);
	//create file again
	fp=fopen(filename,"w+");
	fwrite(m_pAddr,strlen((char*)m_pAddr),1,fp);
	fclose(fp);
	free(m_pAddr);
	return true;
}
bool WritePrivateProfileString(const char * key,const char *cField,char *cValue,char * filename)
{
	FILE* fp=NULL;
	int len,newlen,hinewlen;
	char str[1024],cNewStr[200],cTmp[100],*pc=NULL;
	char *m_pAddr;
	int nFind=0;
	int nFindNextKey=0;
	char tmpstr[100];
	if(!key||!cField)
	{
		return false;
	}
	sprintf(cNewStr,"%s=%s\n",cField,cValue);
	sprintf(cTmp,"[%s]",key);
	//new
	if(!FindKey(key,filename))
	{
		if((fp=fopen(filename,"a+"))==NULL)
		{
			return false;
		}
		fprintf(fp,"\n[%s]\n",key);
		fprintf(fp,"%s",cNewStr);
		fclose(fp);
		return true;
	}
	//exist
	if((fp=fopen(filename,"r+"))==NULL)
	{
		return false;
	}
	newlen=GetFileSize(filename,(DWORD*)&hinewlen)+strlen(cNewStr)+1000;
	m_pAddr=(char*)CBuf(newlen);
	if(m_pAddr==NULL)
	{
		return false;
	}
	//key past part
	while(fgets(str,1024,fp))
	{
		len=strlen(str);
		str[len-1]=0;
		strcat(m_pAddr,str);
		strcat(m_pAddr,"\n");
		if(strcmp(str,cTmp)==0)
		{
			break;
		}
	}

	//key part
	while(fgets(str,1024,fp))
	{
		len=strlen(str);
		str[len-1]=0;
		if(str[0]=='\0')
		{
			continue;
		}
		if(str[0]=='['&&strcmp(str,cTmp))
		{
			nFindNextKey=1;
			break;
		}
		pc=strstr(str,"=");
		if(pc&&(pc-str)>=1)
		{
			memset(tmpstr,0,100);
			strncpy(tmpstr,str,pc-str);
		}

		if(pc&&(pc-str)>=1&&strcmp(cField,tmpstr)==0)
		{
			nFind=1;
			strcat(m_pAddr,cNewStr);
		}
		else
		{
			strcat(m_pAddr,str);
			strcat(m_pAddr,"\n");
		}
	}
	if(nFind==0)
	{
		strcat(m_pAddr,cNewStr);
		strcat(m_pAddr,"\n");
	}
	//key after part
	if(nFindNextKey)
	{
		strcat(m_pAddr,"\n");
		strcat(m_pAddr,str);
		strcat(m_pAddr,"\n");
		while(fgets(str,1024,fp))
		{
			len=strlen(str);
			str[len-1]=0;
			strcat(m_pAddr,str);
			strcat(m_pAddr,"\n");
		}
	}
	fclose(fp);
	//create file again
	fp=fopen(filename,"w+");
	fwrite(m_pAddr,strlen((char*)m_pAddr),1,fp);
	fclose(fp);
	free(m_pAddr);
	return true;
}
