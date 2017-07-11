#include "EthernetController.h"
#include "DataController.h"
#include "StateController.h"
#include "ELM327Controller.h"
#include "SignalController.h"

#include "ModuleHandler.h"
#include "DefaultModule.h"
#include "DataModule.h"

#include "NetworkHandler.h"
#include "DataHandler.h"

EthernetController ethernetController;
DataController dataController;
StateController stateController;
ELM327Controller elm327Controller;
SignalController signalController;

ModuleHandler moduleHandler;
NetworkHandler networkHandler(&dataController, &ethernetController, &moduleHandler);
DataHandler dataHandler(&stateController, &elm327Controller, &signalController);

void setup() {
	// Init serial 1 port for debugging
	Serial.begin(115200);
	Serial.println("Starting arduino-vec-server...");

	moduleHandler.registerModule("default", new DefaultModule(NULL, &dataController, &stateController));
	moduleHandler.registerModule("data", new DataModule(&ethernetController));

	dataController.init(3);
	
	ethernetController.init(ETHERNET_MAC, ETHERNET_IP_ADDR, ETHERNET_GATEWAY, ETHERNET_SUBNET, ETHERNET_DNS);
	ethernetController.startTcpServer(API_PORT);

	// Init ELM327 controller on serial 2
	/*while (! elm327Controller.init(2));
	elm327Controller.setEchoOff();
	elm327Controller.autoSelectProtocol();
	elm327Controller.setAdaptiveTiming();*/

	Serial.println("Started.");
}

void loop() {
	// Read data packet from instrument panel signal and from OBD2
	//const DataPacket *packet = dataHandler.readPacket();
	const DataPacket *packet = dataHandler.generateTestPacket();

	// Broadcast data packet
	networkHandler.loop(packet);

	//Serial.print("freeMemory()=");
  	//Serial.println(freeMemory());
}