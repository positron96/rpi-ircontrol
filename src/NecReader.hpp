/*
 * File:   NecProtocolReader.hpp
 * Author: positron
 *
 * Created on December 4, 2013, 8:37 PM
 */

#ifndef NECPROTOCOLREADER_H
#define	NECPROTOCOLREADER_H

#include "Protocols.hpp"

class NecReader : public ProtocolReader {
public:
	NecReader(CallbackFunc ff);
	void addPulse (int pulse, int val);
	virtual ~NecReader();
private:
	typedef enum {
		/** default state */
		NOTHING,
		/** state after encountering first 9ms pulse*/
		START,
		/** state after encountering 4.5ms 0 after start pulse */
		GAP,
		/** state entered in when starting to read 8 address pulses */
		ADDR,
		/** state entered when starting to read 8 address pulses */
		CMD,
		/** state entered after first 8 bits of command are read */
		CMD2,
		/** state entered in after last cmd2 bit is read and a final high value is being read */
		FIN,
		/** state entered in when encountered 2.25ms 0 after first pulse*/
		REPEAT
	} State;

	State cState;
	int cPos;
	unsigned char cCmd, cCmd2;
	unsigned char cAddr, cAddrI;

};

#endif	/* NECPROTOCOLREADER_H */

