#ifndef ESP8266CONTROLLER_H
#define ESP8266CONTROLLER_H

#include "common.h"

#define ESP8266_DEBUG true

#define ESP8266_TIMEOUT_COMMAND 1000
#define ESP8266_TIMEOUT_RESET   5000

#define ESP8266_COMMAND_RESET       		"RST"
#define ESP8266_COMMAND_CHANGEMODE        	"CWMODE_DEF"
#define ESP8266_COMMAND_CONN_MODE   		"CIPMUX"
#define ESP8266_COMMAND_AP_CONFIG   		"CWSAP_DEF"
#define ESP8266_COMMAND_AP_SET_MAC  		"CIPAPMAC_DEF"
#define ESP8266_COMMAND_AP_SET_IP           "CIPAP_DEF"
#define ESP8266_COMMAND_AP_STATIONS         "CWLIF"
#define ESP8266_COMMAND_STA_SET_IP          "CIPSTA_DEF"
#define ESP8266_COMMAND_STA_CONN    		"CWJAP"
#define ESP8266_COMMAND_TCP_SERVER	        "CIPSERVER"
#define ESP8266_COMMAND_TCP_SERVER_TIMEOUT	"CIPSTO"
#define ESP8266_COMMAND_T_MODE   			"CIPMODE"
#define ESP8266_COMMAND_T_BEGIN     		"CIPSTART"
#define ESP8266_COMMAND_T_SEND      		"CIPSEND"
#define ESP8266_COMMAND_T_END       		"CIPCLOSE"

#define ESP8266_RESPONSE_READY       		"ready"
#define ESP8266_RESPONSE_OK          		"OK"
#define ESP8266_RESPONSE_CONN_OPENED 		"CONNECT"
#define ESP8266_RESPONSE_CONN_CLOSED 		"CLOSE"

#define ESP8266_CONNECTION_TYPE_TCP  		"TCP"
#define ESP8266_CONNECTION_TYPE_UDP  		"UDP"

enum ESP8266Mode_t {
	ESP8266_MODE_STA = 1,
	ESP8266_MODE_AP = 2,
	ESP8266_MODE_BOTH = 3
};

enum ESP8266TransferMode_t {
	ESP8266_T_MODE_NORMAL = 0,
	ESP8266_T_MODE_PASSTHROUGH = 1
};

using namespace std;

struct ESP8266Station {
	string mac;
	string ip;
};

class ESP8266CommandParams {
private:
	stringstream params;

public:
	static ESP8266CommandParams *create() {
		return new ESP8266CommandParams();
	}

	ESP8266CommandParams *push(string data) {
		if (params.str().length() > 0) {
			params << ",";
		}
		
		params << "\"" << data << "\"";

		return this;
	}

	ESP8266CommandParams *push(int data) {
		if (params.str().length() > 0) {
			params << ",";
		}

		params << data;

		return this;
	}

	string get() {
		return params.str();
	}
};

class ESP8266Controller {
private:
	HardwareSerial *serial;
	bool ready;

	stringstream responseBuffer;
	string lastCommand;
	string lastResponse;
	bool hasConnection;

public:
	ESP8266Controller();
	~ESP8266Controller();

	bool init(uint8_t serialPort, uint32_t baudRate = 115200);
	bool reset();
	bool changeMode(ESP8266Mode_t mode);
	bool changeTransferMode(ESP8266TransferMode_t mode);
	bool setMultiConnection(bool multiConnection = true);
	bool setupAp(string ssid, string password, uint8_t channel, string mac, string ip, string netmask);
	bool setupSta(string ip, string gateway, string netmask);
	bool connectToAp(string ssid, string password);
	vector<ESP8266Station> listStations();
	bool tcpServer(uint16_t port, bool create = true);
	void listenConnections(bool lookInBuffer);
	uint8_t beginTransmission(string connectionType, string remoteAddr, uint16_t remotePort, uint16_t localPort = 0);
	bool sendData(uint8_t tid, const uint8_t *data, uint64_t length);
	bool broadcastData(uint16_t remotePort, const uint8_t *data, uint64_t length);
	bool endTransmission(uint8_t tid);

	void sendCommand(string command, ESP8266CommandParams *params = NULL, const uint8_t *data = NULL);
	bool response(string response, uint64_t timeout = ESP8266_TIMEOUT_COMMAND);
	string responseData();
	vector<string> responseDataLines();
};

#endif