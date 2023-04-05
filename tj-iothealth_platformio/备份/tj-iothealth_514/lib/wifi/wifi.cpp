#include "wifi.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiManager.h> //

#include <ArduinoJson.h>
#include <EEPROM.h>
#include <FS.h>

#include <ESP8266HTTPClient.h>
#include <base64.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
//static WiFiClient espClient;

//OLED_SSD1306 myOLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
WiFiManager wifiManager;

int tjulogin();
int tjulogin_v1(char *host,char *username, char *password);
//void webconnect(OLED_SSD1306 &myOLED);
//void initSystem();
//void initWifiManager();
boolean autoConnect(OLED_SSD1306 &myOLED);
boolean autoConnect(char const *apName,OLED_SSD1306 &myOLED,char const *apPassword = NULL);

void configModeCallback(WiFiManager *myWiFiManager);
void saveConfigCallback();
String Read_String(int a, int b);
void Write_String(int a, int b, String str);

String ssid, password, DeviceName0, DeviceSecret0;
bool shouldSaveConfig = false;
char DeviceName[32] = "esp8266-1";
char DeviceSecret[64] = "178e8e484b40eacec500c901f90c4e59";

void wifi::wifi_Init(OLED_SSD1306 &myOLED)
{
  ESP8266WiFiMulti wifiMulti; // 建立ESP8266WiFiMulti对象,对象名称是 'wifiMulti'

  //WiFiManager wifiManager;

  // 清除ESP8266所存储的WiFi连接信息以便测试WiFiManager工作效果
  //wifiManager.resetSettings();
  //Serial.println("ESP8266 WiFi Settings Cleared");
  ////
  WiFi.begin();
  //wifiMulti.cleanAPlist();
  wifiMulti.addAP("TJ-GUEST-WIFI", "");          //校园无线网
  wifiMulti.addAP("Tongj.portal", "9901050429"); //手机热点
  wifiMulti.addAP("305", "123456789+");          //宿舍无线网
  //wifiMulti.addAP("bgl5605500", "ab990429");     //家庭无线网
  //wifiMulti.addAP("bgl5605500-5G", "ab990429");  //家庭无线网
  if (wifiMulti.existsAP("TJ-GUEST-WIFI", ""))
  {
    Serial.println("TJ-GUEST-WIFI在列表中");
  }
  else
  {
    Serial.println("TJ-GUEST-WIFI不在列表中");
  }
  Serial.println("Connecting ...");

  myOLED.setCursor(0, 3); //起点坐标
  myOLED.setTextSize(1);  // Draw 2X-scale text
  myOLED.clearDisplay();
  myOLED.println("Connecting ...");
  myOLED.display();
  delay(2000);

  int i = 0;
  while ((wifiMulti.run() != WL_CONNECTED) && i < 75)
  { //等待
    delay(1000);
    Serial.print(i++);
    Serial.print(' ');
  }
  //对于需要验证的网络进行处理
  if (WiFi.SSID() == "TJ-GUEST-WIFI")
  {
    //if (tjulogin() == 0)
    if (tjulogin_v1("192.168.192.10","1751054", "Lcj990429") == 0)
    {
      Serial.println("TJU Login Success!");
    }
    else
    {
      Serial.println("TJU Login Fail!");
    }
  }
  delay(500);

  //连接成功后的信息展示

  //String wifiinfo="Connected to "+WiFi.SSID();
  myOLED.clearDisplay();
  myOLED.setCursor(0, 3); //起点坐标
  myOLED.setTextSize(1);  // Draw 2X-scale text
  myOLED.println("Connected to:");
  myOLED.setCursor(0, 13); //起点坐标
  myOLED.setTextSize(1);   // Draw 2X-scale text
  myOLED.println(WiFi.SSID());
  myOLED.display();
  delay(3000);

  Serial.println('\n');                         // WiFi连接成功后
  Serial.print("Connected to ");                // NodeMCU将通过串口监视器输出。
  Serial.println(WiFi.SSID());                  // 连接的WiFI名称
  Serial.print("IP address:\t");                // 以及
  Serial.println(WiFi.localIP());               // NodeMCU的IP地址
  Serial.print("ESP8266 Board MAC Address:  "); //
  Serial.println(WiFi.macAddress());            //开发板的mac地址
  return;
};
//针对需要验证的网页

