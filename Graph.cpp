/*
 * Graph.cpp
 *
 *  Created on: Jun 21, 2017
 *      Author: xieliming
 */
#include "Graph.h"
#include "ListStack.h"
#include "ListQueue.h"

Nodelist::Nodelist()
{
	this->length      =0;
	this->first       =new node;
	this->first->index=-1;
	this->first->w    =0;
	this->first->visit_flag=false;
	this->first->next =NULL;
}

Nodelist::~Nodelist()
{
	node * p=this->first,*q=NULL;
	while(p->next!=NULL)
	{
		q=p;
		p=p->next;
		delete q;
	}
	delete p;
	this->length=0;
}

bool Nodelist::addNode(unsigned int ti,double tw)
{
	node * tp=this->first;                      //结点编号不能重复
	while(tp->next!=NULL)
	{
		if(tp->index==(int)ti) return false;
		tp=tp->next;
	}
	if(tp->index==(int)ti) return false;       //add node in teil

	node * p   =new node;
	p->index   =ti;
	p->w       =tw;
	p->visit_flag=false;
	p->next    =tp->next;
	tp->next   =p;
	++this->length;
	return true;
}
bool Nodelist::deleteNode(int indexval)         //delete the node by indexval
{
	node * p=this->first,*q=NULL;
	while(p->next!=NULL)
	{
		q=p->next;
		if(q->index == indexval) break;
		p=p->next;
	}
	p->next=q->next;
	delete q;
	--this->length;
	return true;
}
struct node * Nodelist::getFistnode()const
{
	return this->first;
}
void Nodelist::showlist()const
{
	node * p=this->first;
	while(p->next!=NULL)
	{
		if(p->index!=-1)
			printf("index=:%d ",p->index);
		p=p->next;
	}
	printf("index=:%d\n",p->index);
}

GraphData::GraphData()
{
	this->nlist=NULL;
	this->ncnt =0;
}
GraphData::~GraphData()
{
	node * p=NULL,*q=NULL;
	if(this->nlist!=NULL&&ncnt!=0)
	{
		for(unsigned int i=0;i<ncnt;++i)
		{
			p=NULL;
			q=NULL;
			p=((this->nlist+i)->getFistnode());
			while(p->next!=NULL)
			{
				q=p;
				p=p->next;
				delete q;
			}
			delete p;
		}
	}
}

bool GraphData::objectStr(char * ch)                  //判断输入合法性 '0'~'9'
{
	int n=strlen(ch);
	while(0x2f<*(ch+n-1)&&*(ch+n-1)<':') --n;
	if(n)
	{
		printf("Enter data is error %d!\n",n);
		return false;
	}
	int t=atoi(ch);
	if(t>(int)(this->ncnt-1))
	{
		printf("Enter data Out Max Range!\n");
		return false;
	}
	return true;
}

bool GraphData::isExist(int * tmp,int tval,int & curindex)
{
	for(unsigned int i=0;i<this->ncnt;++i)
	{
		if(*(tmp+i)==tval) return true;
	}
	*(tmp+curindex)=tval;
	++curindex;
	return false;
}

char getKeyCh()                           //获取键盘输入
{
	char a=GetKeyWord();
	if(a<'1'||a>'3')
		return getKeyCh();
	return a;
}

