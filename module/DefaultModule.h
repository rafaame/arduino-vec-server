#ifndef DEFAULTMODULE_H
#define DEFAULTMODULE_H

#include "const.h"
#include "Module.h"

using namespace std;

class DefaultModule : public Module {
private:


public:
	DefaultModule(EthernetController *ethernetController = NULL, DataController *dataController = NULL, StateController *stateController = NULL, RelayController *relayController = NULL, MotorController *motorController = NULL, StepperController *stepperController = NULL);

	virtual void dispatch(const EthernetClient *client, const RequestPacket *requestPacket);
};

#endif