void wifi::wifimanager_Init(OLED_SSD1306 &myOLED)
{

  // 建立WiFiManager对象
  //WiFiManager wifiManager;

  //重置wifi配置信息
  //wifiManager.resetSettings();
  // 自动连接WiFi。以下语句的参数是连接ESP8266时的WiFi名称
  //autoConnect("AutoConnectAP", myOLED);
  //Serial.print("WiFi Connected!");
  EEPROM.begin(1024);
  //iFi.mode(WIFI_STA);
  webconnect(myOLED);

  Serial.println("以下为EEPROM读取的信息：");
  String count = Read_String(EEPROM.read(800), 816);
  delay(50);
  Serial.println(count);
  String pass = Read_String(EEPROM.read(804), 856);
  delay(50);
  Serial.println(pass);
  String dn = Read_String(EEPROM.read(808), 884);
  delay(50);
  Serial.println(dn);
  String ds = Read_String(EEPROM.read(812), 932);
  delay(50);
  Serial.println(ds);

  Serial.print("WiFi Connected!");
  myOLED.clearDisplay();
  myOLED.setCursor(0, 3); //起点坐标
  myOLED.setTextSize(1);  // Draw 2X-scale text
  myOLED.println("Connected to:");
  myOLED.setCursor(0, 11); //起点坐标
  myOLED.setTextSize(1);   // Draw 2X-scale text
  myOLED.println(WiFi.SSID());
  myOLED.display();
  delay(3000);
  //httpClientRequest();
};

void wifi::drawInfo(OLED_SSD1306 &myOLED)
{
  //OLED_SSD1306::clearDisplay();

  //display.setTextSize(2);             // Normal 1:1 pixel scale
  //display.setTextColor(SSD1306_WHITE);        // Draw white text
  //display.setCursor(0,0);             // Start at top-left corner

  //myOLED.drawBitmap(0, 12, wen, 16, 16, 1); //在坐标X:26  Y:16的位置显示中文字符心
  //myOLED.drawBitmap(16, 12, du, 16, 16, 1); //在坐标X:42  Y:16的位置显示中文字符率

  myOLED.setCursor(0, 12);  //起点坐标
  myOLED.setTextSize(1, 2); // Draw 2X-scale text
  myOLED.setTextColor(SSD1306_WHITE);
  //char temp[20];
  //sprintf(temp, "MAC Address:%s", WiFi.macAddress());
  myOLED.println("MAC:");
  myOLED.setCursor(24, 12); //起点坐标
  myOLED.setTextSize(1, 2); // Draw 2X-scale text
  myOLED.setTextColor(SSD1306_WHITE);
  myOLED.println(WiFi.macAddress());

  myOLED.setCursor(0, 30);  //起点坐标
  myOLED.setTextSize(1, 2); // Draw 2X-scale text
  myOLED.setTextColor(SSD1306_WHITE);
  //char temp[20];
  //sprintf(temp, "MAC Address:%s", WiFi.macAddress());
  myOLED.println("WiFi:");
  myOLED.setCursor(30, 30); //起点坐标
  myOLED.setTextSize(1, 2); // Draw 2X-scale text
  myOLED.setTextColor(SSD1306_WHITE);
  myOLED.println(WiFi.SSID());
  //OLED_SSD1306::setCursor(38, 14); //起点坐标
  //OLED_SSD1306::setTextSize(1, 2); // Draw 2X-scale text
  // OLED_SSD1306::setTextColor(SSD1306_WHITE);
  // char temp[6];

  //OLED_SSD1306::setCursor(62, 14); //起点坐标
  //OLED_SSD1306::setTextSize(1, 2); // Draw 2X-scale text
  //OLED_SSD1306::setTextColor(SSD1306_WHITE);
  //OLED_SSD1306::println("C");

  //OLED_SSD1306::display();
  //delay(100);
};
/*
boolean autoConnect(OLED_SSD1306 &myOLED)
{
  //默认的AP ssid,芯片ID
  String ssid = "ESP" + String(ESP.getChipId());
  return autoConnect(ssid.c_str(), myOLED, NULL);
}

boolean autoConnect(char const *apName, OLED_SSD1306 &myOLED, char const *apPassword)
{
  Serial.println("AutoConnect");
  // read eeprom for ssid and pass
  //从flash中读取ssid和password
  //String ssid = getSSID();
  //String pass = getPassword();

  // attempt to connect; should it fail, fall back to AP
  //首先默认为STA模式
  WiFi.mode(WIFI_STA);

  //建立连接
  if (wifiManager.connectWifi("", "") == WL_CONNECTED)
  {
    Serial.println("IP Address:");
    Serial.println(WiFi.localIP());
    //connected
    return true;
  }
  //若连接失败，启动AP模式
  //显示屏显示需要连接AP信息
  myOLED.clearDisplay();
  myOLED.setCursor(0, 3);   //起点坐标
  myOLED.setTextSize(1, 2); // Draw 2X-scale text
  myOLED.println("please Connecte to:");
  myOLED.setCursor(0, 20);  //起点坐标
  myOLED.setTextSize(1, 2); // Draw 2X-scale text
  myOLED.println(apName);
  myOLED.setCursor(0, 37);  //起点坐标
  myOLED.setTextSize(1, 2); // Draw 2X-scale text
  myOLED.println("IP:192.168.4.1");
  myOLED.display();
  return wifiManager.startConfigPortal(apName, apPassword);
}
*/


