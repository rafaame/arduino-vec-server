#include "EthernetController.h"
#include "ELM327Controller.h"
#include "SignalController.h"
#include "NetworkHandler.h"
#include "DataHandler.h"

EthernetController ethernetController;
ELM327Controller elm327Controller;
SignalController signalController;

NetworkHandler networkHandler(&ethernetController);
DataHandler dataHandler(&elm327Controller, &signalController);

void setup() {
	// Init serial 1 port for debugging
	Serial.begin(115200);
	Serial.println("Starting arduino-vec-server...");

	ethernetController.init(ETHERNET_MAC, ETHERNET_IP_ADDR, ETHERNET_GATEWAY, ETHERNET_SUBNET, ETHERNET_DNS);
	ethernetController.startTcpServer(DATA_REMOTE_PORT);

	// Init ELM327 controller on serial 2
	while (! elm327Controller.init(2));
	elm327Controller.setEchoOff();
	elm327Controller.autoSelectProtocol();
	elm327Controller.setAdaptiveTiming();

	Serial.println("Started.");
}

void loop() {
	Serial.println("=== BEGINNING LOOP ===");

	// Read data packet from instrument panel signal and from OBD2
	const DataPacket *packet = dataHandler.readPacket();

	// Broadcast data packet
	networkHandler.broadcastPacket(packet);
	delete packet;
	
	//dataHandler.printCalibrationConstants();
	//delay(2000);

	Serial.println("=== LOOP FINISHED ===");
}