#include "ESP8266Controller.h"

using namespace std;

ESP8266Controller::ESP8266Controller() {
	ready = false;
	serial = NULL;
}

ESP8266Controller::~ESP8266Controller() {

}

bool ESP8266Controller::init(uint8_t serialPort, uint32_t baudRate) {
	switch (serialPort) {
		case 1:
			serial = &Serial1;
			break;
		case 2:
			serial = &Serial2;
			break;
		case 3:
			serial = &Serial3;
			break;
		default:
			serial = NULL;
	}

	serial->begin(baudRate);
	serial->setTimeout(ESP8266_DEBUG ? 10 : 10);
	ready = reset();

	hasConnection = false;

	return ready;
}

bool ESP8266Controller::reset() {
	sendCommand(ESP8266_COMMAND_RESET);
	
	return response(ESP8266_RESPONSE_READY, ESP8266_TIMEOUT_RESET);
}

bool ESP8266Controller::changeMode(ESP8266Mode_t mode) {
	sendCommand(ESP8266_COMMAND_CHANGEMODE, ESP8266CommandParams::create()->push(mode));

	return response(ESP8266_RESPONSE_OK);
}

bool ESP8266Controller::changeTransferMode(ESP8266TransferMode_t mode) {
	sendCommand(ESP8266_COMMAND_T_MODE, ESP8266CommandParams::create()->push(mode));

	return response(ESP8266_RESPONSE_OK);
}

bool ESP8266Controller::setMultiConnection(bool multiConnection) {
	sendCommand(ESP8266_COMMAND_CONN_MODE, ESP8266CommandParams::create()->push(multiConnection));

	return response(ESP8266_RESPONSE_OK);
}

bool ESP8266Controller::setupAp(string ssid, string password, uint8_t channel, string mac, string ip, string netmask) {
	sendCommand(ESP8266_COMMAND_AP_CONFIG,
		ESP8266CommandParams::create()
		->push(ssid)
		->push(password)
		->push(channel)
		->push(3));

	if (! response(ESP8266_RESPONSE_OK)) {
		return false;
	}

	sendCommand(ESP8266_COMMAND_AP_SET_MAC, ESP8266CommandParams::create()->push(mac));

	if (! response(ESP8266_RESPONSE_OK)) {
		return false;
	}

	sendCommand(ESP8266_COMMAND_AP_SET_IP,
		ESP8266CommandParams::create()
		->push(ip)
		->push(ip)
		->push(netmask));

	if (! response(ESP8266_RESPONSE_OK)) {
		return false;
	}

	return true;
}

bool ESP8266Controller::setupSta(string ip, string gateway, string netmask) {
	sendCommand(ESP8266_COMMAND_STA_SET_IP,
		ESP8266CommandParams::create()
		->push(ip)
		->push(gateway)
		->push(netmask));

	if (! response(ESP8266_RESPONSE_OK)) {
		return false;
	}

	return true;
}

bool ESP8266Controller::connectToAp(string ssid, string password) {
	sendCommand(ESP8266_COMMAND_STA_CONN,
		ESP8266CommandParams::create()
		->push(ssid)
		->push(password));

	if (! response(ESP8266_RESPONSE_OK, 5000)) {
		return false;
	}

	return true;
}

vector<ESP8266Station> ESP8266Controller::listStations() {
	vector<ESP8266Station> stations;

	sendCommand(ESP8266_COMMAND_AP_STATIONS);

	if (! response(ESP8266_RESPONSE_OK)) {
		return stations;
	}

	vector<string> responseLines = responseDataLines();
	for (auto it = responseLines.begin(); it != responseLines.end(); it++) {
		Serial.print("Found line: ");
		Serial.println((*it).c_str());
		
		size_t delim = (*it).find(",");

		if (delim == string::npos) {
			continue;
		}

		ESP8266Station station;
		station.ip = (*it).substr(0, delim);
		station.mac = (*it).substr(delim + 1);

		stations.push_back(station);
	}

	return stations;
}

bool ESP8266Controller::tcpServer(uint16_t port, bool create) {
	sendCommand(ESP8266_COMMAND_TCP_SERVER,
		ESP8266CommandParams::create()
			->push(create)
			->push(port));

	if (! response(ESP8266_RESPONSE_OK)) {
		return false;
	}

	sendCommand(ESP8266_COMMAND_TCP_SERVER_TIMEOUT, ESP8266CommandParams::create()->push(5));

	return response(ESP8266_RESPONSE_OK);
}

void ESP8266Controller::listenConnections(bool lookInBuffer) {
	if (lookInBuffer) {
		vector<string> lines = responseDataLines();
		for(auto it = lines.begin(); it != lines.end(); it++) {
			if ((*it) == "0,CONNECTED") {
				hasConnection = true;
			}

			if ((*it) == "0,CLOSED") {
				hasConnection = false;
			}
		}

		return;
	}

	if (! hasConnection && response("0,CONNECTED")) {
		hasConnection = true;
	}

	if (hasConnection && response("0,CLOSED")) {
		hasConnection = false;
	}
}

