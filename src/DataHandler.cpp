#include "DataHandler.h"
#include "StateController.h"
#include "ELM327Controller.h"
#include "SignalController.h"
#include "Packet.h"

DataHandler::DataHandler(const StateController *stateController, const ELM327Controller *elm327Controller, const SignalController *signalController) {
	this->stateController = stateController;
	this->elm327Controller = elm327Controller;
	this->signalController = signalController;
}

DataHandler::~DataHandler() {
	
}

const DataPacket *DataHandler::readPacket() {
	if (DATA_DEBUG) {
		Serial.println("Reading data info...");
	}

	uint64_t currentTime = millis();
	DataPacket *packet = new DataPacket();
	static DataPacket *lastPacket = packet;

	packet->engineRpm = signalController->getEngineRpm();
	packet->speed = signalController->getSpeed();
	
	packet->coolantTemp = lastPacket->coolantTemp;
	static uint64_t coolantTempLastUpdated = millis();
	if ((currentTime - coolantTempLastUpdated) > COOLANT_TEMP_UPDATE_PERIOD) {
		packet->coolantTemp = elm327Controller->getCoolantTemp();
		coolantTempLastUpdated = currentTime;
	}

	packet->fuelLevel = lastPacket->fuelLevel;
	static uint64_t fuelLevelLastUpdated = millis();
	if ((currentTime - fuelLevelLastUpdated) > FUEL_LEVEL_UPDATE_PERIOD) {
		packet->fuelLevel = elm327Controller->getFuelLevel();
		fuelLevelLastUpdated = currentTime;
	}
	
	packet->isDriverPresent = signalController->isKeyOnFirstPos();
	packet->isTurnLeftOn = signalController->isTurnLeftOn();
	packet->isTurnRightOn = signalController->isTurnRightOn();
	packet->isHeadlampOn = signalController->isHeadlampOn();
	packet->hasOpenDoor = signalController->hasOpenDoor();
	packet->hasOilPressure = signalController->hasOilPressure();
	packet->isParkingBrakeOn = signalController->isParkingBrakeOn();
	packet->isKeyOnFirstPos = signalController->isKeyOnFirstPos();
	packet->isKeyAfterFirstPos = signalController->isKeyAfterFirstPos();
	packet->isCranking = signalController->isCranking();
	packet->hasDiagnosisSignal = signalController->hasDiagnosisSignal();
	packet->hasUcSignal = signalController->hasUcSignal();
	packet->isAlternatorOn = signalController->isAlternatorOn();
	packet->isImobilizerOn = signalController->isImobilizerOn();
	packet->isAirConditioningOn = signalController->isAirConditioningOn();
	packet->isDefrostOn = signalController->isDefrostOn();
	packet->isAirRecirculationOn = signalController->isAirRecirculationOn();

	State state = stateController->getState();
	packet->ventilatorPosition = state.ventilatorPosition;
	packet->ventilatorSpeed = state.ventilatorSpeed;
	packet->areHazardLightsOn = state.areHazardLightsOn;

	if (DATA_DEBUG) {
		Serial.println("Read info:");
		
		Serial.print("    engineRpm: ");
		Serial.println((int) packet->engineRpm);

		Serial.print("    speed: ");
		Serial.println((int) packet->speed);

		Serial.print("    coolantTemp: ");
		Serial.println((int) packet->coolantTemp);

		Serial.print("    fuelLevel: ");
		Serial.println((int) packet->fuelLevel);

		Serial.print("    isDriverPresent: ");
		Serial.println((int) packet->isDriverPresent);

		Serial.print("    isTurnLeftOn: ");
		Serial.println((int) packet->isTurnLeftOn);

		Serial.print("    isTurnRightOn: ");
		Serial.println((int) packet->isTurnRightOn);

		Serial.print("    isHeadlampOn: ");
		Serial.println((int) packet->isHeadlampOn);

		Serial.print("    hasOpenDoor: ");
		Serial.println((int) packet->hasOpenDoor);

		Serial.print("    hasOilPressure: ");
		Serial.println((int) packet->hasOilPressure);

		Serial.print("    isParkingBrakeOn: ");
		Serial.println((int) packet->isParkingBrakeOn);

		Serial.print("    isKeyOnFirstPos: ");
		Serial.println((int) packet->isKeyOnFirstPos);

		Serial.print("    isKeyAfterFirstPos: ");
		Serial.println((int) packet->isKeyAfterFirstPos);

		Serial.print("    isCranking: ");
		Serial.println((int) packet->isCranking);

		Serial.print("    hasDiagnosisSignal: ");
		Serial.println((int) packet->hasDiagnosisSignal);

		Serial.print("    hasUcSignal: ");
		Serial.println((int) packet->hasUcSignal);

		Serial.print("    isAlternatorOn: ");
		Serial.println((int) packet->isAlternatorOn);

		Serial.print("    isImobilizerOn: ");
		Serial.println((int) packet->isImobilizerOn);

		Serial.print("    isAirConditioningOn: ");
		Serial.println((int) packet->isAirConditioningOn);

		Serial.print("    isDefrostOn: ");
		Serial.println((int) packet->isDefrostOn);

		Serial.print("    isAirRecirculationOn: ");
		Serial.println((int) packet->isAirRecirculationOn);

		Serial.println("");
	}

	if (lastPacket != packet) {
		delete lastPacket;
	}

	lastPacket = packet;

	return packet;
}

