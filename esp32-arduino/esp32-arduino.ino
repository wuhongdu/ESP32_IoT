#include <WiFi.h>
#include <WebSocketClient.h>
#include <ArduinoJson.h>

#define LED_PIN 2   // 定义 LED的引脚

const char* ssid     = "CAD1009";
const char* password = "yyhayz19680928";
char path[] = "/echo";
char host[] = "192.168.1.108"; // 服务器的IP地址

WebSocketClient webSocketClient;
WiFiClient client;

void setup() {
  // 设备登录提交设备信息
  char JSONInfo[] = "{\"type\":\"LOGIN\",\"deviceTYPE\":\"LED\",\"deviceID\": \"kh344jk2h43j2k\",\"name\":\"小新\"}";
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(5000);
  if (client.connect(host, 8888)) { //调用WiFiClient对象的connect方法与主机建立TCP连接 端口设置为自己服务器websocket服务的端口
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed.");
  }
  webSocketClient.path = path;  //为WebSocketClient的路径分配全局变量
 
  webSocketClient.host = host;  // 为WebSocketClient的主机数据分配全局变量
  if (webSocketClient.handshake(client)) {
    Serial.println("Handshake successful");
    webSocketClient.sendData(JSONInfo);
  } else {
    Serial.println("Handshake failed.");
  }
}
void loop() {
  String data;
  StaticJsonDocument<300> JSONBuffer;
  if (client.connected()) {
//    webSocketClient.sendData("Info to be echoed back"); // 发送数据到服务器
    webSocketClient.getData(data);  // 从服务器接收数据
    if (data.length() > 0) {
//      JsonObject& parsed = JSONBuffer.parseObject(data);
      DeserializationError error = deserializeJson(JSONBuffer, data);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
      int value = JSONBuffer["value"];
      Serial.print("Received data: ");
      Serial.println(value);
      if (value == 1) {
        digitalWrite(LED_PIN,LOW); 
      } else if ( value == 2) {
        digitalWrite(LED_PIN, HIGH);
      }
    }
  } else {
    Serial.println("Client disconnected.");
  }
}