///////////////////////////////////////////////////新添加的内容
/**
   功能描述：初始化wifimanager
*/
void wifi::webconnect(OLED_SSD1306 &myOLED)
{
  /***  步骤一：创建 wifimanager对象 **/
  WiFiManager wifiManager; //实例化WiFiManager

  /*** 步骤二：进行一系列配置，参考配置类方法 **/
  // 重置保存的修改 目标是为了每次进来都是去掉配置页面

  //wifiManager.resetSettings();



  // 配置连接超时
  //wifiManager.setConnectTimeout(1000);
  // 打印调试内容
  //wifiManager.setDebugOutput(true);
  // 设置最小信号强度
  //  wifiManager.setMinimumSignalQuality(30);
  // 设置固定AP信息
  //IPAddress _ip = IPAddress(192, 168, 1, 10);
  //IPAddress _gw = IPAddress(192, 168, 1, 1);
  //IPAddress _sn = IPAddress(255, 255, 255, 0);
  //wifiManager.setAPStaticIPConfig(_ip, _gw, _sn);
  // 设置进入AP模式的回调
  wifiManager.setAPCallback(configModeCallback);
  // 设置点击保存的回调
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  // 设置 如果配置错误的ssid或者密码 退出配置模式
  //  wifiManager.setBreakAfterConfig(true);
  // 设置过滤重复的AP 默认可以不用调用
  wifiManager.setRemoveDuplicateAPs(true);
  // 添加额外的参数 比如加入  apikey 后面可以结合onenet使用
  //  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
  WiFiManagerParameter custom_DeviceName("DeviceName", "DeviceName", DeviceName, 32);
  WiFiManagerParameter custom_DeviceSecret("DeviceSecret", "DeviceSecret", DeviceSecret, 64);
  //  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_DeviceName);
  wifiManager.addParameter(&custom_DeviceSecret);

  /*************************************/
  /*** 步骤三：尝试连接网络，失败去到配置页面 **/
  // ssid 命名为ESP8266wifiAP pwd是12345678

  //if (!autoConnect("ESP8266wifiAP",myOLED))
  if (!wifiManager.autoConnect("ESP8266wifiAP",myOLED))
  {
    Serial.println("连接失败并超时");
    //    ESP.reset();
    ESP.restart();
    delay(1000);
  }

  /*************************************/
  // 读取配置页面配置好的信息
  //  strcpy(mqtt_server, custom_mqtt_server.getValue());
  //      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
  //      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());

  ssid = WiFi.SSID();
  password = WiFi.psk();
  Serial.println(ssid);
  Serial.println(password);
  //  WiFi.printDiag(Serial);

  strcpy(DeviceName, custom_DeviceName.getValue());
  DeviceName0 = String(DeviceName);
  Serial.println(DeviceName0);
  strcpy(DeviceSecret, custom_DeviceSecret.getValue());
  DeviceSecret0 = String(DeviceSecret);
  Serial.println(DeviceSecret0);
  // 保存自定义信息

  if (shouldSaveConfig)
  {
    Serial.println("saving config");
    StaticJsonDocument<128> doc;
    JsonObject root = doc.to<JsonObject>();
    JsonObject json = root.createNestedObject("json");
    json["ssid"] = ssid;
    json["password"] = password;
    json["DeviceName"] = DeviceName;
    json["DeviceSecret"] = DeviceSecret;

    serializeJson(root, Serial);
    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile)
    {
      Serial.println("failed to open config file for writing");
    }
    configFile.close();
    shouldSaveConfig = false;
    Write_String(800, 816, ssid);
    delay(30);
    Write_String(804, 856, password);
    delay(30);
    Write_String(808, 884, DeviceName0);
    delay(30);
    Write_String(812, 932, DeviceSecret0);
    delay(30);
  }

  Serial.print("local ip:");
  Serial.println(WiFi.localIP());
  yield();
}


