#include <WiFi.h>

const char* ssid = "realme 5i";
const char* password = "vishal2004";

// Relay pins (ACTIVE HIGH)
int fanPin    = 26;
int light1Pin = 25;
int light2Pin = 33;
int light3Pin = 32;

// States
bool fanState = false;
bool l1State  = false;
bool l2State  = false;
bool l3State  = false;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  pinMode(fanPin, OUTPUT);
  pinMode(light1Pin, OUTPUT);
  pinMode(light2Pin, OUTPUT);
  pinMode(light3Pin, OUTPUT);

  // 🔴 ALL OFF BY DEFAULT (ACTIVE HIGH → LOW = OFF)
  digitalWrite(fanPin, LOW);
  digitalWrite(light1Pin, LOW);
  digitalWrite(light2Pin, LOW);
  digitalWrite(light3Pin, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  Serial.println("WiFi Connected");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  String req = client.readStringUntil('\r');
  client.flush();

  // FAN
  if (req.indexOf("/FAN_ON")  != -1) { fanState = true;  digitalWrite(fanPin, HIGH); }
  if (req.indexOf("/FAN_OFF") != -1) { fanState = false; digitalWrite(fanPin, LOW);  }

  // LIGHT 1
  if (req.indexOf("/L1_ON")  != -1) { l1State = true;  digitalWrite(light1Pin, HIGH); }
  if (req.indexOf("/L1_OFF") != -1) { l1State = false; digitalWrite(light1Pin, LOW);  }

  // LIGHT 2
  if (req.indexOf("/L2_ON")  != -1) { l2State = true;  digitalWrite(light2Pin, HIGH); }
  if (req.indexOf("/L2_OFF") != -1) { l2State = false; digitalWrite(light2Pin, LOW);  }

  // LIGHT 3
  if (req.indexOf("/L3_ON")  != -1) { l3State = true;  digitalWrite(light3Pin, HIGH); }
  if (req.indexOf("/L3_OFF") != -1) { l3State = false; digitalWrite(light3Pin, LOW);  }

  // -------- UI --------
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();

  client.println("<!DOCTYPE html><html>");
  client.println("<head><meta name='viewport' content='width=device-width'>");
  client.println("<style>");
  client.println("body{font-family:Arial;text-align:center;background:#f1f5f9;}");
  client.println(".card{background:white;padding:15px;margin:10px;border-radius:10px;}");
  client.println("button{padding:10px 20px;font-size:16px;margin:5px;border:none;border-radius:8px;}");
  client.println(".on{background:#22c55e;color:white;}");
  client.println(".off{background:#ef4444;color:white;}");
  client.println("</style></head><body>");

  client.println("<h2>ESP32 Home Control</h2>");

  // FAN
  client.println("<div class='card'><h3>Fan</h3>");
  client.println(fanState ?
    "<a href='/FAN_OFF'><button class='off'>OFF</button></a>" :
    "<a href='/FAN_ON'><button class='on'>ON</button></a>");
  client.println("</div>");

  // LIGHT 1
  client.println("<div class='card'><h3>Light 1</h3>");
  client.println(l1State ?
    "<a href='/L1_OFF'><button class='off'>OFF</button></a>" :
    "<a href='/L1_ON'><button class='on'>ON</button></a>");
  client.println("</div>");

  // LIGHT 2
  client.println("<div class='card'><h3>Light 2</h3>");
  client.println(l2State ?
    "<a href='/L2_OFF'><button class='off'>OFF</button></a>" :
    "<a href='/L2_ON'><button class='on'>ON</button></a>");
  client.println("</div>");

  // LIGHT 3
  client.println("<div class='card'><h3>Light 3</h3>");
  client.println(l3State ?
    "<a href='/L3_OFF'><button class='off'>OFF</button></a>" :
    "<a href='/L3_ON'><button class='on'>ON</button></a>");
  client.println("</div>");

  client.println("</body></html>");
}
