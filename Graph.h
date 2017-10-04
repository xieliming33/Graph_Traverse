/*
 * Graph.h
 *
 *  Created on: Jun 21, 2017
 *      Author: xieliming
 */

#ifndef GRAPH_H_
#define GRAPH_H_
#include "linux.h"
#include "ListStack.h"

struct node
{
	int index;               //图中结点编号 0,1,2,3,4,.....
	double w;                //保存边权值
	bool visit_flag;         //访问标记
	node* next;              //保存下一个结点的地址
	node()
	{
		index=-1;
		w=1;
		visit_flag=false;
		next=NULL;
	}
};

class Nodelist
{
public:
	Nodelist();
	~Nodelist();
	bool addNode(unsigned int ti,double tw=1);         //每次从尾部添加一个节点
	bool deleteNode(int indexval);                     //根据结点编号来删除节点
	struct node * getFistnode()const;
	void showlist()const;
private:
	struct node* first;
	unsigned int length;
};

class GraphData
{
public:
	GraphData();
	~GraphData();
	bool initGraph();
	bool objectStr(char * ch);
	bool isExist(int * tmp,int tval,int & curindex);
	void breadthFirstsearch();                        //广度优先遍历
	void depthSub(node * p,liststack &graphStack);
	void depthFirstsearch();                          //深度优先遍历
private:
	Nodelist* nlist;                                  //保存所有节点的邻接表
	unsigned int ncnt;
};

#endif /* GRAPH_H_ */
