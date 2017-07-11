# arduino-vec-server

## Purpose

This project is used as an information server and controller interface for [arduino-vec-controller](https://github.com/rafaame/arduino-vec-controller) project.

## Platform
This code runs on an [Arduino Mega 2560](https://www.arduino.cc/en/Main/ArduinoBoardMega2560) with a [W5100 Ethernet Shield](https://www.arduino.cc/en/Main/ArduinoEthernetShieldV1). Also, an OBD2 to UART transceiver using an [ELM327](https://www.elmelectronics.com/ic/elm327/) is connected to the Arduino using serial ports.

This server reads car data from various sources (OBD2 interface, 32-bit cable from the instrument panel and signals intercepted from various logic boards) using the Arduino both digital and analog pins.

The W5100 Ethernet Shield is connected to a router located in the car.

## Operation
During boot, the server initializes the libraries that are responsible for communicating with the W5100 Ethernet Shield, the 32-bit instrument panel cable along with signals intercepted from other logic boards and the ELM327 OBD2 to UART transceiver.

During each cycle (each call to the Arduino *loop* function) the following happens:
 
 - The data is read from the 32-bit instrument panel cable and stored as a [DataPacket](#structure-of-the-datapacket);
  - From time to time data is also read from the OBD2 interface (which is significantly slower);
 
 - Broadcast the DataPacket instance to all attached network clients;
  - Send a copy of this data to the arduino-vec-controller using serial communication;
 
 - Check if there is an incoming [network request](#request-and-response-format);
  - If so and if it is an attach request, register the client as an attached client;
  - Otherwise forward it to the arduino-vec-controller, wait for its response and send the response back to the client.

## Structure of the DataPacket
An instance of the DataPacket class represents the state of the car according to last read data. 

Most of the data is obtained from the 32-bit cable from the instrument panel (using both analog and digital pins and functions like [digitalRead](https://www.arduino.cc/en/Reference/DigitalRead) and [pulseIn](https://www.arduino.cc/en/Reference/PulseIn)). Note that this data is read in every cycle.

However, some data still needs to be read from the OBD2 interface. This data is not read in every cycle, as the communication with the OBD2 interface is significantly slower than issuing several *digitalRead* and *pulseIn* calls.

The structure of the DataPacket is as follows:

| Member | Data Type | Data Source | Description |
| ------ | --------- | ----------- | ----------- |
| engineRpm | uint16_t | 32-bit instrument panel cable | The engine RPM |
| speed | uint8_t | 32-bit instrument panel cable | The vehicle speed |
| coolantTemp | uint8_t | OBD2 interface | The engine coolant temperature |
| fuelLevel | uint8_t | 32-bit instrument panel cable | The fuel tank level |
| isDriverPresent | bool | generated |  |
| isTurnLeftOn | bool | 32-bit instrument panel cable | Whether the turn left signal light is on or not |
| isTurnRightOn | bool | 32-bit instrument panel cable | Whether the turn right signal light is on or not |
| isHeadlampOn | bool | 32-bit instrument panel cable | Whether the headlamp is on or not |
| hasOpenDoor | bool | 32-bit instrument panel cable | True if a door is open |
| hasOilPressure | bool | 32-bit instrument panel cable | True if there is oil pressure in the engine |
| isParkingBrakeOn | bool | 32-bit instrument panel cable | True if the parking brake is activated |
| isKeyOnFirstPos | bool | signal from car key contact mechanism | True if the key is at least in the first position of the key contact |
| isKeyAfterFirstPos | bool | signal from car key contact mechanism | True if the key is at least in the second position of the key contact |
| isCranking | bool | signal from car key contact mechanism | True while the key is at the crank position of the key contact |
| hasDiagnosisSignal | bool | 32-bit instrument panel cable | Unknown |
| hasUcSignal | bool | 32-bit instrument panel cable | Unknown |
| isAlternatorOn | bool | signal from car alternator | True if the alternator is charging the car battery |
| isImobilizerOn | bool | 32-bit instrument panel cable | Unknown |
| isAirConditioningOn | bool | signal from AC/ventilator logic board | Whether the air conditioning is on or not |
| isDefrostOn | bool | signal from AC/ventilator logic board | Whether the defrost is on or not |
| isAirRecirculationOn | bool | signal from AC/ventilator logic board | Whether the air recirculation is on or not |
| ventilatorPosition | uint8_t | recorded state |  |
| ventilatorSpeed | uint8_t | recorded state |  |
| areHazardLightsOn | bool | signal from door locks/hazard lights logic board | Whether the hazard lights are on or not |


## Request and Response Format
To receive network requests, the arduino-vec-server uses the W5100 Ethernet Shield to open a TCP server that listens to the port specified in const.h.

The server mimics a RESTful API served over an HTTP server. Both the request and the response bodies are in the JSON format.

A request has the following format:
module (string): the module responsible to execute the desired action
action (string): the desired action
data (integer, string or bool): data that might be needed

The response has the following format:


