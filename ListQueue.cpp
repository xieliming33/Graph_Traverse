/*
 * ListQueue.cpp
 *
 *  Created on: Apr 20, 2017
 *      Author: xieliming
 */
#include "ListQueue.h"
listqueue::listqueue()
{
	this->first=new qnode;
	this->first->next=NULL;
	memset(this->first->buf,0,sizeof(this->first->buf));
	this->tail=this->first;
	this->length=0;
}
listqueue::~listqueue()
{
	qnode * p=this->first;
	for(int i=0;i<=this->length;++i)
	{
		qnode * q=p->next;
		delete q;
		p=q;
	}
	this->length=0;
}
bool listqueue::addQueue(void * val,int size)
{
	qnode * p=new qnode;
	memset(p,0,sizeof(qnode));
	memcpy(p->buf,(char *)val,size);
	p->next=NULL;
	this->tail->next=p;
	this->tail=p;
	++length;
	return true;
}
bool listqueue::upQueue(void * res)
{
	if(length==0)
	{
		return false;
	}
	qnode * p=this->first->next;
	this->first->next=p->next;
	memcpy((char *)res,p->buf,sizeof(p->buf));
	delete p;
	if(length==1)
	{
		this->tail=this->first;
	}
	--length;
	return true;
}
bool listqueue::getlength()const
{
	return this->length;
}


