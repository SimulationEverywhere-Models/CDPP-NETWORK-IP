/*******************************************************************
*
*  DESCRIPTION: Dijkstras Algorithm Function 
*
*  AUTHOR: Abdul-Rahman Elshafei 
*
*  EMAIL: 
*
*  DATE: 4/1/2004
*
*******************************************************************/

/** include files **/
#include<stdio.h>
#include<iostream.h>
#include<list>
using namespace std;

//maximum metric size allowed
#define INFINITY 16
#define NumberOfNodes 100


typedef int MetricTable[NumberOfNodes];
typedef int ConnectionTable[NumberOfNodes][NumberOfNodes];


void Distance(int n, ConnectionTable cost, MetricTable D, MetricTable m)
  {
	//Has connection from 0 to v found, true if v<{S}
	bool final[100];
	//counter for the main loop
	int i;
	//vertex not yet added to {S}
	int w;
	//initialize with vertex 0 alone in {S}
	int v;
	int min,g;
	int flag=0;

	//the stack contains the shortest route from base node to every node
	//in the network. 'next' indicates the best next interface to follow.
	list<int> *next;
	next=new list<int>[n];

	//initializing next, final and D
	for(v=0;v<n;v++)
	  {
		final[v]=false;
		D[v]=cost[0][v];
		//-----------
		if(cost[0][v]!=16)
			next[v].push_back(v);
	  }
	final[0]=true;
	//looping through all other vertices inorder to find min distance
	//from base to node i
	for(i=1; i<n; i++)
	  {
		//initialize the min metric size to i
		min=INFINITY;
		//find current min
		for(w=1;w<n;w++)
		  {
			if(!final[w])
			   {
				if(D[w]<min)
				  {
					v=w;
					min=D[w];
				  }
			  }
		  }
		//node v has found its shortest metric
		final[v]=true;
		//find alternate route to other nodes via node v
		for(w=1;w<n;w++)
		  {
			if(!final[w])
			  {
				if(min + cost[v][w] < D[w])
				{   D[w] = min + cost[v][w];
					next[w].push_back(v); }
			  }
		  }
	  }
	
	/** Shortest metric is found for every node,
	 ** however next interface for every node needs to be adjusted
	 **/
	//g is a temp variable used to avoid a bug in iteration
	g=next[n-1].size();
	//looping through the stack in order to get the first interface
	for(i=0;i<n;i++)
	{	
		if(i==(n-1)) //this extra loop used to avoid a bug
		{ for(w=0;w<g;w++)
			{
			m[i]=next[i].back();
			next[i].pop_back();
			}
		}
		else {
		for(w=0;w<next[i].size();w++)
		{
			m[i]=next[i].back();
			next[i].pop_back();
		}
		}
	}
	//loop is to ensure next interface belongs connected to base node 0 
	for(i=0;i<n;i++)
	{
		if(cost[0][m[i]]==16)
		{ 
			flag=m[i];
			m[i]=m[flag];
		}
	}

  }

/*//----------------------------------------
int main(void)
  {
    ConnectionTable x;
    MetricTable y,z;
	 char garbage;
    int cnt,cnt2;
    for(cnt=0;cnt<6;cnt++)
      {
        for(cnt2=0;cnt2<6;cnt2++)
          x[cnt][cnt2]=INFINITY;
      }
    for(cnt=0;cnt<6;cnt++)
      x[cnt][cnt]=0;
    x[0][1]=2;
    x[0][2]=1;
    x[0][3]=5;
    x[1][0]=3;
    x[1][2]=2;
    x[1][3]=3;
    x[2][0]=7;
    x[2][1]=2;
    x[2][3]=3;
    x[2][4]=1;
    x[3][0]=8;
	 x[3][1]=6;
    x[3][2]=3;
    x[3][4]=1;
    x[3][5]=5;
    x[4][3]=1;
    x[4][2]=1;
    x[4][5]=2;
    x[5][3]=8;
    x[5][4]=4;
    Distance(6,x,y,z);
    for(cnt=0;cnt<6;cnt++)
      cout<<y[cnt]<<endl;
	cout<<endl;
	for(cnt=0;cnt<6;cnt++)
      cout<<z[cnt]<<endl;
    cin>>garbage;
    return 0;
  }   */






