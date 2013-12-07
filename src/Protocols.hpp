/*
 * File:   Protocols.hpp
 * Author: positron
 *
 * Created on December 4, 2013, 8:40 PM
 */

#ifndef PROTOCOLS_HPP
#define	PROTOCOLS_HPP

//#include <cstdlib>
#include <algorithm>
using std::max;

class ProtocolReader;

typedef  void (*CallbackFunc)(ProtocolReader *sender, int addr, int cmd, int cmd2);

class ProtocolReader {
protected:
	static const int THRES = 180;
	inline int PULSEIN(int pulse, int dur, int thr)	{ return abs(pulse-dur)<thr; }
	inline int PULSEIN(int pulse, int dur)	{ return PULSEIN(pulse,dur, max(THRES, dur/20) ); }

	CallbackFunc callback;
public:
	ProtocolReader(CallbackFunc ff) { callback = ff; }

    virtual void addPulse  (int pulse, int val) = 0;
};


#endif	/* PROTOCOLS_HPP */

