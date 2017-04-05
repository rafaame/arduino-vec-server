#ifndef NETWORKHANDLER_H
#define NETWORKHANDLER_H

#include "common.h"
#include "Packet.h"

#define NETWORKMANAGER_DEBUG false
#define NETWORKMANAGER_MIN_BROADCAST_INTERVAL 500

class EthernetController;

using namespace std;

class NetworkHandler {
private:
	uint64_t lastTimeBroadcasted;
	EthernetController *ethernetController;

public:
	NetworkHandler(EthernetController *ethernetController);
	~NetworkHandler();

	void broadcastPacket(const NetworkPacket *packet);
};

#endif