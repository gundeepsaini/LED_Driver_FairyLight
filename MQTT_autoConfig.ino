

/******************** LIB **************************************/
#define MQTT_MAX_PACKET_SIZE 1024  // < ----- Change in lib: This is because the defaul maxium length is 128b. So just go to PubSubClient.h and change #define MQTT_MAX_PACKET_SIZE 128 to #define MQTT_MAX_PACKET_SIZE 1024

#include "src/pubsubclient/PubSubClient.h"


/********************* Var *************************************/
WiFiClient espClient;
PubSubClient client(espClient);

long lastReconnectAttempt = 0;


const uint8_t MSG_BUFFER_SIZE = 20;
char MSG_BUFFER[MSG_BUFFER_SIZE]; 



/******************** Config **************************************/
const char* mqtt_server     = SECRET_MQTT_Server;
const char* mqtt_user       = SECRET_MQTT_User;
const char* mqtt_password   = SECRET_MQTT_Pass;


#define MQTT_TOPIC_CMD1      "HA/LED_Driver_Fairy/cmd1"
#define MQTT_TOPIC_CMD2      "HA/LED_Driver_Fairy/cmd2"
#define MQTT_TOPIC_STATE1    "HA/LED_Driver_Fairy/state1"
#define MQTT_TOPIC_STATE2    "HA/LED_Driver_Fairy/state2"

#define LIGHT_ON    "ON"
#define LIGHT_OFF   "OFF"


// Will Topic - Availability
#define MQTT_TOPIC_WILL       "HA/LED_Driver_Fairy/status"     
#define MQTT_OFFLINE          "Offline"
#define MQTT_ONLINE           "Active"



#define MQTT_TOPIC_CMD3      "HA/LED_Driver_Fairy/cmd3"
#define MQTT_TOPIC_CMD4      "HA/LED_Driver_Fairy/cmd4"
#define MQTT_TOPIC_STATE3    "HA/LED_Driver_Fairy/state3"
#define MQTT_TOPIC_STATE4    "HA/LED_Driver_Fairy/state4"


/**************** External Functions ************************************/

void MQTT_config()
{
	client.setServer(mqtt_server, SECRET_MQTT_Port);
 	client.setCallback(MQTT_MessageRecd_callback);
}


void MQTT_loop()
{
	if (!client.connected())
    	MQTT_reconnect();              
  
  	client.loop();
}


void MQTT_publish()
{   
    if (LED_State_1)
      client.publish(MQTT_TOPIC_STATE1, LIGHT_ON, true);
    else
      client.publish(MQTT_TOPIC_STATE1, LIGHT_OFF, true);
    if (LED_State_2)
      client.publish(MQTT_TOPIC_STATE2, LIGHT_ON, true);
    else
      client.publish(MQTT_TOPIC_STATE2, LIGHT_OFF, true);

    if (External_Relay_1_state)
      client.publish(MQTT_TOPIC_STATE3, LIGHT_ON, true);
    else
      client.publish(MQTT_TOPIC_STATE3, LIGHT_OFF, true);
    if (External_Relay_2_state)
      client.publish(MQTT_TOPIC_STATE4, LIGHT_ON, true);
    else
      client.publish(MQTT_TOPIC_STATE4, LIGHT_OFF, true);
}


/**************** Internal Functions ************************************/

void MQTT_reconnect() 
{
  if (millis()/1000 - lastReconnectAttempt > 30 || lastReconnectAttempt == 0) 
  {
      Serial.println("MQTT reconnecting");
      
      //boolean connect (clientID, [username, password], [willTopic, willQoS, willRetain, willMessage], [cleanSession])
      if (client.connect(DeviceHostName, mqtt_user, mqtt_password, MQTT_TOPIC_WILL, 1, true, MQTT_OFFLINE)) 
      {
        Serial.println("MQTT connected");

        client.subscribe(MQTT_TOPIC_CMD1);
        client.subscribe(MQTT_TOPIC_CMD2);
        client.subscribe(MQTT_TOPIC_CMD3);
        client.subscribe(MQTT_TOPIC_CMD4);        

        client.publish(MQTT_TOPIC_WILL, MQTT_ONLINE, true);
      }      
      lastReconnectAttempt = millis()/1000;
  }
}




void MQTT_MessageRecd_callback(char* p_topic, byte* p_payload, unsigned int p_length) 
{
  String payload;
  
  for (uint8_t i = 0; i < p_length; i++) 
    { payload.concat((char)p_payload[i]); }

  if (String(MQTT_TOPIC_CMD1).equals(p_topic)) 
  {
    if (payload.equals(String(LIGHT_ON)))                      
          LED_Turn_on(1);        
    else 
      if (payload.equals(String(LIGHT_OFF)))         
          LED_Turn_off(1);
  } 

  else 
  {
    if (String(MQTT_TOPIC_CMD2).equals(p_topic)) 
    {
      if (payload.equals(String(LIGHT_ON)))              
            LED_Turn_on(2);        
      else 
        if (payload.equals(String(LIGHT_OFF))) 
            LED_Turn_off(2);
    } 
  }

  if (String(MQTT_TOPIC_CMD3).equals(p_topic)) 
    {
      if (payload.equals(String(LIGHT_ON)))              
            External_Relay_1_state = 1;          
      else 
        if (payload.equals(String(LIGHT_OFF))) 
            External_Relay_1_state = 0; 
    } 

  if (String(MQTT_TOPIC_CMD4).equals(p_topic)) 
    {
      if (payload.equals(String(LIGHT_ON)))              
            External_Relay_2_state = 1;          
      else 
        if (payload.equals(String(LIGHT_OFF))) 
            External_Relay_2_state = 0; 
    } 

  MQTT_publish();
}

//------------------------------------------


void LED_Config()
{
  pinMode(LED_Strip_1, OUTPUT);
  pinMode(LED_Strip_2, OUTPUT);

  digitalWrite(LED_Strip_1, LED_State_1);
  digitalWrite(LED_Strip_2, LED_State_2);
}



void LED_Turn_on(int which_led)
{
  if (which_led == 1)
  {
    LED_State_1 = 1;
    digitalWrite(LED_Strip_1, LED_State_1);
  }
  if (which_led == 2)
  {
    LED_State_2 = 1;
    digitalWrite(LED_Strip_2, LED_State_2);
  }
}


void LED_Turn_off(int which_led)
{
  if (which_led == 1)
  {
    LED_State_1 = 0;
    digitalWrite(LED_Strip_1, LED_State_1);
  }
  if (which_led == 2)
  {
    LED_State_2 = 0;
    digitalWrite(LED_Strip_2, LED_State_2);
  }
}

