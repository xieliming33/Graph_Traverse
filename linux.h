/*
 * linux.h
 *
 *  Created on: Jun 21, 2017
 *      Author: xieliming
 */

#ifndef LINUX_H_
#define LINUX_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/types.h>
#include <ctype.h>
#include <pthread.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <termios.h>
#include <sys/io.h>
#include <sys/ioctl.h>

#define PACKED __attribute__( ( packed, aligned(1) ) )
#pragma pack(1)
typedef unsigned int DWORD;

void setMousePos(unsigned int y,unsigned int x);                                            //Set Mouse Postion
void clearScreen();
int  GetKeyWord();                                                                          //检测键盘按下
int  Rand_value();                                                                          //Produce Rand Data
bool RegisteSignal(int sig,void (*handler)(int _sig));                                      //Register Handler for which signal
int  Bitap(const char *text, const char *find);                                             //Bitap算法主要利用位运算进行字符串的匹配
bool Is_two_n(int num);                                                                     //3、判断一个数是不是2 的n次方。
int CreateThread(void *(*Dealfunction) (void *),void * aParm,int priority);

int  ReplaceStr(char * sSrc,const char * sMatchStr,const char * sReplaceStr);
int  GetPrivateProfileInt(const char *key,const char * cField,int cDefault,char *filename); //.ini file operation eg:"./vxcu.ini"
int  GetPrivateProfileString(const char* key,const char *cField,char * cDefault,char * cRet,int dRetLen,char * filename );
bool WritePrivateProfileInt(const char * key,const char *cField,int nValue,char * filename);
bool WritePrivateProfileString(const char * key,const char *cField,char *cValue,char * filename);

#endif /* LINUX_H_ */
