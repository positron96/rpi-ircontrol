/*
 * File:   Protocols.hpp
 * Author: positron
 *
 * Created on December 4, 2013, 8:40 PM
 */

#ifndef PROTOCOLS_HPP
#define	PROTOCOLS_HPP

class ProtocolReader;

typedef  void (*CallbackFunc)(ProtocolReader *sender, int addr, int cmd, int cmd2);

class ProtocolReader {
protected:
	CallbackFunc callback;
public:
	ProtocolReader(CallbackFunc ff) { callback = ff; }

    virtual void addPulse  (int pulse, int val) = 0;
};


#endif	/* PROTOCOLS_HPP */

