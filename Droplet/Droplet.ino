//Droplet NodeMCU project created by Ajmal VA
//https://github.com/Ajmalva/Droplet/

// Import required libraries
#include <ESP8266WebServer.h>
#define WEBSERVER_H   //defined to avoid conflict b/w libraries by ESPAsyncWebServer library
#include "ESPAsyncWebServer.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <DNSServer.h>
#include<WiFiManager.h>    //library to use the wifi anywhere


const int rainSensor = 14; //  Rain Sensor
int sensorPin = A0;    // input from rain sensor
int sensorValue2 = 0;  // variable to store the value coming from sensor Rain sensor
bool rainDetected = false;
int Sensed = 0;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Initialize Telegram BOT
#define BOTtoken "#############"  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "#####"

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);


// Indicates when Rain is detected
void ICACHE_RAM_ATTR detectsDroplet() {
  //Serial.println("RAIN DETECTED!!!");
  rainDetected = true;
}

//html file of the droplet webserver
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <title>Droplet</title>
  <style>
    @import url(https://fonts.googleapis.com/css?family=Lato:100,300,400,700,900);
#ajerez {
  position: absolute;
  top: 5px;
  right: 5px;
  z-index: 9999;
}
#ajerez img {
  box-sizing: border-box;
  width: 45px;
  height: 45px;
  border-radius: 50%;
  border: 2px solid #7482A5;
  transition: border-radius 0.2s ease-in-out;
}
#ajerez img:hover {
  border-radius: 30%;
}

body {
  background-color: #ebf2f6;
}

*, *:before, *:after {
  box-sizing: border-box;
}

.weather-wrapper {
  margin-top: 20px;
  display: flex;
  flex-direction: row;
  flex-wrap: wrap;
  justify-content: center;
}

.weather-card {
  margin: 20px 5px;
  border-radius: 20px;
  position: relative;
  overflow: hidden;
  width: 270px;
  height: 270px;
  background-color: white;
  box-shadow: 0px 0px 25px 1px rgba(50, 50, 50, 0.1);
  -webkit-animation: appear 500ms ease-out forwards;
          animation: appear 500ms ease-out forwards;
}
.weather-card h1 {
  position: absolute;
  font-family: "Lato", sans-serif;
  font-weight: 300;
  font-size: 80px;
  color: #B8B8B8;
  bottom: 0;
  left: 35px;
  opacity: 0;
  transform: translateX(150px);
  -webkit-animation: title-appear 500ms ease-out 500ms forwards;
          animation: title-appear 500ms ease-out 500ms forwards;
}
.weather-card p {
  position: absolute;
  font-family: "Lato", sans-serif;
  font-weight: 300;
  font-size: 28px;
  color: #d2d2d2;
  bottom: 0;
  left: 35px;
  -webkit-animation: title-appear 1s ease-out 500ms forwards;
          animation: title-appear 1s ease-out 500ms forwards;
}

.weather-icon {
  position: relative;
  width: 50px;
  height: 50px;
  top: 0;
  float: right;
  margin: 40px 40px 0 0;
  -webkit-animation: weather-icon-move 5s ease-in-out infinite;
          animation: weather-icon-move 5s ease-in-out infinite;
}

.sun {
  background: #FFCD41;
  border-radius: 50%;
  box-shadow: rgba(255, 255, 0, 0.1) 0 0 0 4px;
  -webkit-animation: light 800ms ease-in-out infinite alternate, weather-icon-move 5s ease-in-out infinite;
          animation: light 800ms ease-in-out infinite alternate, weather-icon-move 5s ease-in-out infinite;
}

@-webkit-keyframes light {
  from {
    box-shadow: rgba(255, 255, 0, 0.2) 0 0 0 10px;
  }
  to {
    box-shadow: rgba(255, 255, 0, 0.2) 0 0 0 17px;
  }
}

@keyframes light {
  from {
    box-shadow: rgba(255, 255, 0, 0.2) 0 0 0 10px;
  }
  to {
    box-shadow: rgba(255, 255, 0, 0.2) 0 0 0 17px;
  }
}
.cloud {
  margin-right: 60px;
  background: #b6cede;
  border-radius: 20px;
  width: 25px;
  height: 25px;
  box-shadow: #b6cede 24px -6px 0 2px, #b6cede 10px 5px 0 5px, #b6cede 30px 5px 0 2px, #b6cede 11px -8px 0 -3px, #b6cede 25px 11px 0 -1px;
}
.cloud:after {
  content: "";
  position: absolute;
  border-radius: 10px;
  background-color: transparent;
  width: 4px;
  height: 12px;
  left: 0;
  top: 31px;
  transform: rotate(30deg);
  -webkit-animation: rain 800ms ease-in-out infinite alternate;
          animation: rain 800ms ease-in-out infinite alternate;
}

