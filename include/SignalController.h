#ifndef SIGNALCONTROLLER_H
#define SIGNALCONTROLLER_H

#include "common.h"
#include "const.h"

#define SIGNALMANAGER_DEBUG true

#define SIGNAL_RPM_PIN 18
#define SIGNAL_SPEED_PIN 19
#define SIGNAL_TEMPERATURE_PIN 20
#define SIGNAL_FUEL_LEVEL_PIN 21
#define SIGNAL_TURN_LEFT_PIN 36
#define SIGNAL_TURN_RIGHT_PIN 38
#define SIGNAL_HEADLAMP_PIN 40
#define SIGNAL_DOOR_PIN 42
#define SIGNAL_OIL_PRESSURE_PIN 44
#define SIGNAL_PARKING_BRAKE_PIN 46
#define SIGNAL_KEY_POS1_PIN 32
#define SIGNAL_KEY_POS2_PIN 34

using namespace std;

class SignalController {
private:
	static volatile uint64_t** signalCounters;

public:
	SignalController();
	~SignalController();

	void init();
	void addInterrupt(uint8_t pin, uint8_t mode);
	
	static void interruptCounter(uint8_t pin);
	static void interruptCounterPin2() { SignalController::interruptCounter(2); }
	static void interruptCounterPin3() { SignalController::interruptCounter(3); }
	static void interruptCounterPin18() { SignalController::interruptCounter(18); }
	static void interruptCounterPin19() { SignalController::interruptCounter(19); }
	static void interruptCounterPin20() { SignalController::interruptCounter(20); }
	static void interruptCounterPin21() { SignalController::interruptCounter(21); }

	static uint8_t interruptIndex(uint8_t pin) {
		if (pin == 2) return 0;
		if (pin == 3) return 1;
		if (pin == 18) return 2;
		if (pin == 19) return 3;
		if (pin == 20) return 4;
		if (pin == 21) return 5;

		return 255;
	}

	uint64_t getSignalPulseWidth(uint8_t pin, uint64_t timeout) const;
	uint64_t getSignalFrequency(uint8_t pin) const;
	void clearSignalCounter(uint8_t pin);

	uint16_t getEngineRpm() const;
	uint8_t getSpeed() const;
	uint8_t getCoolantTemp() const;
	uint8_t getFuelLevel() const;

	bool isTurnLeftOn() const { return digitalRead(SIGNAL_TURN_LEFT_PIN) == HIGH; }
	bool isTurnRightOn() const { return digitalRead(SIGNAL_TURN_RIGHT_PIN) == HIGH; }
	bool isHeadlampOn() const { return digitalRead(SIGNAL_HEADLAMP_PIN) == HIGH; }
	bool hasOpenDoor() const { return digitalRead(SIGNAL_DOOR_PIN) == LOW; }
	bool hasOilPressure() const { return digitalRead(SIGNAL_OIL_PRESSURE_PIN) == HIGH; }
	bool isParkingBrakeOn() const { return digitalRead(SIGNAL_PARKING_BRAKE_PIN) == LOW; }
	bool isKeyOnFirstPos() const { return digitalRead(SIGNAL_KEY_POS1_PIN) == HIGH; }
	bool isKeyAfterFirstPos() const { return digitalRead(SIGNAL_KEY_POS2_PIN) == HIGH; }
};

#endif