void DataHandler::printCalibrationConstants() {
	// Engine RPM calibration
	/*uint16_t value = elm327Controller->getEngineRpm();
	uint64_t signalPulseWidth = signalController->getSignalPulseWidth(SIGNAL_RPM_PIN, 100000);
	double calibrationConstant = (signalPulseWidth > 0) ? (value * signalPulseWidth) : -1;
	// 0.045

	Serial.print("Engine RPM Calibration: (");
	Serial.print(calibrationConstant, 6);
	Serial.print("; ");
	Serial.print((int) value);
	Serial.print("; ");
	Serial.print((long int) signalPulseWidth);
	Serial.println(")");

	// Speed calibration
	value = elm327Controller->getSpeed();
	signalPulseWidth = signalController->getSignalPulseWidth(SIGNAL_SPEED_PIN, 100000);
	calibrationConstant = (signalPulseWidth > 0) ? (value * signalPulseWidth) : -1;
	// ?

	Serial.print("Speed Calibration: (");
	Serial.print(calibrationConstant, 6);
	Serial.print("; ");
	Serial.print((int) value);
	Serial.print("; ");
	Serial.print((long int) signalPulseWidth);
	Serial.println(")");*/

	// Coolant temperature calibration
	uint16_t value = elm327Controller->getCoolantTemp();
	uint64_t signalPulseWidth = pulseIn(SIGNAL_TEMPERATURE_PIN, HIGH);
	double calibrationConstant = (signalPulseWidth > 0) ? ((double) value / (double) signalPulseWidth) : -1;
	// ?

	Serial.print("Temperature Calibration: (");
	Serial.print(calibrationConstant, 6);
	Serial.print("; ");
	Serial.print((int) value);
	Serial.print("; ");
	Serial.print((long int) signalPulseWidth);
	Serial.println(")");
}

