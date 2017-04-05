#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include "common.h"

#define DATA_DEBUG true

#define FUEL_LEVEL_UPDATE_PERIOD 60000
#define ENGINE_LOAD_UPDATE_PERIOD 60000
#define TIMING_ADVANCE_UPDATE_PERIOD 60000
#define THROTTLE_UPDATE_PERIOD 60000

class ELM327Controller;
class SignalController;
class DataPacket;

class DataHandler {
private:
	const ELM327Controller *elm327Controller;
	const SignalController *signalController;

public:
	DataHandler(const ELM327Controller *elm327Controller, const SignalController *signalController);
	~DataHandler();

	const DataPacket *readPacket();
	void printCalibrationConstants();
};

#endif