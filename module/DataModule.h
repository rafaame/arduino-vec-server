#ifndef DATAMODULE_H
#define DATAMODULE_H

#include "const.h"
#include "Module.h"

using namespace std;

class DataModule : public Module {
private:
	vector<uint8_t> consumers;

public:
	DataModule(EthernetController *ethernetController = NULL, DataController *dataController = NULL, StateController *stateController = NULL, RelayController *relayController = NULL, MotorController *motorController = NULL, StepperController *stepperController = NULL);

	virtual void loop(const DataPacket *dataPacket);
	virtual void dispatch(const EthernetClient *client, const RequestPacket *requestPacket);
};

#endif