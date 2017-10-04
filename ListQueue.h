/*
 * ListQueue.h
 *
 *  Created on: Apr 20, 2017
 *      Author: xieliming
 */

#ifndef LISTQUEUE_H_
#define LISTQUEUE_H_
#include "linux.h"
#define BUF_SIZE 50

#define PACKED __attribute__( ( packed, aligned(1) ) )
#pragma pack(1)

struct qnode
{
	char buf[BUF_SIZE];
	qnode * next;
};

class listqueue
{
public:
	listqueue();
	~listqueue();
	bool addQueue(void * val,int size);
	bool upQueue(void * res);
	bool getlength()const;
private:
	qnode * first;
	qnode * tail;
	int length;
};

#endif /* LISTQUEUE_H_ */
