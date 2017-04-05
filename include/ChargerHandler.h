#ifndef CHARGERHANDLER_H
#define CHARGERHANDLER_H

#include "common.h"

#define CHARGER_DEBUG false

#define TIME_CHARGER_ON (1 * 60 * 60)
#define TIME_CHARGER_OFF (4 * 60 * 60)

using namespace std;

class RelayController;

class ChargerHandler {
private:
	bool charging;
    uint64_t lastTimeChanged;

    RelayController *relayController;

public:
	ChargerHandler(RelayController *relayController);
	~ChargerHandler();

    void init();
	void reset();

    void update();

    void turnChargingOn();
    void turnChargingOff();
};

#endif