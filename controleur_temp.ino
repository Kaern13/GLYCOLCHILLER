//Libraries
#include <ESP8266WiFi.h>//https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFi.h
#include <DHT.h>

//Constants
#define LED D4
// Definit la broche de l'Arduino sur laquelle la
// broche DATA du capteur est reliee
#define DHTPIN D2
// Definit le type de capteur utilise
#define DHTTYPE DHT11
// Constante mesuree avec un thermometre de reference
// qui sert a etalonner le capteur de temperature
#define DELTA_TEMPERATURE 0.7
// Declare un objet de type DHT
// Il faut passer en parametre du constructeur
// de l'objet la broche et le type de capteur
DHT dht(DHTPIN, DHTTYPE);

//Parameters
String request;
char* ssid = "********";
char* password = "********";
String nom = "ESP8266";
//Objects
WiFiServer server(80);

void setup() {
  {
    //Init Serial USB
    Serial.begin(115200);
    Serial.println(F("Initialize System"));
    //Init ESPBrowser
    Serial.print(F("Connecting to ")); Serial.println(ssid);
    WiFi.begin(ssid, password);
    // Connect to Wifi network.
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500); Serial.print(F("."));
    }
    server.begin();
    Serial.println();
    Serial.println(F("ESPBrowser initialized"));
    Serial.print(F("IP Address: "));
    Serial.println(WiFi.localIP());
    //Init WiFi server
    server.begin();
    pinMode(LED, OUTPUT);
    pinMode(A0, INPUT);
  }

  {
    Serial.begin(9600);

    // Initialise la capteur DHT11
    dht.begin();
  }
}


void loop() {
  {
  }
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  handleRequest(request);
  webpage(client);//Return webpage
}
void handleRequest(String request) { /* function handleRequest */
  ////Handle web client request
  if (request.indexOf("/light1on") > 0)  {
    digitalWrite(LED, LOW);
  }
  if (request.indexOf("/light1off") > 0)   {
    digitalWrite(LED, HIGH);
  }
}
void webpage(WiFiClient client) { /* function webpage */
  ////Send wbepage to client
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
  client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
  client.println("</head>");
  client.println("<body bgcolor = '#70706F'>");
  client.println("<hr/><hr>");
  client.println("<h1 style='color : #3AAA35;'><center> " + nom + " Device Control </center></h1>");
  client.println("<hr/><hr>");
  client.println("<br><br>");
  client.println("<br><br>");
  client.println("<center>");
  client.println("  Pin A0");
  client.println("  <input value=" + String(analogRead(A0)) + " readonly></input>");
  client.println("  </center>");
  client.println("<center>");
  client.println("Built-in LED");
  client.println("<a href='/light1on'><button>Turn On </button></a>");
  client.println("<a href='/light1off'><button>Turn Off </button></a><br />");
  client.println("</center>");
  client.println("<br><br>");
  client.println("<br><br>");
  client.println("<center>");
  client.println("<table border='5'>");
  client.println("<tr>");
  client.print("<td>LED is </td>");
  if (digitalRead(LED))
  {
    client.print("<td> OFF</td>");
  } else {
    client.print("<td> ON</td>");
  }
  client.println("</tr>");
  client.println("</table>");
  client.println("</center>");
  client.println("</body></html>");
  client.println();
  delay(1);
}

  // Recupere la temperature et l'humidite du capteur et l'affiche
  // sur le moniteur serie
  Serial.println("Temperature = " + String(dht.readTemperature()) + " °C");
  Serial.println("Humidite = " + String(dht.readHumidity()) + " %");

  // Attend 10 secondes avant de reboucler
  delay(10000);
}
