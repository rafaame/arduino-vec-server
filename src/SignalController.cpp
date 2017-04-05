#include "SignalController.h"

volatile uint64_t **SignalController::signalCounters = new volatile uint64_t *[MAX_INTERRUPT_PINS];

SignalController::SignalController() {

}

SignalController::~SignalController() {

}

void SignalController::init() {
	pinMode(SIGNAL_RPM_PIN, INPUT_PULLUP);
	pinMode(SIGNAL_SPEED_PIN, INPUT_PULLUP);
	pinMode(SIGNAL_TEMPERATURE_PIN, INPUT_PULLUP);
	pinMode(SIGNAL_FUEL_LEVEL_PIN, INPUT_PULLUP);
	
	pinMode(SIGNAL_TURN_LEFT_PIN, INPUT);
	pinMode(SIGNAL_TURN_RIGHT_PIN, INPUT);
	pinMode(SIGNAL_HEADLAMP_PIN, INPUT);
	pinMode(SIGNAL_DOOR_PIN, INPUT);
	pinMode(SIGNAL_OIL_PRESSURE_PIN, INPUT);
	pinMode(SIGNAL_PARKING_BRAKE_PIN, INPUT);
	pinMode(SIGNAL_KEY_POS1_PIN, INPUT);
	pinMode(SIGNAL_KEY_POS2_PIN, INPUT);
}

void SignalController::addInterrupt(uint8_t pin, uint8_t mode) {
	void (*interruptCounter)(void) = NULL;

	switch (pin) {
		case 2:
			interruptCounter = SignalController::interruptCounterPin2;
			break;

		case 3:
			interruptCounter = SignalController::interruptCounterPin3;
			break;

		case 18:
			interruptCounter = SignalController::interruptCounterPin18;
			break;

		case 19:
			interruptCounter = SignalController::interruptCounterPin19;
			break;

		case 20:
			interruptCounter = SignalController::interruptCounterPin20;
			break;

		case 21:
			interruptCounter = SignalController::interruptCounterPin21;
			break;

		default:
			return;
	}

	SignalController::signalCounters[SignalController::interruptIndex(pin)] = new uint64_t[2];
	clearSignalCounter(pin);

	pinMode(pin, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(pin), interruptCounter, (int) mode);
}

void SignalController::interruptCounter(uint8_t pin) {
	SignalController::signalCounters[SignalController::interruptIndex(pin)][0]++;
}

uint64_t SignalController::getSignalPulseWidth(uint8_t pin, uint64_t timeout) const {
	return pulseIn(pin, HIGH, timeout);
}

uint64_t SignalController::getSignalFrequency(uint8_t pin) const {
	uint64_t counter = SignalController::signalCounters[SignalController::interruptIndex(pin)][0];
	double elapsedTime = (millis() - SignalController::signalCounters[SignalController::interruptIndex(pin)][1]) / 1000.0;

	return (uint64_t) (counter / elapsedTime);
}

void SignalController::clearSignalCounter(uint8_t pin) {
	SignalController::signalCounters[SignalController::interruptIndex(pin)][0] = 0;
	SignalController::signalCounters[SignalController::interruptIndex(pin)][1] = millis();
}

uint16_t SignalController::getEngineRpm() const {
	uint64_t pulseWidth = getSignalPulseWidth(SIGNAL_RPM_PIN, 100000);

	return 15057648.0 / pulseWidth;
}

uint8_t SignalController::getSpeed() const {
	uint64_t pulseWidth = getSignalPulseWidth(SIGNAL_SPEED_PIN, 100000);

	return 139040.0 / pulseWidth;
}

uint8_t SignalController::getCoolantTemp() const {
	uint64_t pulseWidth = getSignalPulseWidth(SIGNAL_TEMPERATURE_PIN, 2000);

	return 0.02 * pulseWidth;
}

uint8_t SignalController::getFuelLevel() const {
	return 0;
}