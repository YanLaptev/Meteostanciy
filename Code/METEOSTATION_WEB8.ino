#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <Adafruit_BMP085.h>
Adafruit_BMP085 bmp;
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>
#include <ArduinoJson.h>
#include "DHT.h"
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>
#include <SoftwareSerial.h>
SoftwareSerial btSerial(D7, D0); // Rx,Tx

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // Ширина OLED-дисплея, в пикселях
#define SCREEN_HEIGHT 64 // Высота OLED-дисплея, в пикселях
#define OLED_RESET     -1 // Сброс контакта # (или -1, если общий контакт сброса Arduino)
#define SCREEN_ADDRESS 0x3C ///< См. техническое описание адрес; 0x3D для 128x64, 0x3C для 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

float WEB_Temperature;
long WEB_Pressure;
float WEB_Wind;
float WEB_Humidity;
float DHT_Humidity;
float DHT_Temperature;
long BMP_Pressure;

int xx = 0;
#define DHTPIN 12 // Цифровой контакт, подключенный к датчику DHT
#define DHTTYPE DHT11 // DHT 11
DHT dht(DHTPIN, DHTTYPE);


unsigned long previousMillis = 0;  // Задержка

const char* ssid = "Xiaomi_BCD9";
const char* password = "13243546";

// Ваше доменное имя с URL-путем или IP-адрес с путем
String openWeatherMapApiKey = "b7005f3af2b6b7a9b93a3ef0df2a2056";
// Пример:
//String openWeatherMapApiKey = "bd939aa3d23ff33d3c8f5dd1dd4";

// Заменить кодом страны и городом
String city = "Penza";
String countryCode = "";

// THE DEFAULT TIMER IS SET TO 10 SECONDS FOR TESTING PURPOSES
// For a final application, check the API call limits per hour/minute to avoid getting blocked/banned
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 10 seconds (10000)
unsigned long timerDelay = 10000;

String jsonBuffer;








// Initialize Telegram BOT
#define BOTtoken "5265517725:AAEarajU2VZxTFAQ_zeY0aByb2Hnbu0qlyU"  // replace this with your bot token

#define CHAT_ID "-769242393"  //replace with your telegram user ID

#ifdef ESP8266
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int bot_delay = 1000;
unsigned long lastTimeBotRan;

const int ledPin = 2;
bool ledState = LOW;

















#define BLYNK_TEMPLATE_ID           "TMPL3os43E7j"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "qXdVcW398rK41xLhTlSota8JlxC6LcA6"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid_blynk[] = "Xiaomi_BCD9";
char pass_blynk[] = "13243546";

BlynkTimer timer;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();

  // Update state
  Blynk.virtualWrite(V1, value);
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 20000);
}


















// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {

  for (int i = 0; i < numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }

    // Print the received message
    String user_text = bot.messages[i].text;

    String your_name = bot.messages[i].from_name;

    if (user_text == "/start") {
      String welcome = "Welcome, " + your_name + ".\n";
      welcome += "Use the following commands to control your outputs or check the weather!\n\n";
      welcome += "Send /on to turn GPIO2 ON \n";
      welcome += "Send /off to turn GPIO2 OFF \n";
      welcome += "Send /get_state to request current GPIO state \n";
      welcome += "Send /T to know the temperature \n";
      welcome += "Send /P to know the Pressure \n";
      welcome += "Send /H to know the Humidity \n";
      welcome += "Send /S to know the speed of wind \n";
      welcome += "Send /ScreenAll to know the speed of wind \n";
      welcome += "Send /ScreenT to display the temperature on the screen \n";
      welcome += "Send /ScreenH to display the humidity on the screen \n";
      welcome += "Send /ScreenP to display the pressure on the screen \n";
      welcome += "Send /ScreenW to display the speed of wind \n";
      bot.sendMessage(chat_id, welcome, "");
    }


    if (user_text == "/help") {
      String help = "Здравствуйте! \n";
      help += "Используйте различные команды, для того чтобы управлять устройством или узнать метеоданные.\n\n";
      help += "/T - вывод температуры\n";
      help += "/P - вывод давления\n";
      help += "/H - вывод влажности воздуха\n";
      help += "/S - вывод скорости ветра\n";
      help += "/ScreenAll - отображение на экране устройства всех параметров\n";
      help += "/ScreenT - отображение на экране устройства температуры\n";
      help += "/ScreenH - отображение на экране устройства влажности\n";
      help += "/ScreenP - отображение на экране устройства давления\n";
      help += "/ScreenW - отображение на экране устройства скорости ветра\n";
      bot.sendMessage(chat_id, help, "");
    }

    if (user_text == "/ScreenAll") {
      bot.sendMessage(chat_id, "На экране показываются все значения", "");
      xx = 0;
    }

    if (user_text == "/ScreenT") {
      bot.sendMessage(chat_id, "На экране показывается температура", "");
      xx = 1;
    }

    if (user_text == "/ScreenH") {
      bot.sendMessage(chat_id, "На экране показывается влажность воздуха", "");
      xx = 2;
    }

    if (user_text == "/ScreenP") {
      bot.sendMessage(chat_id, "На экране показывается атмосферное давление", "");
      xx = 3;
    }

    if (user_text == "/ScreenW") {
      bot.sendMessage(chat_id, "На экране показывается скорость ветра", "");
      xx = 4;
    }


    if (user_text == "/Напиши температуру") {
      String readings = get_Temperature();
      bot.sendMessage(chat_id, "Почему всегда я?", "");
      bot.sendMessage(chat_id, readings, "");
    }

    if (user_text == "/Напиши температуру пожалуйста") {
      String readings = get_Temperature();
      bot.sendMessage(chat_id, readings, "");
    }

    if (user_text == "/T") {
      String readings = get_Temperature();
      bot.sendMessage(chat_id, readings, "");
    }
    
    if (user_text == "/P") {
      String readings = get_Pressure();
      bot.sendMessage(chat_id, readings, "");
    }

    if (user_text == "/H") {
      String readings = get_Humidity();
      bot.sendMessage(chat_id, readings, "");
    }

    if (user_text == "/S") {
      String readings = get_Wind();
      bot.sendMessage(chat_id, readings, "");
    }

    if (user_text == "/on") {
      bot.sendMessage(chat_id, "LED state set to ON", "");
      ledState = LOW;
      digitalWrite(ledPin, ledState);
    }

    if (user_text == "/fat") {
      bot.sendMessage(chat_id, "Ян,смени шрифты", "");
    }

    if (user_text == "/off") {
      bot.sendMessage(chat_id, "LED state is set to OFF", "");
      ledState = HIGH;
      digitalWrite(ledPin, ledState);
    }

    if (user_text == "/get_state") {
      if (digitalRead(ledPin)) {
        bot.sendMessage(chat_id, "LED is ON", "");
      }
      else {
        bot.sendMessage(chat_id, "LED is OFF", "");
      }
    }
  }
}