uint8_t ESP8266Controller::beginTransmission(string connectionType, string remoteAddr, uint16_t remotePort, uint16_t localPort) {
	uint8_t tid = 0;

	ESP8266CommandParams *params = ESP8266CommandParams::create()
		->push(connectionType)
		->push(remoteAddr)
		->push(remotePort);

	if (localPort) {
		params->push(localPort);
	}

	sendCommand(ESP8266_COMMAND_T_BEGIN, params);

	if (! response(ESP8266_RESPONSE_OK)) {
		return 0;
	}

	return tid;
}

bool ESP8266Controller::sendData(uint8_t tid, const uint8_t *data, uint64_t length) {
	if (! hasConnection) {
		//return false;
	}

	ESP8266CommandParams *params = ESP8266CommandParams::create()
		->push(tid)
		->push(length);
		//->push(1024);

	sendCommand(ESP8266_COMMAND_T_SEND, params, data);

	if (! response(ESP8266_RESPONSE_OK, 10000)) {
		return false;
	}

	return true;
}

bool ESP8266Controller::broadcastData(uint16_t remotePort, const uint8_t *data, uint64_t length) {
	ESP8266CommandParams *params = ESP8266CommandParams::create()
		->push(length)
		->push("192.168.0.255")
		->push(remotePort);

	sendCommand(ESP8266_COMMAND_T_SEND, params, data);

	if (! response(ESP8266_RESPONSE_OK)) {
		return false;
	}

	return true;
}

bool ESP8266Controller::endTransmission(uint8_t tid) {
	sendCommand(ESP8266_COMMAND_T_END);

	if (! response(ESP8266_RESPONSE_OK)) {
		return false;
	}

	return true;
}

void ESP8266Controller::sendCommand(string command, ESP8266CommandParams *params, const uint8_t *data) {
	if (command != ESP8266_COMMAND_RESET && !ready) {
		Serial.println("[ESP8266] Error, device not ready yet.");
		
		return;
	}

	stringstream cmd;
	cmd << "AT+" << command;

	if (params) {
		cmd << "=" << params->get();
	}

	cmd << "\r";
	
	if (ESP8266_DEBUG) {
		Serial.print("[ESP8266] Sending command: ");
		Serial.println(cmd.str().c_str());
	}

	serial->println(cmd.str().c_str());
	serial->flush();

	lastCommand = cmd.str();
	lastResponse.clear();

	if (command == ESP8266_COMMAND_T_SEND) {
		if (! response(">")) {
			return;
		}

		if (! data) {
			return;
		}

		cmd.str("");
		cmd << data;

		if (ESP8266_DEBUG) {
			Serial.print("[ESP8266] Sending data: ");
			Serial.println(cmd.str().c_str());
		}

		serial->print(cmd.str().c_str());
		serial->flush();
	}

	delete params;
}

bool ESP8266Controller::response(string response, uint64_t timeout) {
	uint64_t timestamp = millis();
	lastResponse = response;
	responseBuffer.str("");

	while (timestamp + timeout > millis()) {
		String r = serial->readString();
		string buffer(r.c_str());

		responseBuffer << buffer;

		if (ESP8266_DEBUG) {
			//Serial.print("[ESP8266] Read buffer: ");
			//Serial.println(buffer.c_str());
		}

		size_t responsePos = buffer.find(response);
		if (responsePos != string::npos) {
			if (ESP8266_DEBUG) {
				Serial.print("[ESP8266] Response took: ");
				Serial.print((int) (millis() - timestamp));
				Serial.println("ms");

				Serial.print("[ESP8266] Response: ");
				Serial.println(responseBuffer.str().c_str());
				//delay(3000);
			}

			//listenConnections(true);
			return true;
		}
	}

	//listenConnections(true);
	return false;
}

string ESP8266Controller::responseData() {
	string response = responseBuffer.str();

	size_t startPos = response.find(lastCommand);
	size_t endPos = response.find(lastResponse);

	if (startPos == string::npos || endPos == string::npos) {
		return "";
	}

	startPos += lastCommand.length();

	if (response[startPos] == '\n') {
		startPos++;
	}

	return response.substr(startPos, endPos - startPos);
}

vector<string> ESP8266Controller::responseDataLines() {
	string data = responseData();
	vector<string> lines;

	size_t startPos = 0;
	size_t endPos = data.find("\n", 0) + 1;
	while (true) {
		startPos = data.find("\n", endPos);

		if (startPos == string::npos) {
			break;
		}

		lines.push_back(data.substr(endPos, startPos - endPos));
		endPos = startPos + 1;
	}

	return lines;
}