bool GraphData::initGraph()              //初始化图
{
	setMousePos(1,1);
	printf("Please Enter Node Num :\n");
	scanf("%d",&this->ncnt);
	if(this->ncnt<0) return false;
	this->nlist=new Nodelist[this->ncnt];
	int *tmp=new int[this->ncnt];
	int curindex=0;
	for(unsigned int i=0;i<this->ncnt;++i)
	{
		*(tmp+i)=-1;
		(this->nlist+i)->addNode(i);
	}

	printf("Please Enter Every Node Index and Adjacent with space separates Key # End:\n");
	for(unsigned int i=0;i<ncnt;++i)
	{
		printf("%d \n",i);
	}
	int _cnt=0,sx=5,sy=4;                      //_cnt 记录 # 数量 and 保存 enter 邻接点的 index
	char ch[8]={0};
	while(1)
	{
		if(_cnt>(int)this->ncnt-1) break;
		setMousePos(sy,sx);
		memset(ch,0,8);
		scanf("%s",ch);
		if(strcmp(ch,"#")==0)
		{
			sy++;
			sx=5;
			_cnt++;
			for(unsigned int i=0;i<this->ncnt;++i) *(tmp+i)=-1;
			curindex=0;
			continue;
		}
		if(objectStr(ch))
		{
			int k=atoi(ch);
			if(isExist(tmp,k,curindex)||k==_cnt)
			{
				printf("the node is exist!\n");
				exit(1);
			}
			(this->nlist+_cnt)->addNode(k);
			sx+=4;
		}
	}
	printf("Enter Graph End..........!\n");
	for(unsigned int i=0;i<this->ncnt;++i)
		(this->nlist+i)->showlist();
	printf("Please Select Operation:\n\n");
	printf("\t\t1.BreadthFirstSearch.\n\n");
	printf("\t\t2.DepthFirstSearch.\n\n");
	printf("\t\t3.Exit Run.\n\n");
	printf("Wait You Key............\n");

	char a=getKeyCh();
	switch(a)
	{
		case '1':
		{
			this->breadthFirstsearch();
			break;
		}
		case '2':
		{
			this->depthFirstsearch();
			break;
		}
		case '3':
		{
			printf("Prograss was exit successful!\n");
			exit(0);
			break;
		}
	}
	while(1)
	{
		node * tp=NULL;                            //Init Graph Node visit_flag is to false
		for(unsigned int i=0;i<this->ncnt;++i)
		{
			tp=(this->nlist+i)->getFistnode();
			while(tp->next!=NULL)
			{
				tp=tp->next;
				tp->visit_flag=false;
			}
		}
		printf("\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
		printf("$The Progress Run Or No Opt 1 2 3 ?$");
		printf("\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
		char a=getKeyCh();
		switch(a)
		{
			case '1':
			{
				this->breadthFirstsearch();
				break;
			}
			case '2':
			{
				this->depthFirstsearch();
				break;
			}
			case '3':
			{
				printf("Prograss was exit successful!\n");
				exit(0);
				break;
			}
		}
	}
	return true;
}
void GraphData::breadthFirstsearch()
{
	printf("BreadthFirstSearch:");
	unsigned int sn=Rand_value()%this->ncnt;
	listqueue graphQueue;
	node *pos=NULL,*q=NULL,* p=(this->nlist+sn)->getFistnode()->next;
	if(!p->visit_flag)
	{
		printf("%d ",p->index);
		p->visit_flag=true;
	}
	while(p->next!=NULL)
	{
		q=NULL;
		pos=NULL;
		q=p->next;
		pos=(this->nlist+q->index)->getFistnode();
		pos=pos->next;
		if(!pos->visit_flag)
		{
			printf("%d ",q->index);
			pos->visit_flag=true;
			graphQueue.addQueue(&q->index,sizeof(int));      //被访问的节点入队列
		}
		p=p->next;
	}
	char buf[1024]={0};
	while(graphQueue.getlength()>0)
	{
		memset(buf,0,sizeof(buf));
		graphQueue.upQueue(buf);                      //出队列
		int tn=*(int *)(buf);
		node * p=(this->nlist+tn)->getFistnode()->next;
		if(!p->visit_flag)
		{
			printf("%d ",p->index);
			p->visit_flag=true;
		}
		q=NULL;
		while(p->next!=NULL)                      //依次访问本层次未被访问的节点
		{
			q=NULL;
			q=p->next;
			pos=NULL;
			pos=(this->nlist+q->index)->getFistnode()->next;
			if(!pos->visit_flag)
			{
				printf("%d ",q->index);
				pos->visit_flag=true;
				graphQueue.addQueue(&q->index,sizeof(int));
			}
			p=p->next;
		}
	}
	printf("\n");
}
void GraphData::depthSub(node * p,liststack &graphStack)
{
	char buf[1024]={0};
	node *q=NULL;
	bool r_flag=false;
	do                                                           //查找第一个未被访问的节点
	{
		if(p->next==NULL)
		{
			r_flag=true;
			break;
		}
		p=p->next;
	}while((this->nlist+p->index)->getFistnode()->next->visit_flag);
	if(!r_flag)                                                     //如果存在未被访问的节点
	{
		q=p;
		if(!(this->nlist+q->index)->getFistnode()->next->visit_flag)
		{
			printf("%d ",q->index);
			(this->nlist+q->index)->getFistnode()->next->visit_flag=true;
			graphStack.downstack(&q->index,sizeof(int));           //所有访问的节点都要入Stack
		}
		p=(this->nlist+q->index)->getFistnode()->next;             //跳转到节点的对应NodeList
		if(p->next!=NULL)                                          //未到达NULL
		{
			this->depthSub(p,graphStack);
		}
	}

	if(graphStack.getlength()>0)
	{
		memset(buf,0,1024);
		graphStack.upstack(buf);
		int tn=*(int*)buf;
		node *tq=NULL,* tp=(this->nlist+tn)->getFistnode();
		while(tp->next!=NULL)
		{
			tq=tp->next;
			if(!((this->nlist+tq->index)->getFistnode()->next->visit_flag))   //如果本链表还存在没有被访问的节点则继续进Stack
			{
				graphStack.downstack(&(this->nlist+tn)->getFistnode()->next->index,sizeof(int));
				break;
			}
			tp=tp->next;
		}
		this->depthSub((this->nlist+tn)->getFistnode()->next,graphStack);    //递归
	}
}

void GraphData::depthFirstsearch()
{
	printf("DedthFirstSearch:");
	unsigned int sn=Rand_value()%this->ncnt;
	liststack graphStack;
	node * p=(this->nlist+sn)->getFistnode()->next;                         //访问随机选中的节点
	printf("%d ",p->index);
	graphStack.downstack(&p->index,sizeof(int));
	p->visit_flag=true;
	this->depthSub(p,graphStack);
	printf("\n");
}