void setup() {
  //Serial.begin(9600);
 // btSerial.begin(9600);     // bluetooth module baudrate
  dht.begin();

  pinMode(0, INPUT_PULLUP);
  pinMode(A0, INPUT);
  pinMode(14, INPUT_PULLUP);










  Blynk.begin(auth, ssid_blynk, pass_blynk);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);








  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  // display.display();
  //delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  // display.drawPixel(10, 10, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  //display.display();
  // delay(2000);


  if (!bmp.begin()) {
    //Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }

  WiFi.begin(ssid, password);
  //Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  //Serial.println(WiFi.localIP());


#ifdef ESP8266
  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
#endif
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
#ifdef ESP32
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
#endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

}

void loop() {




  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
  Blynk.virtualWrite(V4, WEB_Temperature);
  Blynk.virtualWrite(V5, WEB_Humidity);
  Blynk.virtualWrite(V6, WEB_Pressure);
  Blynk.virtualWrite(V7, WEB_Wind);
  Blynk.virtualWrite(V8, DHT_Temperature);
  Blynk.virtualWrite(V9, DHT_Humidity);
  Blynk.virtualWrite(V10, BMP_Pressure);


  if (btSerial.available() > 0) {    // check if bluetooth module sends some data to esp8266
    char data = btSerial.read();  // read the data from HC-05
    xx = data;
    Serial.println(xx);
  }
  //  unsigned long currentMillis = millis();


  if (digitalRead(0) == LOW) {
    xx = xx + 1;
 ///   delay(300);
  }

  switch (xx) {
    case 0:
      disp_all();
      break;
    case 1:
      disp_temp();
      break;
    case 2:
      disp_hum();
      break;
    case 3:
      disp_press();
      break;
    case 4:
      disp_wind();
      break;
  }

  if (xx > 4) {
    xx = 0;
  }
  calculate();

  // Send an HTTP GET request
  if ((millis() - lastTime) > timerDelay) {
    // Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;

      jsonBuffer = httpGETRequest(serverPath.c_str());
      //Serial.println(jsonBuffer);
      JSONVar myObject = JSON.parse(jsonBuffer);

      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined") {
        //Serial.println("Parsing input failed!");
        return;
      }

      //Serial.print("JSON object = ");
      //Serial.println(myObject);
      // Serial.print("WEB_Temperature: ");
      //Serial.println(myObject["main"]["temp"]);
      //Serial.print("WEB_Pressure: ");
      //Serial.println(myObject["main"]["pressure"]);
      //Serial.print("WEB_Humidity: ");
      //Serial.println(myObject["main"]["humidity"]);
      //Serial.print("WEB_Wind Speed: ");
      //Serial.println(myObject["wind"]["speed"]);

      String WWEB_Temperature = JSON.stringify(myObject["main"]["temp"]);
      float Kelvin = 273.15;
      WEB_Temperature = (WWEB_Temperature.toFloat() - Kelvin);

      String WWEB_Pressure = JSON.stringify(myObject["main"]["pressure"]);
      WEB_Pressure = WWEB_Pressure.toFloat();

      String WWEB_Humidity = JSON.stringify(myObject["main"]["humidity"]);
      WEB_Humidity = WWEB_Humidity.toFloat();

      String WWEB_Wind = JSON.stringify(myObject["wind"]["speed"]);
      WEB_Wind = WWEB_Wind.toFloat();
    }
    else {
      //Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }




  if (millis() > lastTimeBotRan + bot_delay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      //Serial.println("Got Response!");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }

}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;

  // Your IP address with path or Domain name with URL path
  http.begin(client, serverName);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode > 0) {
    //Serial.print("HTTP Response code: ");
    //Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    //Serial.print("Error code: ");
    //Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;


}