void wifi::webconfig(OLED_SSD1306 &myOLED){
  wifiManager.startConfigPortal(myOLED);
}
/**
   功能描述：配置进入AP模式通知回调
*/
void configModeCallback(WiFiManager *myWiFiManager)
{
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

/**
   功能描述：设置点击保存的回调
*/
void saveConfigCallback()
{
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

//a写入字符串长度，b是起始位，str为要保存的字符串
void Write_String(int a, int b, String str)
{
  EEPROM.write(a, str.length()); //EEPROM第a位，写入str字符串的长度
  //把str所有数据逐个保存在EEPROM
  for (int i = 0; i < str.length(); i++)
  {
    EEPROM.write(b + i, str[i]);
  }
  EEPROM.commit();
}
//a位是字符串长度，b是起始位
String Read_String(int a, int b)
{
  String data = "";
  //从EEPROM中逐个取出每一位的值，并链接
  for (int i = 0; i < a; i++)
  {
    data += char(EEPROM.read(b + i));
  }
  return data;
}

int tjulogin()
{
  const char *host = "192.168.192.10";
  const int httpPort = 80;

  WiFiClient client; //建立客户端

  if (!client.connect(host, httpPort))
  {
    Serial.println("connection failed"); //成功建立连接
    return 1;
  }
  //Serial.println(" Success!");
  delay(10);
  String postDate = "action=login&username=1751054&password={B}TGNqOTkwNDI5&ac_id=1'&save_me=1&ajax=1"; //将从串口接收的数据发送到服务器，readLine()方法可以自行设计
  if (postDate.length() && postDate != "0")
  {
    String data = (String)postDate;
    int length = data.length();
    String postRequest = (String)("POST ") + "/include/auth_action.php HTTP/1.1\r\n" +
                         "Host: " + host + "\r\n" +
                         "Connection: Keep Alive\r\n" +
                         "Content-Length: " + length + "\r\n" +
                         "Accept: */*\r\n" +
                         "Origin: http://192.168.192.10\r\n" +
                         "Content-Type: application/x-www-form-urlencoded" + "\r\n" +
                         "User-Agent: lcjESP8266\r\n" +
                         "\r\n" +
                         data + "\r\n";
    Serial.println();
    client.print(postRequest); // 向服务器发送HTTP请求
    //Serial.print(postRequest); //打印请求
    //Serial.println("test3");
    delay(600);
    //处理返回信息
    String line = client.readStringUntil('\n');
    //Serial.println("line is");
    //Serial.print(line);
    while (client.available() > 0)
    {
      line += client.readStringUntil('\n');
    }
    //  Serial.println(line);
    client.stop();
    if (line.indexOf("OK") != -1)
    { //认证成功
      return 0;
    }
    else
    {
      return 2;
    }
  }
}

//测试输入账号密码的方式连接校园网，校园网密码采用base64加密方式
int tjulogin_v1(char *host,char *username, char *password)
{
  //const char *host = "192.168.192.10";
  const int httpPort = 80;
  String toencode=(String)password;
  String encode =base64::encode(toencode);
  WiFiClient client; //建立客户端
  Serial.println(encode); //成功建立连接
  if (!client.connect(host, httpPort))
  {
    Serial.println("connection failed"); //成功建立连接
    return 1;
  }
  //Serial.println(" Success!");
  delay(10);
  String postDate = "action=login&username="+ (String)username +"&password={B}"+encode+"&ac_id=1'&save_me=1&ajax=1"; //将从串口接收的数据发送到服务器，readLine()方法可以自行设计
  //String postDate = "action=login&username="+ (String)username +"&password={B}TGNqOTkwNDI5&ac_id=1'&save_me=1&ajax=1"; //将从串口接收的数据发送到服务器，readLine()方法可以自行设计
  if (postDate.length() && postDate != "0")
  {
    String data = (String)postDate;
    int length = data.length();
    String postRequest = (String)("POST ") + "/include/auth_action.php HTTP/1.1\r\n" +
                         "Host: " + host + "\r\n" +
                         "Connection: Keep Alive\r\n" +
                         "Content-Length: " + length + "\r\n" +
                         "Accept: */*\r\n" +
                         "Origin: "+ host +"0\r\n" +
                         "Content-Type: application/x-www-form-urlencoded" + "\r\n" +
                         "User-Agent: lcjESP8266\r\n" +
                         "\r\n" +
                         data + "\r\n";
    Serial.println();
    client.print(postRequest); // 向服务器发送HTTP请求
    //Serial.print(postRequest); //打印请求
    //Serial.println("test3");
    delay(600);
    //处理返回信息
    String line = client.readStringUntil('\n');
    //Serial.println("line is");
    //Serial.print(line);
    while (client.available() > 0)
    {
      line += client.readStringUntil('\n');
    }
    //  Serial.println(line);
    client.stop();
    if (line.indexOf("OK") != -1)
    { //认证成功
      return 0;
    }
    else
    {
      return 2;
    }
  }
}