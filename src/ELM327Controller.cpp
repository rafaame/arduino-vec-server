#include "ELM327Controller.h"

using namespace std;

ELM327Controller::ELM327Controller() {
	ready = false;
	serial = NULL;
}

ELM327Controller::~ELM327Controller() {

}

bool ELM327Controller::init(uint8_t serialPort, uint32_t baudRate) {
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
	serial->setTimeout(ELM327_DEBUG ? 1000 : 10);
	ready = reset();
	delay(ELM327_TIMEOUT_RESET);

	return ready;
}

bool ELM327Controller::reset() const {
	if (ELM327_DEBUG) {
		Serial.println("[ELM327] Resetting...");
	}

	sendCommand(ELM327_COMMAND_RESET);
	
	return confirmResponse(ELM327_RESPONSE_READY, ELM327_TIMEOUT_RESET);
}

void ELM327Controller::setEchoOff() const {
	sendCommand(ELM327_COMMAND_ECHO_OFF);

	response(ELM327_TIMEOUT_COMMAND);
	delay(ELM327_TIMEOUT_COMMAND);
}

void ELM327Controller::autoSelectProtocol() const {
	sendCommand(ELM327_COMMAND_PROTOCOL_AUTO);

	response(ELM327_TIMEOUT_COMMAND);
	delay(ELM327_TIMEOUT_COMMAND);
}

void ELM327Controller::setAdaptiveTiming() const {
	sendCommand(ELM327_COMMAND_ADAPTIVE_TIMING);

	response(ELM327_TIMEOUT_COMMAND);
	delay(ELM327_TIMEOUT_COMMAND);
}

uint16_t ELM327Controller::getEngineRpm() const {
	sendCommand(ELM327_COMMAND_ENGINE_RPM);
	vector<uint8_t> result = responseBytes(ELM327_TIMEOUT_COMMAND, "00000000");

	return ((uint16_t) result[2] * 256 + (uint16_t) result[3]) / 4;
}

float ELM327Controller::getEngineLoad() const {
	sendCommand(ELM327_COMMAND_ENGINE_LOAD);
	vector<uint8_t> result = responseBytes(ELM327_TIMEOUT_COMMAND, "00000000");

	return (result[2] * 256 + result[3]) / 255.0f;
}

uint8_t ELM327Controller::getCoolantTemp() const {
	sendCommand(ELM327_COMMAND_COOLANT_TEMP);
	vector<uint8_t> result = responseBytes(ELM327_TIMEOUT_COMMAND, "000000");

	return result[2] - 40;
}

uint8_t ELM327Controller::getFuelLevel() const {
	sendCommand(ELM327_COMMAND_FUEL_LEVEL);
	vector<uint8_t> result = responseBytes(ELM327_TIMEOUT_COMMAND, "000000");

	return 100.0f * result[2] / 255.0f;
}

float ELM327Controller::getThrottle() const {
	sendCommand(ELM327_COMMAND_THROTTLE);
	vector<uint8_t> result = responseBytes(ELM327_TIMEOUT_COMMAND, "000000");

	return (result[2] * 100) / 255.0f;
}

float ELM327Controller::getMafFlow() const {
	sendCommand(ELM327_COMMAND_MAF_FLOW);
	vector<uint8_t> result = responseBytes(ELM327_TIMEOUT_COMMAND, "00000000");

	return (result[2] * 256 + result[3]) / 100.0f;
}

uint8_t ELM327Controller::getSpeed() const {
	sendCommand(ELM327_COMMAND_SPEED);
	vector<uint8_t> result = responseBytes(ELM327_TIMEOUT_COMMAND, "000000");

	return result[2];
}

