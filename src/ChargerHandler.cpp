#include "ChargerHandler.h"
#include "RelayController.h"

using namespace std;

ChargerHandler::ChargerHandler(RelayController *relayController) {
    charging = false;
    lastTimeChanged = 0;

    this->relayController = relayController;
}

ChargerHandler::~ChargerHandler() {
    
}

void ChargerHandler::init() {
    if (CHARGER_DEBUG) {
        Serial.println("Initializing charger manager...");
    }
    
    relayController->init();
    reset();
}

void ChargerHandler::reset() {
    if (CHARGER_DEBUG) {
        Serial.println("Resetting charger manager...");
    }

    turnChargingOn();

    lastTimeChanged = millis();
}

void ChargerHandler::update() {
    uint64_t currentTime = millis();

    Serial.print("currentTime: ");
    Serial.println((int) (currentTime / 1000));

    Serial.print("lastTimeChanged: ");
    Serial.print((int) (lastTimeChanged / 1000));

    Serial.print("diff: ");
    Serial.println((int) (currentTime - lastTimeChanged));

    if (currentTime - lastTimeChanged > 1000 * (charging ? TIME_CHARGER_ON : TIME_CHARGER_OFF)) {
        if (CHARGER_DEBUG) {
            Serial.print("Charged for ");
            Serial.print((int) (currentTime - lastTimeChanged));
            Serial.print(" seconds, turning charger ");
            Serial.println(charging ? "off..." : "on...");
        }

        if (charging) {
            turnChargingOff();
        } else {
            turnChargingOn();
        }

        lastTimeChanged = currentTime;
    }
}

void ChargerHandler::turnChargingOn() {
    relayController->setHigh(RELAY_1);

    charging = true;
}

void ChargerHandler::turnChargingOff() {
    relayController->setLow(RELAY_1);

    charging = false;
}