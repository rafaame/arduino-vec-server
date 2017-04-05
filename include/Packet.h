#ifndef PACKET_H
#define PACKET_H

#include "common.h"
#include "const.h"

enum TransportProtocol_t {
	TRANSPORT_PROTOCOL_UDP,
	TRANSPORT_PROTOCOL_TCP
};

using namespace std;

class NetworkPacket {
public:
	TransportProtocol_t transportProtocol;
	uint16_t            remotePort;

	NetworkPacket() {
		transportProtocol = TRANSPORT_PROTOCOL_TCP;
		remotePort        = 80;
	};

	virtual ~NetworkPacket() {

	}

	virtual const string raw() const = 0;
};

class DataPacket : public NetworkPacket {
public:
	int engineRpm;
	int speed;
	int coolantTemp;
	int fuelLevel;

	bool isDriverPresent;
	bool isTurnLeftOn;
	bool isTurnRightOn;
	bool isHeadlampOn;
	bool hasOpenDoor;
	bool hasOilPressure;
	bool isParkingBrakeOn;
	bool isKeyOnFirstPos;
	bool isKeyAfterFirstPos;

	DataPacket() : NetworkPacket() {
		transportProtocol = TRANSPORT_PROTOCOL_UDP;
		remotePort        = DATA_REMOTE_PORT;

		engineRpm   = 0;
		speed       = 0;
		coolantTemp = 0;
		fuelLevel   = 0;

		isDriverPresent    = false;
		isTurnLeftOn       = false;
		isTurnRightOn      = false;
		isHeadlampOn       = false;
		hasOpenDoor        = false;
		hasOilPressure     = false;
		isParkingBrakeOn   = false;
		isKeyOnFirstPos    = false;
		isKeyAfterFirstPos = false;
	};

	virtual ~DataPacket() {
		
	}

	virtual const string raw() const {
		stringstream buffer;
		
		buffer << "{" << endl;

		buffer << "\"engineRpm\": " << engineRpm << "," << endl;
		buffer << "\"speed\": " << speed << "," << endl;
		buffer << "\"coolantTemp\": " << coolantTemp << "," << endl;
		buffer << "\"fuelLevel\": " << fuelLevel << "," << endl;

		buffer << "\"isDriverPresent\": " << (isDriverPresent ? "true" : "false") << "," << endl;
		buffer << "\"isTurnLeftOn\": " << (isTurnLeftOn ? "true" : "false") << "," << endl;
		buffer << "\"isTurnRightOn\": " << (isTurnRightOn ? "true" : "false") << "," << endl;
		buffer << "\"isHeadlampOn\": " << (isHeadlampOn ? "true" : "false") << "," << endl;
		buffer << "\"hasOpenDoor\": " << (hasOpenDoor ? "true" : "false") << "," << endl;
		buffer << "\"hasOilPressure\": " << (hasOilPressure ? "true" : "false") << "," << endl;
		buffer << "\"isParkingBrakeOn\": " << (isParkingBrakeOn ? "true" : "false") << "," << endl;
		buffer << "\"isKeyOnFirstPos\": " << (isKeyOnFirstPos ? "true" : "false") << "," << endl;
		buffer << "\"isKeyAfterFirstPos\": " << (isKeyAfterFirstPos ? "true" : "false") << endl;

		buffer << "}";

		return buffer.str();
	}
};

#endif