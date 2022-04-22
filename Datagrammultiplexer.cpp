#include<iostream.h>
#include<conio.h>
#include <stdio.h>

double creator( double appData );



/* main()
{
// (Value*100 ) + Port
double x= 1188;
double y =creator(x);
//the first location should have 11, the second should have 88 
int z= (y/1E6);	//882233
//-------------------------
int X = z/1E2;	// 8822
//-----------------------
int second = (y-(z*1E6));	// 445511
int W = second/1e2;	// 4455
//----------------------
int first = z/10000;	// 88
int sNumber = X%100; // 22
int tNumber = z%100;	// 33	
cout <<"first "<<first <<" second "<<sNumber<<" third "<<tNumber<<endl;
int sfirst = second/10000;	// 44
int ssNumber = W%100; // 55
int stNumber = second%100;	// 11	
cout <<"first "<<sfirst <<" second "<<ssNumber<<" third "<<stNumber<<endl;



//getch();
return 0;
}
*/

double creator( double appData )
{
   const int sequence=22;
	const int acknowledge=33;
	const int window=44;
	const int checksum=55;
	int data = (int) appData/100;
	int port =  (int)(appData-(data*100)); 
	double first  = port;
	double second = (sequence*100)+acknowledge;
	double third  = (window*100)+checksum;
	// PP SSAA WWCC DD, if the display is cluttered we can discard the firt portion            
	double datagram = (first* 1E10) + (second*1000000) + (third*100) + data;
  	//flag = false;
	return datagram;
}