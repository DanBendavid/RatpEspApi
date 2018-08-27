
//Air Quality Open Data Platform  http://aqicn.org

#ifndef RATPESPApi_h
#define RATPESPApi_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Client.h>

#define RATPESP_HOST "api-ratp.pierre-grimaud.fr"

struct RATPESPResponse{

String schedules0_message; // "INFO INDISPO ...."
String schedules0_destination; // "Parc de Saint-Cloud"
String schedules1_message; // "INFO INDISPO ...."
String schedules1_destination; // "Parc de Saint-Cloud"
String metadata_call ; // "GET /schedules/bus/52/la+muette+++boulainvilliers+rer/R"
String metadata_date ; // "2018-08-27T03:48:03+02:00"
int metadata_version; // 3

String error;
};


class RATPESPApi
{
  public:
    RATPESPApi (Client &client);
    String SendGetToRATPESP(String command);
    RATPESPResponse GetRATPESPInfo(String StationB);
    int Port = 443;

  private:
    Client *client;
    void closeClient();
};

#endif
