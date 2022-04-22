/*******************************************************************
*
*  DESCRIPTION: class Time
*
*  AUTHOR: Amir Barylko & Jorge Beyoglonian 
*
*  EMAIL: mailto://amir@dc.uba.ar
*         mailto://jbeyoglo@dc.uba.ar
*
*  DATE: 27/6/1998
*
*******************************************************************/

#ifndef __TIME_H
#define __TIME_H

/** include files **/
#include <string> // class string

/** definitions **/
typedef int Hours ;
typedef int Minutes ;
typedef int Seconds ;
typedef int MSeconds ;

class Time
{
public:
	Time( Hours h = 0, Minutes m = 0, Seconds s = 0, MSeconds ms = 0 )
		: hour( h )
		, min( m )
		, sec( s )
		, msec( ms )
		{ normalize(); }
	
	Time( const string &t )
		{ makeFrom(t); }
		
	Time( float mseconds )
		: hour( 0 )
		, min( 0 )
		, sec( 0 )
		, msec( 0 )
		{ makeFrom( mseconds ); }
	
	Time( const Time & ) ; // copy constructor

	// ** Modifiers ** //
	Time &hours( const Hours & ) ;
	Time &minutes( const Minutes & ) ;
	Time &seconds( const Seconds & ) ;
	Time &mseconds( const MSeconds & ) ;

	// ** Queries ** // 
	const Hours    &hours() const ;
	const Minutes  &minutes() const ;
	const Seconds  &seconds() const ;
	const MSeconds &mseconds() const ;

	Time operator +( const Time & ) const ;	// addition operator
	Time operator -( const Time & ) const ;	// substraction operator

	Time &operator =( const Time & ) ;			// assignment operator
	bool operator ==( const Time & ) const ;	// Equality test

	Time &operator =( const string &t )		// assignment operator
		{makeFrom(t); return *this;}
	
	Time &operator -=( const Time &t ) ;
	Time &operator +=( const Time &t ) ;

	bool operator <( const Time & ) const ;	// comparission operator 

	string asString() const ;

	float asMsecs() const
		{ return mseconds() + seconds() * 1000 + minutes() * 60000 + hours() * 3600 * 1000; }

	static const Time Zero ;
	static const Time Inf  ;

private:
	Hours hour  ;
	Minutes min ;
	Seconds sec ;
	MSeconds msec ;

	Time &makeFrom( const string & ) ;

	Time &makeFrom( float miliseconds ) ;
	Time &normalize() ;
	Time &adjust( int &, int &, int ) ;
};

/** inline methods **/
inline
const Hours &Time::hours() const
{
	return hour ;
}

inline
const Minutes &Time::minutes() const
{
	return min ;
}

inline
const Seconds &Time::seconds() const
{
	return sec ;
}

inline
const MSeconds &Time::mseconds() const
{
	return msec ;
}

inline
Time &Time::hours( const Hours &h )
{
	hour = h ;
	return *this ;
}

inline
Time &Time::minutes( const Minutes &m )
{
	min = m ;
	normalize() ;
	return *this ;
}

inline
Time &Time::seconds( const Seconds &s )
{
	sec = s ;
	normalize() ;
	return *this ;
}

inline
Time &Time::mseconds( const MSeconds &ms )
{
	msec = ms ;
	normalize() ;
	return *this ;
}

inline
Time &Time::operator -=( const Time &t )
{
	(*this) = (*this) - t ;
	return *this ;
}

inline
Time &Time::operator +=( const Time &t )
{
	(*this) = (*this) + t ;
	return *this ;
}

inline
ostream &operator <<( ostream &os, const Time &t )
{
	os << t.asString();
	return os;
}

#endif // __TIME_H
