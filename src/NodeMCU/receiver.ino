//  Modified version of https://github.com/scottlawsonbc/audio-reactive-led-strip by Scott Lawson
//  Written by Sami Laubo

// Libraries
#include <NeoPixelBus.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// Toggles FPS output (1 = print FPS over serial, 0 = disable output)
#define PRINT_FPS 1

// Wifi and socket settings
const char* SSID     = "";    // Wifi SSID
const char* PASSWORD = "";    // Wifi PASSWORD 
unsigned int LOCAL_PORT = 7777;          // Local port - choose yourself

// Network setting
WiFiUDP PORT;
IPAddress IP(192, 168, 0, 150);     // ESP8266 ip: Choose yourself but must match the IP in controller.py (and don't set to wifi-ip etc.)
IPAddress GATEWAY(192, 168, 0, 1);  // Routers gateway (obtain by "ipconfig" in terminal)
IPAddress SUBNET(255, 255, 255, 0); // Routers subnet (obtain by "ipconfig" in terminal)

// LED and buffer
constexpr int PIN = 3;              // Output pin for led strip
constexpr int NUM_LEDS = 256;       // Number of individuall LED lights
constexpr int BUFFER_LEN = 1024;    // Length of buffer
char packetBuffer[BUFFER_LEN];      // Buffer
uint8_t N = 0;                      // Variable for later use

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> ledstrip(NUM_LEDS, PIN);

void setup() {
    // Begin serial communication
    Serial.begin(9600);

    // Connect to wifi
    WiFi.mode(WIFI_STA);
    WiFi.config(IP, GATEWAY, SUBNET);
    WiFi.begin(SSID, PASSWORD);

    Serial.println("");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connection Successful");
    Serial.print("Connected to: ");
    Serial.println(SSID);
    Serial.print("The IP Address of ESP8266 Module is: ");
    Serial.println(WiFi.localIP());


    Serial.println("Starting UDP");
    PORT.begin(LOCAL_PORT);
    Serial.print("Local port: ");
    Serial.println(PORT.localPort());
    
    ledstrip.Begin();   //Begin output
    ledstrip.Show();    //Clear the strip for use
}

#if PRINT_FPS
    uint16_t fpsCounter = 0;
    uint32_t secondTimer = 0;
#endif

void loop() {
    int packetSize = PORT.parsePacket();    // Read data over socket

    if (packetSize) {   // If packets have been received, interpret the command
        int len = PORT.read(packetBuffer, BUFFER_LEN);
        for(int i = 0; i < len; i+=4) {
            packetBuffer[len] = 0;
            N = packetBuffer[i];
            RgbColor pixel((uint8_t)packetBuffer[i+1], (uint8_t)packetBuffer[i+2], (uint8_t)packetBuffer[i+3]);//color
            ledstrip.SetPixelColor(N, pixel);   //N is the pixel number
        } 
        ledstrip.Show();
        #if PRINT_FPS
            fpsCounter++;
            Serial.print("/");  //Monitors connection(shows jumps/jitters in packets)
        #endif
    }
    #if PRINT_FPS
        if (millis() - secondTimer >= 1000U) {
            secondTimer = millis();
            Serial.printf("FPS: %d\n", fpsCounter);
            fpsCounter = 0;
        }   
    #endif
}
