#include "EthernetController.h"
#include "Packet.h"

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

void EthernetController::init(const byte mac[6], const byte ipAddr[4], const byte dns[4], const byte gateway[4], const byte subnet[4]) {
	pinMode(4, OUTPUT);
	pinMode(53, OUTPUT);
	digitalWrite(4, LOW);
	
	Ethernet.begin((byte *) mac, ipAddr, dns, gateway, subnet);

	ready = true;
}

void EthernetController::startTcpServer(uint16_t port) {
	ethernetServer = new EthernetServer(port);
	ethernetServer->begin();
}

void EthernetController::onClientData(EthernetClient *client) {
	if (ETHERNET_DEBUG) {
		Serial.print("[ETHERNET] Client data (");
		Serial.print((int) client);
		Serial.println(");");
		Serial.print("freeMemory()=");
  		Serial.println(freeMemory());
	}

	if (clients.find(client->getSocketNumber()) == clients.end()) {
		if (ETHERNET_DEBUG) {
			Serial.print("[ETHERNET] Client (");
			Serial.print((int) client);
			Serial.println(") connected;");

			Serial.print("freeMemory()=");
  			Serial.println(freeMemory());
		}

		clients.insert(make_pair(client->getSocketNumber(), client));
	}
}

void EthernetController::loop() {
	for (auto it = clients.begin(); it != clients.end(); it++) {
		EthernetClient *client = it->second;
		if (! client->connected()) {
			if (ETHERNET_DEBUG) {
				Serial.print("[ETHERNET] Client disconnected (");
				Serial.print((int) client);
				Serial.println(");");

				Serial.print("freeMemory()=");
  				Serial.println(freeMemory());
			}

			client->stop();

			clients.erase(it);
			delete client;
			continue;
		}
	}

	EthernetClient client = ethernetServer->available();
	if (client) {
		onClientData(new EthernetClient(client.getSocketNumber()));
	}
}