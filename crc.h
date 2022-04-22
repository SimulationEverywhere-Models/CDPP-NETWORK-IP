/*******************************************************************
*
*  DESCRIPTION: 16 bit Cyclic Redunduncy Check
*
*  AUTHOR: Abdul-Rahman Elshafei
*
*  EMAIL:
*
*  DATE: 25/1/2004
*
*******************************************************************/

#include<iostream.h>
#define divisor 98309   // => x^16+x^15+x^2+1


//to find the Frame Check Sequence for a frame
double FCS(double Message)
  {
  int x=(int)Message;
  	return ( (double)(x%divisor));
  }


//check if a frame has error
bool CRC(long Message, int FCS)
	{
   	return((Message-FCS)%divisor==0);
   }

