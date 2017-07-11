#include "NetworkHandler.h"
#include "DataController.h"
#include "EthernetController.h"
#include "ModuleHandler.h"
#include "const.h"

using namespace std;

NetworkHandler::NetworkHandler(DataController *dataController, EthernetController *ethernetController, ModuleHandler *moduleHandler) {
	this->dataController = dataController;
	this->ethernetController = ethernetController;
	this->moduleHandler = moduleHandler;
}

NetworkHandler::~NetworkHandler() {
	
}

void NetworkHandler::onClientRequest(EthernetClient *client, JsonMap data) {
	if (NETWORK_DEBUG) {
		Serial.print("[NETWORK] Client request (");
		Serial.print((int) client);
		Serial.println("), reading data:");
		Serial.println("");
		Serial.println(stringifyJson(data).c_str());
		Serial.println("");

		Serial.print("freeMemory()=");
  		Serial.println(freeMemory());
	}

	RequestPacket *requestPacket = RequestPacket::fromJson(data);
	if (! requestPacket) {
		return;
	}

	moduleHandler->dispatch(client, requestPacket);
	delete requestPacket;
}

void NetworkHandler::onClientData(EthernetClient *client) {
	stringstream buffer;
	while (client->available()) {
		char c = client->read();
		buffer << c;
	}

	if (NETWORK_DEBUG) {
		Serial.print("[NETWORK] Client data (");
		Serial.print((int) client);
		Serial.println("), reading data:");
		Serial.println("");
		Serial.println(buffer.str().c_str());
		Serial.println("");

		Serial.print("freeMemory()=");
  		Serial.println(freeMemory());
	}

	JsonMap data = parseJson(buffer.str());
	onClientRequest(client, data);
}

void NetworkHandler::share(const DataPacket *dataPacket) {
	uint8_t buffer[SERIAL_TX_BUFFER_SIZE];
	buffer[0] = 1;
	
	uint8_t length = dataPacket->toByteArray(buffer + 1) + 1;
	dataController->send(buffer, length);
}

void NetworkHandler::loop(const DataPacket *dataPacket) {
	ethernetController->loop();
	moduleHandler->loop(dataPacket);

	auto clients = ethernetController->getClients();
	for (auto it = clients->begin(); it != clients->end(); it++) {
		EthernetClient *client = it->second;
		if (client->available()) {
			onClientData(client);
		}
	}
	
	share(dataPacket);
}