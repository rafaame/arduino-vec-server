#ifndef ETHERNETCONTROLLER_H
#define ETHERNETCONTROLLER_H

#include "const.h"

#define ETHERNET_DEBUG true

using namespace std;

class EthernetController {
private:
	bool ready;
	EthernetServer *ethernetServer;
	std::map<uint8_t, EthernetClient *> clients;

public:
	EthernetController();
	~EthernetController();

	void init(const byte mac[6], const byte ipAddr[4], const byte dns[4], const byte gateway[4], const byte subnet[4]);
	void startTcpServer(uint16_t port);

	void onClientData(EthernetClient *client);
	void loop();

	std::map<uint8_t, EthernetClient *> *getClients() {return &clients;}
};

#endif