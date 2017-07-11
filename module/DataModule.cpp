#include "DataModule.h"
#include "EthernetController.h"

DataModule::DataModule(EthernetController *ethernetController, DataController *dataController, StateController *stateController, RelayController *relayController, MotorController *motorController, StepperController *stepperController) :
	Module(ethernetController, dataController, stateController, relayController, motorController, stepperController) {
	
}

void DataModule::loop(const DataPacket *dataPacket) {
	string data = dataPacket->toJson();
	for (auto it = consumers.begin(); it != consumers.end();) {
		uint8_t socketNumber = *it;

		auto cit = ethernetController->getClients()->find(socketNumber);
		if (cit == ethernetController->getClients()->end() || ! cit->second->connected()) {
			it = consumers.erase(it);
			continue;
		}

		EthernetClient *client = cit->second;
		client->print(data.c_str());
		client->flush();

		it++;
	}
}

void DataModule::dispatch(const EthernetClient *client, const RequestPacket *requestPacket) {
	if (requestPacket->action == "attach") {
		consumers.push_back(client->getSocketNumber());
	}
}