@-webkit-keyframes rain {
  from {
    box-shadow: #2092A9 8px 0px, #2092A9 32px -6px, #2092A9 20px 0px;
  }
  to {
    box-shadow: #2092A9 8px 6px, #2092A9 32px 0px, #2092A9 20px 6px;
  }
}

@keyframes rain {
  from {
    box-shadow: #2092A9 8px 0px, #2092A9 32px -6px, #2092A9 20px 0px;
  }
  to {
    box-shadow: #2092A9 8px 6px, #2092A9 32px 0px, #2092A9 20px 6px;
  }
}
@-webkit-keyframes weather-icon-move {
  50% {
    transform: translateY(-8px);
  }
}
@keyframes weather-icon-move {
  50% {
    transform: translateY(-8px);
  }
}
.inspiration {
  margin-top: 80px;
  color: #709fbe;
  font-family: "Lato", sans-serif;
  font-weight: 300;
  font-size: 24px;
  text-align: center;
}
.inspiration a {
  color: #FF8F8F;
  font-weight: 400;
  -webkit-animation: all 300ms ease-in-out;
          animation: all 300ms ease-in-out;
}

@-webkit-keyframes appear {
  0% {
    transform: scale(0);
  }
  50% {
    transform: scale(1.05);
  }
  75% {
    transform: scale(0.95);
  }
  100% {
    transform: scale(1);
  }
}

@keyframes appear {
  0% {
    transform: scale(0);
  }
  50% {
    transform: scale(1.05);
  }
  75% {
    transform: scale(0.95);
  }
  100% {
    transform: scale(1);
  }
}
@-webkit-keyframes title-appear {
  from {
    opacity: 0;
    transform: translateX(150px);
  }
  to {
    opacity: 1;
    transform: translateX(0px);
  }
}
@keyframes title-appear {
  from {
    opacity: 0;
    transform: translateX(150px);
  }
  to {
    opacity: 1;
    transform: translateX(0px);
  }
}
  
  </style>
</head>
<body>
  <center>
  <h2>Droplet WebAPP</h2>
 </center>
 <div class="weather-wrapper">
    <div class="weather-card madrid">
        <div id="myDIV" class="weather-icon sun"></div>
        <h1>26&#8451;</h1>
        <p id="tsr">Its Clear</p>
    </div>
</div>

</body>
<script>


setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      
    if(this.responseText == 1){
    document.getElementById("myDIV").className ="weather-icon cloud";
      document.getElementById("tsr").innerHTML = "Water Detected";
    } else {
    document.getElementById("myDIV").className = "weather-icon sun";
      document.getElementById("tsr").innerHTML = "Its Dry";
    }
    }
  };
  xhttp.open("GET", "/sky", true);
  xhttp.send();
}, 10000 ) ;
</script>
</html>)rawliteral";  
// html page ends here


String readsen() {
  int h = Sensed;
  Serial.println(h);
  return String(h);
  }

  
// Replaces placeholder with rain values
String processor(const String& var){
  if(var == "SKY"){
  Serial.println("var");
  Serial.println(var);
    return readsen();
  }
  return String();
}

void setup() {
  
  Serial.begin(115200);
  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org

  //  Rain Sensor mode INPUT_PULLUP
  pinMode(rainSensor, INPUT_PULLUP);
  // Set rainSensor pin as interrupt, assign interrupt function and set RISING mode
  attachInterrupt(digitalPinToInterrupt(rainSensor), detectsDroplet, HIGH);

  // Attempt to connect to Wifi network:
  // using wifi manager for better portablity
  // no need of hardcodiing wifi credentials
  WiFiManager wifiManager;
  wifiManager.autoConnect("Droplet");
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  bot.sendMessage(CHAT_ID, "Hi IM Awake", "");
  
   // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/sky", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readsen().c_str());
  });
 

  // Start server
  server.begin();
}

void loop() {
  if(rainDetected){
    bot.sendMessage(CHAT_ID, "Water Detected!!", "");
    Serial.println("bot :Water Detected ");
    rainDetected = false;
  }
 sensorValue2 = analogRead(sensorPin);
sensorValue2 = constrain(sensorValue2, 150, 440); 
sensorValue2 = map(sensorValue2, 150, 440, 1023, 0); 
if (sensorValue2>= 20)
{
  Serial.println("Water is detected");
  Sensed = 1;
  }
  else
  
{
  Serial.println("No water detected");
  Sensed = 0;
  }
  Serial.print("value:  ");
  Serial.println(sensorValue2);
Serial.println();
delay(100);
}
