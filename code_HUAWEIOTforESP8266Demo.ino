//连接wifi后登陆MQTT，然后每1s上报一次数据(数据每次加1)
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define WIFI_SSID "WIFI_SSID"//wifi名称
#define WIFI_PASSWORD "WIFI_PASSWORDRD"//wifi密码
//注册设备的ID和密钥
#define device_id "61fb2d7fde9933029be5ff9e_esp8266_test01"
#define secret "yourdevicesecret"
//MQTT三元组
#define ClientId "61fb2d7fde9933029be5ff9e_esp8266_test01_0_0_2022020310"
#define Username "61fb2d7fde9933029be5ff9e_esp8266_test01"
#define Password "yourMQTTsecret"

#define MQTT_Address "iot-mqtts.cn-north-4.myhuaweicloud.com"
#define MQTT_Port 1883
#define Iot_link_Body_Format "{\"services\":[{\"service_id\":\"Dev_data\",\"properties\":{%s"
//{"services":[{"service_id":"Dev_data","properties":{"temp": 39}}]}
#define Iot_link_MQTT_Topic_Report "$oc/devices/"device_id"/sys/properties/report"
WiFiClient myesp8266Client;
PubSubClient client(myesp8266Client);
int data_temp=1;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WIFI_Init();
  MQTT_Init();
}
void loop() {
  // put your main code here, to run repeatedly:
  MQTT_POST();
  delay(1000);
  data_temp++;
}
//连接wifi
void WIFI_Init()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
    while(WiFi.status()!=WL_CONNECTED)
    {
      delay(1000);
      Serial.println("WiFi Not Connect");
    }
    Serial.println("WiFi Connected OK!");
}
//连接MQTT
void MQTT_Init()
{
  client.setServer(MQTT_Address,MQTT_Port);
  while(!client.connected())
  {
    client.connect(ClientId,Username,Password);
  }
  Serial.println("MQTT Connected OK!");
}
void MQTT_POST()
{
  char properties[32];
  char jsonBuf[128];
  sprintf(properties,"\"temp\":%d}}]}",data_temp);
  sprintf(jsonBuf,Iot_link_Body_Format,properties);
  client.publish(Iot_link_MQTT_Topic_Report, jsonBuf);
  Serial.println(Iot_link_MQTT_Topic_Report);
  Serial.println(jsonBuf);
  Serial.println("MQTT Publish OK!");
}
