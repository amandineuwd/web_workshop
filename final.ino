#include <WiFi.h>
#include <WebSocketsServer.h>
#include <math.h>

const char* ssid = "PoleDeVinci_IFT";
const char* password = "*c.r4UV@VfPn_0";

WebSocketsServer webSocket = WebSocketsServer(81);

// LED RGB
#define RED1 5
#define GRN1 7
#define BLU1 6

// LED
#define LED_PIN 8

// Capteur de température
#define TEMP_PIN 2 

// Calcul de la température
double Thermister(int RawADC) {
    if (RawADC <= 0) {
        Serial.println("Error: RawADC is 0 or negative. Check sensor connection.");
        return NAN;
    }

    double Temp;
    Temp = log(((10240000 / RawADC) - 10000));
    Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp)) * Temp);
    Temp = Temp - 273.15; // Convertir de Kelvin à Celsius
    Temp = (Temp - 32) * 5.0 / 9.0; // Convertir en degrés Celsius

    return Temp;
}

void setup() {
    Serial.begin(115200);

    pinMode(RED1, OUTPUT);
    pinMode(GRN1, OUTPUT);
    pinMode(BLU1, OUTPUT);
    pinMode(LED_PIN, OUTPUT);

    // Connexion au Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    Serial.print("ESP32 IP address: ");
    Serial.println(WiFi.localIP());

    // Démarrer le serveur WebSocket
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    Serial.println("WebSocket server started");
}

void loop() {
    // Lire la température
    int sensorValue = analogRead(TEMP_PIN);
    double temperature = Thermister(sensorValue);

    // Vérifier que la lecture de la température est valide
    if (!isnan(temperature)) {
        String tempString = String(temperature, 2);
        webSocket.broadcastTXT("Temperature:" + tempString); // Diffuser la température via WebSocket
        Serial.println("Temperature: " + tempString + " °C");
    }

    webSocket.loop();

    delay(1000);  // Attendre avant de prendre la prochaine lecture
}

bool ledState = false;  // Variable pour suivre l'état actuel de la LED (éteinte par défaut)

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    if (type == WStype_TEXT) {
        String message = String((char*)payload);
        Serial.printf("Received message: %s\n", message.c_str());

        // Bascule l'état de la LED quand TOGGLE_LED est reçu
        if (message == "TOGGLE_LED") {
            ledState = !ledState;  // Inverser l'état de la LED
            digitalWrite(LED_PIN, ledState ? HIGH : LOW);  // Allumer ou éteindre la LED
            Serial.println(ledState ? "LED ON" : "LED OFF");
        }

        // Réception de l'intensité lumineuse
        if (message.startsWith("BRIGHTNESS:")) {
            int brightness = message.substring(11).toInt();
            analogWrite(RED1, brightness);  // Appliquer la luminosité
            Serial.println("Brightness set to: " + String(brightness));
        }

        // Si le message est un code couleur #RRGGBB
        if (message.startsWith("#") && message.length() == 7) {
            int r = strtol(message.substring(1, 3).c_str(), NULL, 16);
            int g = strtol(message.substring(3, 5).c_str(), NULL, 16);
            int b = strtol(message.substring(5, 7).c_str(), NULL, 16);

            // Appliquer la couleur aux LEDs RGB
            analogWrite(RED1, r);
            analogWrite(GRN1, g);
            analogWrite(BLU1, b);

            Serial.printf("Set RGB to R:%d, G:%d, B:%d\n", r, g, b);
        }
    }
}


/* JOYSTICK

// Pins du joystick
#define JOY_X_PIN 34
#define JOY_Y_PIN 35
#define JOY_SW_PIN 32

void setup() {
    // Configurer les pins du joystick
    pinMode(JOY_X_PIN, INPUT);
    pinMode(JOY_Y_PIN, INPUT);
    pinMode(JOY_SW_PIN, INPUT_PULLUP);
}

void loop() {
    // Lire les valeurs du joystick
    int joyX = analogRead(JOY_X_PIN);
    int joyY = analogRead(JOY_Y_PIN);
    bool joySwitch = digitalRead(JOY_SW_PIN) == LOW;

    // Envoyer les valeurs du joystick via WebSocket
    String joystickData = "JOYSTICK:" + String(joyX) + "," + String(joyY) + "," + (joySwitch ? "CLICK" : "NO_CLICK");
    webSocket.broadcastTXT(joystickData);
}

*/