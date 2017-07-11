#include "SignalController.h"

SignalController::SignalController() {

}

SignalController::~SignalController() {

}

void SignalController::init() {
	pinMode(SIGNAL_RPM_PIN, INPUT_PULLUP);
	pinMode(SIGNAL_SPEED_PIN, INPUT_PULLUP);
	pinMode(SIGNAL_TEMPERATURE_PIN, INPUT_PULLUP);
	pinMode(SIGNAL_FUEL_LEVEL_PIN, INPUT_PULLUP);
	
	pinMode(SIGNAL_TURN_LEFT_PIN, INPUT_PULLUP);
	pinMode(SIGNAL_TURN_RIGHT_PIN, INPUT_PULLUP);
	pinMode(SIGNAL_HEADLAMP_PIN, INPUT_PULLUP);
	pinMode(SIGNAL_DOOR_PIN, INPUT_PULLUP);
	pinMode(SIGNAL_OIL_PRESSURE_PIN, INPUT_PULLUP);
	pinMode(SIGNAL_PARKING_BRAKE_PIN, INPUT_PULLUP);
	pinMode(SIGNAL_KEY_POS1_PIN, INPUT_PULLUP);
	pinMode(SIGNAL_KEY_POS2_PIN, INPUT_PULLUP);
	pinMode(SIGNAL_CRANKING_PIN, INPUT_PULLUP);
	pinMode(SIGNAL_DIAGNOSIS_PIN, INPUT_PULLUP);
	pinMode(SIGNAL_UC_PIN, INPUT_PULLUP);
	pinMode(SIGNAL_ALTERNATOR_PIN, INPUT_PULLUP);
	pinMode(SIGNAL_IMOBILIZER_PIN, INPUT_PULLUP);
	pinMode(SIGNAL_AIR_CONDITIONING_PIN, INPUT_PULLUP);
	pinMode(SIGNAL_DEFROST_PIN, INPUT_PULLUP);
	pinMode(SIGNAL_AIR_RECIRCULATION_PIN, INPUT_PULLUP);
}

uint64_t SignalController::getSignalPulseWidth(uint8_t pin, uint64_t timeout) const {
	return pulseIn(pin, HIGH, timeout);
}

uint16_t SignalController::getEngineRpm() const {
	uint64_t pulseWidth = getSignalPulseWidth(SIGNAL_RPM_PIN, 200000);

	return 15057648.0 / pulseWidth;
}

uint8_t SignalController::getSpeed() const {
	uint64_t pulseWidth = getSignalPulseWidth(SIGNAL_SPEED_PIN, 200000);

	return 139040.0 / pulseWidth;
}

uint8_t SignalController::getCoolantTemp() const {
	uint64_t pulseWidth = getSignalPulseWidth(SIGNAL_TEMPERATURE_PIN, 2000);

	return 0.02 * pulseWidth;
}

uint8_t SignalController::getFuelLevel() const {
	return 0;
}