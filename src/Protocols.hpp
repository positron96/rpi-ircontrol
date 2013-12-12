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
	static const int THRES_ = 250;
	inline int THRES(int d) { return max(THRES_, d/10);}
	inline int PULSEIN(int pulse, int dur, int thr)	{ return abs(pulse-dur)<thr; }
	inline int PULSEIN(int pulse, int dur)	{ return PULSEIN(pulse,dur, THRES(dur) ); }
	inline int PULSEINRANGE(int pulse, int lo, int hi)	{ return pulse<hi && pulse>lo; }

	CallbackFunc callback;
public:
	ProtocolReader(CallbackFunc ff) { callback = ff; }

    virtual void addPulse  (int pulse, int val) = 0;
};


#endif	/* PROTOCOLS_HPP */

