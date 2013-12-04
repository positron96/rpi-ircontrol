/*
 * File:   NecProtocolReader.cpp
 * Author: positron
 *
 * Created on December 4, 2013, 8:37 PM
 */

#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <cstring>

#include "NecProtocolReader.hpp"

#define printf(fmt, ...) (0)

const int THRES = 300;
inline int PULSEIN(int pulse, int dur)	{ return abs(pulse-dur)<THRES; }


/** Prints current date&time (used for debug logging) */
char* dt() {
	time_t tt = time(0);
	char *v = ctime(&tt);
	v[strlen(v)-1]=0;
	return v;
}

NecProtocolReader::NecProtocolReader(CallbackFunc ff):ProtocolReader(ff),
	cState(NOTHING), cPos(0) {
	//printf("NecProtocolReader()\n");
}

void NecProtocolReader::addPulse (int pulse, int val) {
	//std::cout<<"NecProtocolReader::addPulse "<<val<<" "<<pulse<<std::endl;
/*	if(cState==NOTHING)
		printf("%s: GOT %d for %dus, state is %d\n", dt(),val, pulse, cState);
	else
		printf("%s: GOT %d for %dus, state is %d; pos=%d, addr/~addr=%d/%d,  cmd/cmd2=%d/%d\n", dt(),
			val, pulse, cState, cPos, cAddr, cAddrI, cCmd, cCmd2);
 */
	switch(cState) {
		case NOTHING:
			if(val==1 && PULSEIN(pulse,9000)) {
				printf("switching to START\n");
				cState=START;
				return;
			}
			break;
		case START:
			if(val==0 && PULSEIN(pulse,4500)) {
				printf("switching to GAP\n");
				cState=GAP;
				return;
			} else
			if(val==0 && PULSEIN(pulse,2250)) {
				printf("switching to REPEAT\n");
				cState=REPEAT;
				return;
			}
			break;
		case REPEAT:
			if(val==1) {
				printf("REPEAT completed\n");
				//return 2;
				//lastCmd=ccmd
				cState=NOTHING;
				return;
			}
			break;
		case GAP:
			if(val==1) {
				printf("switching to ADDR\n");
				cPos = 0;
				cAddr = 0;
				cAddrI = 0;
				cState = ADDR;
				return;
			}
			break;
		case ADDR:
			if(cPos<16 && val==0) {
				if(cPos<8) if(pulse>1000) cAddr |= 1<<cPos;
				if(cPos>=8) if(pulse>1000) cAddrI |= 1<<(cPos-8);
				cPos++;
			}
			if(cPos==16) {
				if(cAddr != (char)~cAddrI ) {
					printf("address validation failed: %1$d(%1$o) != %2$d(%2$o)\n", cAddr, (char)~cAddrI);
					printf("switching to NOTHING\n");
					cState = NOTHING;
					return;
				}
				printf("Got address %o (validated)\n", cAddr);
				printf("switching to CMD\n");
				cPos=0;
				cCmd=0;
				cState = CMD;
				return;
			}
			if(val==0 && PULSEIN(pulse, 562)) return;
			if(val==0 && PULSEIN(pulse, 1687)) return;
			if(val==1 && PULSEIN(pulse, 562) ) return;
			break;
		case CMD:
			if(cPos<8 && val==0) {
				if(pulse>1000) cCmd |= 1<<cPos;
				cPos++;
			}
			if(cPos==8) {
				printf("switching to CMD2\n");
				cPos = 0;
				cCmd2=0;
				cState = CMD2;
				return;
			}
			if(val==0 && PULSEIN(pulse, 562)) return;
			if(val==0 && PULSEIN(pulse, 1687)) return;
			if(val==1 && PULSEIN(pulse, 560) ) return;
			break;
		case CMD2:
			if(cPos<8 && val==0) {
				if(pulse>1000) cCmd2 |= 1<<cPos;
				cPos++;
			}
			if (cPos==8) {
				printf("switching to FIN\n");
				cState = FIN;
				return;
			}
			if(val==0 && PULSEIN(pulse, 562)) return;
			if(val==0 && PULSEIN(pulse, 1687)) return;
			if(val==1 && PULSEIN(pulse, 560) ) return;
			break;
		case FIN:
			if(val==1 && PULSEIN(pulse, 500)) {
				printf("SUCCESS addr %d cmd %d (%2$o), cmd2 %3$d (~%4$o)\n", cAddr, cCmd, cCmd2, (char)~cCmd2);
				printf("switching to NOTHING\n");
				callback(this, cAddr, cCmd, cCmd2);
				//lastCmd=cCmd | cCmd2<<8;
				cState=NOTHING;
				return;
			}
			break;
	}
	cState = NOTHING;
}

NecProtocolReader::~NecProtocolReader() {
	//printf("~NecProtocolReader()\n");
}

