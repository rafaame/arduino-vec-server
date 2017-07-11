#include "DefaultModule.h"
#include "DataController.h"
#include "StateController.h"

DefaultModule::DefaultModule(EthernetController *ethernetController, DataController *dataController, StateController *stateController, RelayController *relayController, MotorController *motorController, StepperController *stepperController) :
	Module(ethernetController, dataController, stateController, relayController, motorController, stepperController) {
	
}

void DefaultModule::dispatch(const EthernetClient *client, const RequestPacket *requestPacket) {
	uint8_t buffer[1024];
	buffer[0] = 0;

	uint8_t length = requestPacket->toByteArray(buffer + 1) + 1;
	dataController->send(buffer, length);
	
	bool success = dataController->hasSuccess();
	if (success) {
		State state = stateController->getState();
		if (requestPacket->module == "comfort") {
			requestPacket->action == "set-ventilator-position" && (state.ventilatorPosition = (uint8_t) requestPacket->data->i);
			requestPacket->action == "set-ventilator-speed" && (state.ventilatorSpeed = (uint8_t) requestPacket->data->i);
			requestPacket->action == "set-air-conditioning" && (state.isAirConditioningOn = requestPacket->data->b);
			requestPacket->action == "set-air-recirculation" && (state.isAirRecirculationOn = requestPacket->data->b);
			requestPacket->action == "set-hazard-lights" && (state.areHazardLightsOn = requestPacket->data->b);

			stateController->setState(state);
		}
	}

	JsonMap jsonResponse;
	jsonResponse.insert(make_pair(string("success"), JsonValue(success)));
	string response = httpResponse(false, stringifyJson(jsonResponse));
	client->write(response.c_str());
	client->flush();
	client->stop();
}