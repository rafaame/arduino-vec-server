#ifndef NETWORKHANDLER_H
#define NETWORKHANDLER_H

#include "const.h"
#include "Packet.h"

#define NETWORK_DEBUG true

class DataController;
class EthernetController;
class ModuleHandler;

using namespace std;

class NetworkHandler {
private:
	DataController *dataController;
	EthernetController *ethernetController;
	ModuleHandler *moduleHandler;

public:
	NetworkHandler(DataController *dataController, EthernetController *ethernetController, ModuleHandler *moduleHandler);
	~NetworkHandler();

	void onClientRequest(EthernetClient *client, JsonMap data);
	void onClientData(EthernetClient *client);
	void share(const DataPacket *dataPacket);
	void loop(const DataPacket *dataPacket);
};

#endif