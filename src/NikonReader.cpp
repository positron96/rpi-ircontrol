/*
 * File:   NikonReader.cpp
 * Author: positron
 *
 * Created on December 7, 2013, 3:06 PM
 */

#include "NikonReader.hpp"
#include <cstdio>

NikonReader::~NikonReader() {
}

void NikonReader::addPulse(int pulse, int val) {
	//printf("GOT %d for %dus, state is %d; half=%d\n", val, pulse, cState, cHalf);
	switch (cState) {
		case 0:
			if (val == 1 && PULSEIN(pulse, 2000)) {
				cState = 1;
				return;
			}
			break;
		case 1:
			if (val == 0 && PULSEIN(pulse, 28000, 1000)) {
				cState = 2;
				return;
			}
			break;
		case 2:
			if (val == 1 && PULSEIN(pulse, 400)) {
				cState = 3;
				return;
			}
			break;
		case 3:
			if (val == 0 && PULSEIN(pulse, 1580)) {
				cState = 4;
				return;
			}
			break;
		case 4:
			if (val == 1 && PULSEIN(pulse, 400)) {
				cState = 5;
				return;
			}
			break;
		case 5:
			if (val == 0 && PULSEIN(pulse, 3580)) {
				cState = 6;
				return;
			}
			break;
		case 6:
			if (val == 1 && PULSEIN(pulse, 400)) {
				cState = 7;
				if(cHalf == 1) {
					callback(this, 0, 1, 0);
					cHalf = 0;
					cState = 0;
				}
				return;
			}
			break;
		case 7:
			if(val==0 && PULSEIN(pulse, 60000)) {
				cState = 0;
				cHalf++;
				return;
			}
	}
	cState = 0;
	cHalf = 0;

}

