#ifndef RELAYCONTROLLER_H
#define RELAYCONTROLLER_H

#include "common.h"

#define RELAY_DEBUG false

#define RELAY_PIN_1 2
#define RELAY_PIN_2 3

using namespace std;

enum Relay_t {
    RELAY_1 = RELAY_PIN_1,
    RELAY_2 = RELAY_PIN_2
};

class RelayController {
private:
	bool ready;

public:
	RelayController();
	~RelayController();

	void init();
	void reset();
	
    void setHigh(Relay_t relay);
    void setLow(Relay_t relay);
};

#endif