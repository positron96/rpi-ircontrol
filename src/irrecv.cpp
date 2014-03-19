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
#include <vector>
#include <typeinfo>

#include "Protocols.hpp"
#include "NecReader.hpp"
#include "NikonReader.hpp"

using namespace std;

#define PIN 1

int last;

int lastCmd=0;
int lastAddr=0;
int haveCmd = false;
ProtocolReader *lastSender;

void cmdReceived(ProtocolReader *sender, int addr, int cmd1, int cmd2) {

	lastCmd = cmd1;
	lastAddr = addr;
	haveCmd = true;
	lastSender = sender;
	//cout<<"callback called cmd1="<<cmd1<<" lastCmd="<<lastCmd<<endl;
};


vector< ProtocolReader* > handlers;
//ProtocolReader reader(&cmdReceived);

void intr() {
    int v = digitalRead(PIN);
    int cur = micros();

	for(vector<ProtocolReader* >::iterator it = handlers.begin();
			it != handlers.end(); ++it) {
		(*it)->addPulse(cur-last, v);
	}
    last = cur;
}

int main(int argc, char** argv) {
	//freopen("irrecv.log.txt", "w", stdout);
	if (wiringPiSetup() == -1)
		exit(1);

	handlers.push_back( new NecReader(&cmdReceived) );
	handlers.push_back( new NikonReader(&cmdReceived) );

	last = micros();
	wiringPiISR(PIN, INT_EDGE_BOTH, &intr);

	pinMode(PIN, INPUT);

	while (1) {
		if(haveCmd) {
			//cout<<"Got cmd="<<lastCmd<<" addr="<<lastAddr<<" from "<<typeid(*lastSender).name()<<"; switching led"<<endl;

			char msg[255];
			//sprintf(cmd, "./lightcontrol -m \"IR beacon (addr=%d cmd=%d from %s)\"", lastAddr, lastCmd, typeid(*lastSender).name() );
			sprintf(msg, "IR. %d %d %s\n" , lastAddr, lastCmd, static_cast<ProtocolReader*>(lastSender)->tag.c_str());//typeid(*lastSender).name());
#ifndef DEBUG
			//system(cmd);
			const char* pipe = argc>1 ? argv[1] : "missioninput";
			FILE* fPipe = fopen(pipe, "a");
			if(fPipe==0) {
				fprintf(stderr, "error opening pipe");
			} else {
				fprintf(fPipe, msg);
				fclose(fPipe);
			}
#else
			printf(msg);
#endif
			haveCmd = false;
			lastSender = NULL;

		}
		delay(50);
	}

	return 0;
}

