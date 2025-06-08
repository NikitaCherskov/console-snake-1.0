#include "Globals.h"

int FIELD_SIZE = 16;
int SNAKE_SPEED = 2;
int HAS_BARRIER = 0;
int SKIN_NUMBER = 1;
const char* ep_ask = "You are sure exit?\n";
const char* cp_ask = "You are sure clear your save?\n";

int& getFieldSizeLink() {
	return FIELD_SIZE;
}

int& getSnakeSpeedLink() {
	return SNAKE_SPEED;
}

int& getHasBarrierLink() {
	return HAS_BARRIER;
}

int& getSkinNumberLink() {
	return SKIN_NUMBER;
}

const char* getEPAsk() {
	return ep_ask;
}
const char* getCPAsk() {
	return cp_ask;
}