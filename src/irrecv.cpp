/*
 * File:   irrecv.cpp
 * Author: positron
 *
 * Created on December 4, 2013, 8:32 PM
 */

#include <wiringPi.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>

#include "Protocols.hpp"
#include "NecProtocolReader.hpp"

using namespace std;

#define PIN 1

int last;

int lastCmd=0;

void cmdReceived(ProtocolReader *sender, int addr, int cmd1, int cmd2) {

	lastCmd = cmd1;
	//cout<<"callback called cmd1="<<cmd1<<" lastCmd="<<lastCmd<<endl;
};

NecProtocolReader reader(&cmdReceived);

void intr() {
//    if(cpulse>=NPULSES) return;
    int v = digitalRead(PIN);
    int cur = micros();
	reader.addPulse(cur-last, v );
    //addCmd(cur-last, digitalRead(PIN) );
    //pulses[cpulse++] = (cur-last) ;
    last = cur;
}

int main(int argc, char** argv) {
	//freopen("irrecv.log.txt", "w", stdout);
	if (wiringPiSetup() == -1)
		exit(1);

	last = micros();
	wiringPiISR(PIN, INT_EDGE_BOTH, &intr);

	pinMode(PIN, INPUT);

	while (1) {
		if(lastCmd!=0) {
			cout<<"Got cmd "<<lastCmd<<"; switching led"<<endl;

			char cmd[255];
			sprintf(cmd, "./lightcontrol -m \"IR beacon cmd %d\"", lastCmd);
			system(cmd);
			lastCmd = 0;

		}
		delay(50);
	}

	return 0;
}

