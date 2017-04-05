#include "DataHandler.h"
#include "ELM327Controller.h"
#include "SignalController.h"
#include "Packet.h"
#include "Spline.h"

DataHandler::DataHandler(const ELM327Controller *elm327Controller, const SignalController *signalController) {
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

	packet->engineRpm = signalController->getEngineRpm();
	//packet->engineRpm = 1000;
	packet->speed = signalController->getSpeed();
	//packet->speed = packet->engineRpm;
	packet->coolantTemp = 0;
	//packet->coolantTemp = signalController->getCoolantTemp();
	
	packet->fuelLevel = 0;
	/*static uint64_t fuelLevelLastUpdated = millis();
	if ((currentTime - fuelLevelLastUpdated) > FUEL_LEVEL_UPDATE_PERIOD) {
		packet->fuelLevel = elm327Controller->getFuelLevel();
		fuelLevelLastUpdated = currentTime;
	}*/
	
	packet->isDriverPresent = signalController->isKeyOnFirstPos();
	packet->isTurnLeftOn = signalController->isTurnLeftOn();
	packet->isTurnRightOn = signalController->isTurnRightOn();
	packet->isHeadlampOn = signalController->isHeadlampOn();
	packet->hasOpenDoor = signalController->hasOpenDoor();
	packet->hasOilPressure = signalController->hasOilPressure();
	packet->isParkingBrakeOn = signalController->isParkingBrakeOn();
	packet->isKeyOnFirstPos = signalController->isKeyOnFirstPos();
	packet->isKeyAfterFirstPos = signalController->isKeyAfterFirstPos();

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


		Serial.println("");
	}

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