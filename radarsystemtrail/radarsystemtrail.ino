  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
  #include <NewPing.h>

  // WiFi settings
  const char* ssid = "srinivas";
  const char* password = "rishi1234";

  // Web server
  ESP8266WebServer server(80);
  int D1=5;
  int D2=4;
  int D3=0;
  int D4=2;
  int D5=14;
  int D6=12;
  int D7=13;
  int D8=15;
  // Ultrasonic sensor pins
  const int frontTriggerPin = D1;  // Use D1 notation
  const int frontEchoPin = D2;     // Use D2 notation
  const int rightTriggerPin = D3;  // Use D3 notation
  const int rightEchoPin = D4;     // Use D4 notation
  const int backTriggerPin = D5;   // Use D5 notation
  const int backEchoPin = D6;      // Use D6 notation
  const int leftTriggerPin = D7;   // Use D7 notation
  const int leftEchoPin = D8;      // Use D8 notation

  // Create NewPing instances
  NewPing frontSensor(frontTriggerPin, frontEchoPin, 400);
  NewPing rightSensor(rightTriggerPin, rightEchoPin, 400);
  NewPing backSensor(backTriggerPin, backEchoPin, 400);
  NewPing leftSensor(leftTriggerPin, leftEchoPin, 400);

  void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi");
    Serial.println("IP address: " + WiFi.localIP().toString());

    server.on("/", handleRoot);
    server.on("/data", handleData); // Handle data request

    server.begin();
  }

  void loop() {
    server.handleClient();
  }

  void handleRoot() {
    int frontDistance = frontSensor.ping_cm();
    int rightDistance = rightSensor.ping_cm();
    int backDistance = backSensor.ping_cm();
    int leftDistance = leftSensor.ping_cm();

    // Ensure the maximum box size is 400
    frontDistance = min(frontDistance, 400);
    rightDistance = min(rightDistance, 400);
    backDistance = min(backDistance, 400);
    leftDistance = min(leftDistance, 400);

    String html = "<html><head>";
    html += "<meta http-equiv=\"refresh\" content=\"5\">";  // Auto-refresh every 5 seconds
    html += "<style>";
    html += "body { margin: 0; display: flex; align-items: center; justify-content: center; height: 100vh; background-color: #f0f0f0; }";
    html += ".radar { width: 400px; height: 400px; border: 1px solid black; position: relative; }";
    html += ".end-box { position: absolute; width: 30px; height: 30px; background-color: green; transform: translate(-50%, -50%); }";
    html += ".end-box.front { top: " + String(200 - frontDistance) + "px; left: 50%; }";
    html += ".end-box.right { left: " + String(200 + rightDistance) + "px; top: 50%; transform: translateY(-50%); }";
    html += ".end-box.back { top: " + String(200 + backDistance) + "px; left: 50%; }";
    html += ".end-box.left { left: " + String(200 - leftDistance) + "px; top: 50%; transform: translateY(-50%); }";
    html += ".side-distance { position: absolute; font-size: 12px; font-weight: bold; color: black; }";
    html += ".side-distance.front { top: " + String(200 - frontDistance - 15) + "px; left: 50%; transform: translateX(-50%); }";
    html += ".side-distance.right { top: 50%; left: " + String(200 + rightDistance + 5) + "px; transform: translateY(-50%); }";
    html += ".side-distance.back { top: " + String(200 + backDistance + 5) + "px; left: 50%; transform: translateX(-50%); }";
    html += ".side-distance.left { top: 50%; left: " + String(200 - leftDistance - 20) + "px; transform: translateY(-50%); }";
    html += "</style>";
    html += "</head><body>";
    html += "<div class=\"radar\">";
    html += "<div class=\"end-box front\"></div>";
    html += "<div class=\"end-box right\"></div>";
    html += "<div class=\"end-box back\"></div>";
    html += "<div class=\"end-box left\"></div>";
    html += "<div class=\"side-distance front\">Front: " + String(frontDistance) + " cm</div>";
    html += "<div class=\"side-distance right\">Right: " + String(rightDistance) + " cm</div>";
    html += "<div class=\"side-distance back\">Back: " + String(backDistance) + " cm</div>";
    html += "<div class=\"side-distance left\">Left: " + String(leftDistance) + " cm</div>";
    html += "</div>";
    html += "</body></html>";

    server.send(200, "text/html", html);
  }

  void handleData() {
    int frontDistance = frontSensor.ping_cm();
    int rightDistance = rightSensor.ping_cm();
    int backDistance = backSensor.ping_cm();
    int leftDistance = leftSensor.ping_cm();

    String data = "{\"frontDistance\":" + String(frontDistance) +
                  ",\"rightDistance\":" + String(rightDistance) +
                  ",\"backDistance\":" + String(backDistance) +
                  ",\"leftDistance\":" + String(leftDistance) + "}";

    server.send(200, "application/json", data);
  }
