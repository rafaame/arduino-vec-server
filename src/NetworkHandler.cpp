#include "NetworkHandler.h"
#include "EthernetController.h"
#include "const.h"

using namespace std;

NetworkHandler::NetworkHandler(EthernetController *ethernetController) {
	lastTimeBroadcasted = millis();
	this->ethernetController = ethernetController;
}

NetworkHandler::~NetworkHandler() {
	
}

void NetworkHandler::broadcastPacket(const NetworkPacket *packet) {
	if (NETWORKMANAGER_DEBUG) {
		Serial.println("[NetworkHandler] Broadcasting packet");
	}

	if ((millis() - lastTimeBroadcasted) < NETWORKMANAGER_MIN_BROADCAST_INTERVAL) {
		delay(millis() - lastTimeBroadcasted);
	}

	const string data = packet->raw();
	ethernetController->broadcastData(data.c_str(), data.length());
}