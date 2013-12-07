/*
 * File:   NikonReader.hpp
 * Author: positron
 *
 * Created on December 7, 2013, 3:06 PM
 */

#ifndef NIKONREADER_HPP
#define	NIKONREADER_HPP

#include "Protocols.hpp"

class NikonReader : public ProtocolReader {
public:
	NikonReader(CallbackFunc ff) : ProtocolReader(ff),
		cState(0), cHalf(0) { }
	virtual ~NikonReader();

	void addPulse  (int pulse, int val);
private:
	int cState;
	int cHalf;
};

#endif	/* NIKONREADER_HPP */