void calculate() {
  DHT_Humidity = dht.readHumidity();
  DHT_Temperature = dht.readTemperature();
  BMP_Pressure = (bmp.readPressure()) / 100;
  // Serial.println(bmp.readPressure());
}

String get_Temperature() {
  String message = "Температура на DHT: " + String(DHT_Temperature) + " ºC \n";
  message += "Температура OpenWeather.com: " + String (WEB_Temperature) + " ºC \n";
  return message;
}

String get_Pressure() {
  String message = "Давление на BMP: " + String(BMP_Pressure) + " hPA \n";
  message += "Давление OpenWeather.com: " + String (WEB_Pressure) + " hPA \n";
  return message;
}

String get_Humidity() {
  String message = "Влажность на DHT: " + String(DHT_Humidity) + " % \n";
  message += "Влажность OpenWeather.com: " + String (WEB_Humidity) + " % \n";
  return message;
}

String get_Wind() {
  String message = "Скорость ветра OpenWeather.com " + String(WEB_Wind) + " м/c \n";
  return message;
}

/*void displ() {
  String Hum_DHT =   "Hum DHT:     " + String(DHT_Humidity) + " % \n";
  String Temp_DHT =  "Temp DHT:   " + String(DHT_Temperature) + " ºC \n";
  String BMP_Press = "Press BMP:   " + String(bmp.readPressure()) + " hPA \n";
  String WEB_Hum =   "Press WEB:  " + String(WEB_Pressure) + " hPA \n";
  String WEB_Win =   "Wind WEB:   " + String(WEB_Wind) + " m/s \n";
  String WEB_Temp =  "Temp WEB:  " + String(WEB_Temperature) + " ºC \n";

  display.clear(); // Очищаем экран
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, WEB_Temp);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 10, WEB_Hum);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 20, WEB_Win);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 30, Temp_DHT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 40, Hum_DHT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 50, BMP_Press);
  display.display(); // Выводим на экран
  }*/

void disp_all() {
  String WEB_Temp =  "Temp WEB: " + String(WEB_Temperature) + " C \n";
  String WEB_Press =   "Press WEB: " + String(WEB_Pressure) + " hPA \n";
  String WEB_Win =   "Wind WEB: " + String(WEB_Wind) + " m/s \n";
  String Hum_DHT =   "Hum DHT: " + String(DHT_Humidity) + " % \n";
  String Temp_DHT =  "Temp DHT: " + String(DHT_Temperature) + " C \n";
  String BMP_Press = "Press BMP: " + String(BMP_Pressure) + " hPA \n";


  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 0);            // Start at top-left corner
  display.println(WEB_Temp);


  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 10);            // Start at top-left corner
  display.println(WEB_Press);


  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 20);            // Start at top-left corner
  display.println(WEB_Win);


  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 30);            // Start at top-left corner
  display.println(Hum_DHT);

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 40);            // Start at top-left corner
  display.println(Temp_DHT);

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 50);            // Start at top-left corner
  display.println(BMP_Press);

  display.display();
}

void disp_temp() {
  String WEB_Temp =  "WEB:" + String(WEB_Temperature) + " \n";
  String Temp_DHT =  "DHT:" + String(DHT_Temperature) + " \n";

  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 10);            // Start at top-left corner
  display.println("TEMPERATURE");

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 22);            // Start at top-left corner
  display.println(WEB_Temp);

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 42);            // Start at top-left corner
  display.println(Temp_DHT);

  display.display();
}

void disp_hum() {
  String WEB_Hum =   "WEB:" + String(WEB_Humidity) + "\n";
  String Hum_DHT =   "DHT:" + String(DHT_Humidity) + "\n";

  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 10);            // Start at top-left corner
  display.println("HUMIDITY");

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 22);            // Start at top-left corner
  display.println(WEB_Hum);

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 42);            // Start at top-left corner
  display.println(Hum_DHT);

  display.display();
}

void disp_press() {
  String WEB_Press =   "WEB:" + String(WEB_Pressure) + "\n";
  String BMP_Press = "BMP:" + String(BMP_Pressure) + "\n";

  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 10);            // Start at top-left corner
  display.println("PRESSURE");

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 22);            // Start at top-left corner
  display.println(WEB_Press);

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 42);            // Start at top-left corner
  display.println(BMP_Press);

  display.display();
}

void disp_wind() {
  String WEB_Win = String(WEB_Wind) + " m/s\n";

  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 10);            // Start at top-left corner
  display.println("SPEED OF WIND");

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 30);            // Start at top-left corner
  display.println(WEB_Win);

  display.display();
}
