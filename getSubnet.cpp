#include "iostream.h"

void getSubnet(long double ip, long double mask)
{

	int complete, incomplete, last_byte, i;
	//-------------------------------------
	complete = (int)mask/8;
	cout<<"# of complete bytes"<<complete<<"\n";
	incomplete = (int)mask%8;
	cout<<"# of incomplete bytes"<<incomplete<<"\n";


	//-------------------------------------
	int subnet[4], addresses[4], v=(int) (ip/1E6);
	long y = (ip-(v*1E6));
	//-------------------------------------
	addresses[0] = (int) (ip/1E9);
	cout << " byte 1 :"<<addresses[0]<<"\n";
	addresses[1] = (int) (ip/1E6)%1000;
	cout << " byte 2 :"<<addresses[1]<<"\n";
	addresses[2] = (int) (y/1000)%1000;
	cout << " byte 3 :"<<addresses[2]<<"\n";
	addresses[3] = (int) (y%1000);									// this is the host count.
	cout << " byte 4 :"<<addresses[3]<<"\n";
	//-------------------------------------
	// this step assumes we won't get any class A subnets, i.e the mask can't be less than 8.
	for(int i = 0; i < complete; i++)
	{
		subnet[i] = addresses[i];
		last_byte = i;	// last location we have written useful data
	}
	//-------------------------------------
		cout<<"last byte is :"<<last_byte<<"\n";
	if(last_byte<3)
	{
		long temp;
		int bit = 1;
		temp = complete * 8;
		temp = mask - temp;
		if( (temp<=7) && (temp>=4) )
		{
			bit = 5;
		} else if( (temp <= 15) && (temp >=8) )
		{
					bit = 4;
		} else if ( (temp <=31) && (temp >=16) )
		{
						bit = 3;
		}
		temp=temp<<bit;


		long adtemp = addresses[last_byte+1];
		subnet[last_byte+1] = temp & adtemp;

	}

	if(last_byte+1 != 3)
	{
		subnet[last_byte+2] = 0;
	}

	if(last_byte+2 != 3)
	{
		subnet[last_byte +3] = 0;
	}

	for(i=0; i<4;i++)
	{
		cout <<"subnet: "<<i<<" = "<<subnet[i]<<"\n";
	}
	//-------------------------------------


}

/*
void main (void)
{
	
	long double ip;
	long double mask;
	cout << "enter IP to operate on \n";
	cin  >> ip;
	cout << " you entered: "<<ip<<"\n";
	cout << "enter mask value to use \n";
	cin  >> mask;
	cout << " you entered: " <<mask<<"\n";
	getSubnet(ip, mask);
}


*/