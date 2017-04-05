#ifndef ELM327CONTROLLER_H
#define ELM327CONTROLLER_H

#include "common.h"

#define ELM327_DEBUG false

#define ELM327_TIMEOUT_COMMAND 1000
#define ELM327_TIMEOUT_RESET   2000

#define ELM327_COMMAND_RESET           "AT Z"
#define ELM327_COMMAND_ECHO_OFF        "AT E0"
#define ELM327_COMMAND_PROTOCOL_AUTO   "AT SP 0"
#define ELM327_COMMAND_ADAPTIVE_TIMING "AT AT1"
#define ELM327_COMMAND_ENGINE_RPM      "01 0C 1"
#define ELM327_COMMAND_ENGINE_LOAD     "01 43 1"
#define ELM327_COMMAND_COOLANT_TEMP    "01 05 1"
#define ELM327_COMMAND_THROTTLE        "01 11 1"
#define ELM327_COMMAND_MAF_FLOW        "01 10 1"
#define ELM327_COMMAND_SPEED           "01 0D 1"

#define ELM327_RESPONSE_READY "ATZELM327v1.5"

using namespace std;

class ELM327Controller {
private:
	HardwareSerial *serial;
	bool ready;

public:
	ELM327Controller();
	~ELM327Controller();

	bool init(uint8_t serialPort, uint32_t baudRate = 38400);
	bool reset() const;
	void setEchoOff() const;
	void autoSelectProtocol() const;
	void setAdaptiveTiming() const;

	uint16_t getEngineRpm() const;
	float getEngineLoad() const;
	uint8_t getCoolantTemp() const;
	float getThrottle() const;
	float getMafFlow() const;
	uint8_t getSpeed() const;

	void sendCommand(string command) const;
	bool confirmResponse(string expected, uint64_t timeout = ELM327_TIMEOUT_COMMAND) const;
	vector<uint8_t> responseBytes(uint64_t timeout = ELM327_TIMEOUT_COMMAND, string defaultResponse = "") const;
	string response(uint64_t timeout = ELM327_TIMEOUT_COMMAND, string defaultResponse = "") const;
	string trimResponse(string response) const;
	string responseTreatErrors(string response, string defaultResponse) const;
	void readFlush() const;
	void forceInterrupt() const;
};

#endif