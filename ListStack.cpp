/*
 * ListStack.cpp
 *
 *  Created on: Apr 20, 2017
 *      Author: xieliming
 */
#include "ListStack.h"
liststack::liststack()
{
	this->first=new snode;
	this->first->next=NULL;
	memset(this->first->buf,0,BUF_SIZE);
	this->tail=this->first;
	this->length=0;
}
liststack::~liststack()
{
	snode * p=this->first;
	for(int i=0;i<=length;++i)
	{
		snode * q=p;
		p=p->next;
		delete q;
	}
}
bool liststack::downstack(void * val,int size)
{
	snode * p=new snode;
	memset(p,0,sizeof(snode));
	memcpy(p->buf,(char*)val,size);
	p->next=NULL;
	this->tail->next=p;
	this->tail=p;
	++length;
	return true;
}
bool liststack::upstack(void *res)
{
	if(this->length==0)
	{
		return false;
	}
	snode * p=this->first;
	for(int i=0;i<length-1;++i)               //this is very important
	{
		p=p->next;
	}
	snode * q=p->next;
	memcpy((char*)res,tail->buf,strlen(q->buf)+1);
	this->tail=p;
	this->tail->next=NULL;
	delete q;
	--length;
	return true;
}
int liststack::getlength()const
{
	return this->length;
}
