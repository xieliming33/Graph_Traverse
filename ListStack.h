/*
 * ListStack.h
 *
 *  Created on: Apr 20, 2017
 *      Author: xieliming
 */

#ifndef LISTSTACK_H_
#define LISTSTACK_H_
#include "linux.h"
#define BUF_SIZE 50

#define PACKED __attribute__( ( packed, aligned(1) ) )
#pragma pack(1)

struct snode
{
	char buf[BUF_SIZE];
	snode * next;
};

class liststack
{
public:
	liststack();
	~liststack();
	bool downstack(void * val,int size);
	bool upstack(void *res);
	int getlength()const;
private:
	snode * first;
	snode * tail;
	int length;
};
#endif /* LISTSTACK_H_ */