const DataPacket *DataHandler::generateTestPacket() {
	if (DATA_DEBUG) {
		Serial.println("Reading data info...");
	}

	uint64_t currentTime = millis();
	DataPacket *packet = new DataPacket();
	static DataPacket *lastPacket = packet;

	packet->engineRpm = rand() % 5000;
	packet->speed = rand() % 180;
	
	packet->coolantTemp = lastPacket->coolantTemp;
	static uint64_t coolantTempLastUpdated = millis();
	if ((currentTime - coolantTempLastUpdated) > COOLANT_TEMP_UPDATE_PERIOD) {
		packet->coolantTemp = rand() % 100;
		coolantTempLastUpdated = currentTime;
	}

	packet->fuelLevel = lastPacket->fuelLevel;
	static uint64_t fuelLevelLastUpdated = millis();
	if ((currentTime - fuelLevelLastUpdated) > FUEL_LEVEL_UPDATE_PERIOD) {
		packet->fuelLevel = rand() % 100;
		fuelLevelLastUpdated = currentTime;
	}
	
	packet->isDriverPresent = true;
	packet->isTurnLeftOn = (bool) (rand() % 2);
	packet->isTurnRightOn = (bool) (rand() % 2);
	packet->isHeadlampOn = (bool) (rand() % 2);
	packet->hasOpenDoor = (bool) (rand() % 2);
	packet->hasOilPressure = (bool) (rand() % 2);
	packet->isParkingBrakeOn = (bool) (rand() % 2);
	packet->isKeyOnFirstPos = true;
	packet->isKeyAfterFirstPos = true;
	packet->isCranking = (bool) (rand() % 2);
	packet->hasDiagnosisSignal = (bool) (rand() % 2);
	packet->hasUcSignal = (bool) (rand() % 2);
	packet->isAlternatorOn = (bool) (rand() % 2);
	packet->isImobilizerOn = (bool) (rand() % 2);
	packet->isAirConditioningOn = (bool) (rand() % 2);
	packet->isDefrostOn = (bool) (rand() % 2);
	packet->isAirRecirculationOn = (bool) (rand() % 2);

	State state = stateController->getState();
	packet->ventilatorPosition = state.ventilatorPosition;
	packet->ventilatorSpeed = state.ventilatorSpeed;
	packet->areHazardLightsOn = state.areHazardLightsOn;

	if (DATA_DEBUG) {
		Serial.println("Read info:");
		
		Serial.print("    engineRpm: ");
		Serial.println((int) packet->engineRpm);

		Serial.print("    speed: ");
		Serial.println((int) packet->speed);

		Serial.print("    coolantTemp: ");
		Serial.println((int) packet->coolantTemp);

		Serial.print("    fuelLevel: ");
		Serial.println((int) packet->fuelLevel);

		Serial.print("    isDriverPresent: ");
		Serial.println((int) packet->isDriverPresent);

		Serial.print("    isTurnLeftOn: ");
		Serial.println((int) packet->isTurnLeftOn);

		Serial.print("    isTurnRightOn: ");
		Serial.println((int) packet->isTurnRightOn);

		Serial.print("    isHeadlampOn: ");
		Serial.println((int) packet->isHeadlampOn);

		Serial.print("    hasOpenDoor: ");
		Serial.println((int) packet->hasOpenDoor);

		Serial.print("    hasOilPressure: ");
		Serial.println((int) packet->hasOilPressure);

		Serial.print("    isParkingBrakeOn: ");
		Serial.println((int) packet->isParkingBrakeOn);

		Serial.print("    isKeyOnFirstPos: ");
		Serial.println((int) packet->isKeyOnFirstPos);

		Serial.print("    isKeyAfterFirstPos: ");
		Serial.println((int) packet->isKeyAfterFirstPos);

		Serial.print("    isCranking: ");
		Serial.println((int) packet->isCranking);

		Serial.print("    hasDiagnosisSignal: ");
		Serial.println((int) packet->hasDiagnosisSignal);

		Serial.print("    hasUcSignal: ");
		Serial.println((int) packet->hasUcSignal);

		Serial.print("    isAlternatorOn: ");
		Serial.println((int) packet->isAlternatorOn);

		Serial.print("    isImobilizerOn: ");
		Serial.println((int) packet->isImobilizerOn);

		Serial.print("    isAirConditioningOn: ");
		Serial.println((int) packet->isAirConditioningOn);

		Serial.print("    isDefrostOn: ");
		Serial.println((int) packet->isDefrostOn);

		Serial.print("    isAirRecirculationOn: ");
		Serial.println((int) packet->isAirRecirculationOn);

		Serial.println("");
	}

	if (lastPacket != packet) {
		delete lastPacket;
	}

	lastPacket = packet;

	return packet;
}