void ELM327Controller::sendCommand(string command) const {
	if (command != ELM327_COMMAND_RESET && !ready) {
		Serial.println("[ELM327] Error, device not ready yet.");
		
		return;
	}

	readFlush();

	stringstream cmd;
	cmd << command << "\r";

	if (ELM327_DEBUG) {
		Serial.print("[ELM327] Sending command: ");
		Serial.println(cmd.str().c_str());
	}

	serial->println(cmd.str().c_str());
	serial->flush();
}

bool ELM327Controller::confirmResponse(string expected, uint64_t timeout) const {
	string data = response(timeout);

	return data == expected;
}

vector<uint8_t> ELM327Controller::responseBytes(uint64_t timeout, string defaultResponse) const {
	string data = response(timeout, defaultResponse);
	vector<uint8_t> result;

	if (data.length() % 2 != 0) {
		if (ELM327_DEBUG) {
			Serial.print("[ELM327] responseBytes on a non-even sized string: ");
			Serial.println(data.c_str());
		}

		return result;
	}

	if (ELM327_DEBUG) {
		Serial.print("[ELM327] Response string: ");
		Serial.println(data.c_str());
		
		Serial.print("[ELM327] Response in bytes: ");
	}

	for (uint8_t i = 0; i < data.length(); i += 2) {
		uint8_t byte = 0;

		byte += (data[i] >= 'A' ? (data[i] - 55) : (data[i] - '0')) * 16;
		byte += (data[i + 1] >= 'A' ? (data[i + 1] - 55) : (data[i + 1] - '0'));

		result.push_back(byte);

		if (ELM327_DEBUG) {
			Serial.print((int) byte);
			Serial.print(" ");
		}
	}

	if (ELM327_DEBUG) {
		Serial.println("\r\n");
	}

	return result;
}

string ELM327Controller::response(uint64_t timeout, string defaultResponse) const {
	uint64_t timestamp = millis();
	stringstream buffer;

	if (ELM327_DEBUG) {
		Serial.println("[ELM327] Read response:");
	}

	while (timestamp + timeout > millis()) {
		int8_t c = serial->read();

		if (c == '\0' || c == -1) {
			continue;
		}

		if (c == '>') {
			if (ELM327_DEBUG) {
				Serial.print("\r\n");
				Serial.print("[ELM327] Response took: ");
				Serial.print((int) (millis() - timestamp));
				Serial.println("ms");
			}

			string response = trimResponse(buffer.str());
			response = responseTreatErrors(response, defaultResponse);

			return response;
		}

		buffer << c;

		if (ELM327_DEBUG) {
			Serial.print((char) c);
		}
	}

	//forceInterrupt();
	//delay(1000);

	if (ELM327_DEBUG) {
		Serial.print("\r\n");
	}

	return defaultResponse;
}

string ELM327Controller::trimResponse(string response) const {
	//response = str_replace("SEARCHING", "", response);
	response = str_replace("\t", "", response);
	response = str_replace("\n", "", response);
	response = str_replace("\x0B", "", response);
	response = str_replace("\f", "", response);
	response = str_replace("\r", "", response);
	response = str_replace(" ", "", response);

	if (ELM327_DEBUG) {
		Serial.print("[ELM327] Trimmed response: ");
		Serial.println(response.c_str());
	}

	return response;
}

string ELM327Controller::responseTreatErrors(string response, string defaultResponse) const {
	if (response.find("STOPPED") != string::npos ||
		response.find("ERROR") != string::npos ||
		response.find("NODATA") != string::npos ||
		response.find("?") != string::npos ||
		response.find("UNABLE TO CONNECT") != string::npos ||
		response.find("SEARCHING") != string::npos ||
		response == "7F0112" // PID not supported
		) {
		if (ELM327_DEBUG) {
			Serial.println("Found error in response, ignoring...");
		}

		return defaultResponse;
	}

	return response;
}

void ELM327Controller::readFlush() const {
	while (serial->read() != -1);
}

void ELM327Controller::forceInterrupt() const {
	readFlush();

	serial->println("XXXXXXXXX\r\r\r");
	serial->flush();

	readFlush();
}