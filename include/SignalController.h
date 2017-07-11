#ifndef SIGNALCONTROLLER_H
#define SIGNALCONTROLLER_H

#include "const.h"

#define SIGNALMANAGER_DEBUG true

#define SIGNAL_RPM_PIN 18
#define SIGNAL_SPEED_PIN 19
#define SIGNAL_TEMPERATURE_PIN 20
#define SIGNAL_FUEL_LEVEL_PIN 21
#define SIGNAL_TURN_LEFT_PIN 22
#define SIGNAL_TURN_RIGHT_PIN 23
#define SIGNAL_HEADLAMP_PIN 24
#define SIGNAL_DOOR_PIN 25
#define SIGNAL_OIL_PRESSURE_PIN 26
#define SIGNAL_PARKING_BRAKE_PIN 27
#define SIGNAL_KEY_POS1_PIN 28
#define SIGNAL_KEY_POS2_PIN 29
#define SIGNAL_CRANKING_PIN 30
#define SIGNAL_DIAGNOSIS_PIN 31
#define SIGNAL_UC_PIN 32
#define SIGNAL_ALTERNATOR_PIN 33
#define SIGNAL_IMOBILIZER_PIN 34
#define SIGNAL_AIR_CONDITIONING_PIN 35
#define SIGNAL_DEFROST_PIN 36
#define SIGNAL_AIR_RECIRCULATION_PIN 37

using namespace std;

class SignalController {
public:
	SignalController();
	~SignalController();

	void init();
	uint64_t getSignalPulseWidth(uint8_t pin, uint64_t timeout) const;
	
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
	bool isCranking() const { return digitalRead(SIGNAL_CRANKING_PIN) == HIGH; }
	bool hasDiagnosisSignal() const { return digitalRead(SIGNAL_DIAGNOSIS_PIN) == HIGH; }
	bool hasUcSignal() const { return digitalRead(SIGNAL_UC_PIN) == HIGH; }
	bool isAlternatorOn() const { return digitalRead(SIGNAL_ALTERNATOR_PIN) == HIGH; }
	bool isImobilizerOn() const { return digitalRead(SIGNAL_IMOBILIZER_PIN) == HIGH; }
	bool isAirConditioningOn() const { return digitalRead(SIGNAL_AIR_CONDITIONING_PIN) == HIGH; }
	bool isDefrostOn() const { return digitalRead(SIGNAL_DEFROST_PIN) == HIGH; }
	bool isAirRecirculationOn() const { return digitalRead(SIGNAL_AIR_RECIRCULATION_PIN) == HIGH; }
};

#endif