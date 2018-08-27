#include "RATPESPApi.h"

RATPESPApi::RATPESPApi(Client &client)	{
  this->client = &client;
}

String RATPESPApi::SendGetToRATPESP(String command) {
  String body="";
  body.reserve(700);
  bool finishedHeaders = false;
  bool currentLineIsBlank = true;
	long now;
	bool avail;

	if (client->connect(RATPESP_HOST, Port)) {
		Serial.println(".... connected to server");
		String a="";
		char c;
		int ch_count=0;
		client->println("GET " + command + " HTTP/1.1");
    client->println("Host: " RATPESP_HOST);
		client->println(F("User-Agent: arduino/1.0.0"));
		client->println();
		now=millis();
		avail=false;
		while (millis()-now<1500) {
			while (client->available()) {
				char c = client->read();
				//Serial.write(c);

        if(!finishedHeaders){
          if (currentLineIsBlank && c == '\n') {
            finishedHeaders = true;
          }
        } else {
          body=body+c;
          ch_count++;
        }

        if (c == '\n') {
          currentLineIsBlank = true;
        }else if (c != '\r') {
          currentLineIsBlank = false;
        }

				avail=true;
			}
			if (avail) {
				//Serial.println("Body:");
				//Serial.println(body);
				//Serial.println("END");
				break;
			}
		}
	}
  closeClient();
  int l = body.length();
  body = body.substring(3,l-5);
  //Serial.println(body);
  return body;
}

RATPESPResponse RATPESPApi::GetRATPESPInfo(String StationB) {
  // https://api.coinmarketcap.com/v1/ticker/bitcoin/
  //String command="/v1/ticker/" + coinId + "/";
  String command = "/v3/schedules/";
  command += StationB;
  command += "/R?_format=json";

 
  Serial.println(command);
  String response = SendGetToRATPESP(command);
  RATPESPResponse responseObject = RATPESPResponse();
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(response);
  if (root.success()) {

responseObject.schedules0_message = root["result"]["schedules"][0]["message"].as<String>(); // "INFO INDISPO ...."
responseObject.schedules0_destination = root["result"]["schedules"][0]["destination"].as<String>(); // "Parc de Saint-Cloud"
responseObject.schedules1_message = root["result"]["schedules"][1]["message"].as<String>(); // "INFO INDISPO ...."
responseObject.schedules1_destination = root["result"]["schedules"][1]["destination"].as<String>(); // "Parc de Saint-Cloud"

JsonObject& _metadata = root["_metadata"];
responseObject.metadata_call = _metadata["call"].as<String>(); // "GET /schedules/bus/52/la+muette+++boulainvilliers+rer/R"
responseObject.metadata_date = _metadata["date"].as<String>(); // "2018-08-27T03:48:03+02:00"
responseObject.metadata_version = _metadata["version"]; // 3
  
  
    
  } else {
    JsonObject& rootObject = jsonBuffer.parseObject(response);
    if (rootObject.containsKey("error")) {
       responseObject.error = rootObject["error"].as<String>();
    } else {
      responseObject.error = "Failed to parse JSON";
    }
  }

  return responseObject;
}

void RATPESPApi::closeClient() {
  if(client->connected()){
    client->stop();
  }
}
