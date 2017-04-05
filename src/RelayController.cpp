#include "RelayController.h"

using namespace std;

RelayController::RelayController() {
    ready = false;
}

RelayController::~RelayController() {

}

void RelayController::init() {
    if (ready) {
        return;
    }

    pinMode(RELAY_1, OUTPUT);
    pinMode(RELAY_2, OUTPUT);

    ready = true;
}

void RelayController::reset() {
    setLow(RELAY_1);
    setLow(RELAY_2);
}

void RelayController::setHigh(Relay_t relay) {
    digitalWrite(relay, HIGH);
}

void RelayController::setLow(Relay_t relay) {
    digitalWrite(relay, LOW);
}