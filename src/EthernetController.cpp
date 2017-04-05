#include "EthernetController.h"

using namespace std;

EthernetController::EthernetController() {
	ready = false;

	ethernetServer = NULL;
}

EthernetController::~EthernetController() {
	if (ethernetServer) {
		delete ethernetServer;
	}
}

void EthernetController::init(byte mac[6], byte ipAddr[4], byte dns[4], byte gateway[4], byte subnet[4]) {
	Ethernet.begin(mac, ipAddr, dns, gateway, subnet);

	ready = true;
}

void EthernetController::startTcpServer(uint16_t port) {
	ethernetServer = new EthernetServer(port);
	ethernetServer->begin();
}

void EthernetController::broadcastData(const uint8_t *data, uint64_t length) {
	ethernetServer->write(data, length);
}