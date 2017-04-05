#ifndef ETHERNETCONTROLLER_H
#define ETHERNETCONTROLLER_H

#include "common.h"
#include "Ethernet.h"

#define ETHERNET_DEBUG true

using namespace std;

class EthernetController {
private:
	bool ready;
	EthernetServer *ethernetServer;

public:
	EthernetController();
	~EthernetController();

	void init(byte mac[6], byte ipAddr[4], byte dns[4], byte gateway[4], byte subnet[4]);
	void startTcpServer(uint16_t port);
	void broadcastData(const uint8_t *data, uint64_t length);
};

#endif