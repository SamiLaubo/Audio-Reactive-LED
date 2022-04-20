//  Modified version of https://github.com/scottlawsonbc/audio-reactive-led-strip by Scott Lawson
//  Written by Sami Laubo


// Libraries
#include <NeoPixelBus.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>


// Wifi and socket settings
const char* SSID     = "Get-2G-34D521";    // Wifi SSID - NB: Must be 2GHz wifi
const char* PASSWORD = "AGB7R877GC";    // Wifi password - Blank if open network 
unsigned int LOCAL_PORT = 7777;          // Local port - choose yourself


// Network setting
WiFiUDP PORT;
IPAddress IP(192, 168, 0, 150);     // ESP8266 ip: Choose yourself but must match the IP in controller.py (and don't set to wifi-ip etc.)
IPAddress GATEWAY(192, 168, 0, 1);  // Routers gateway (obtain by "ipconfig" in terminal)
IPAddress SUBNET(255, 255, 255, 0); // Routers subnet (obtain by "ipconfig" in terminal)


// LED and buffer
constexpr int PIN = 3;              // Output pin for led strip
constexpr int NUM_LEDS = 300;       // Number of individuall LED lights
constexpr int BUFFER_LEN = 1024;    // Length of buffer
char PACKET_BUFFER[BUFFER_LEN];     // Buffer
uint16_t N = 0;                      // Variable for later use

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> ledstrip_audio(NUM_LEDS, PIN);


// Setup
void setup() {
    delay(1000);        // Hardware safety

    Serial.begin(9600); // Begin serial communication

    // Connect to wifi
    WiFi.mode(WIFI_STA);
    WiFi.config(IP, GATEWAY, SUBNET);
    WiFi.begin(SSID, PASSWORD);

    Serial.println("");
    int i = 0;
    while (WiFi.status() != WL_CONNECTED && i < 20) {   // Try to connect 20 times
        delay(500);
        Serial.print("*");
        ++i;
    }

    if (WiFi.status() == WL_CONNECTED)  // If connection succesful
    {
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
    }
    else    // Connection not succesful
    {
        Serial.println("");
        Serial.println("WiFi connection Unsuccessful");
    }   

    // Ledstrip output
    ledstrip_audio.Begin();   // Begin output
    ledstrip_audio.Show();    // Clear the strip for use
}

void loop() {
    int packetSize = PORT.parsePacket();    // Read data over socket

    if (packetSize) {   // If packets have been received, interpret the command
        int len = PORT.read(PACKET_BUFFER, BUFFER_LEN);

        for(int i = 0; i < len; i+=5) {
            PACKET_BUFFER[len] = 0;
            // N = PACKET_BUFFER[i];   // N is the pixel number
            // RgbColor pixel((uint8_t)PACKET_BUFFER[i+1], (uint8_t)PACKET_BUFFER[i+2], (uint8_t)PACKET_BUFFER[i+3]); // color
            N = ((PACKET_BUFFER[i] << 8) + PACKET_BUFFER[i + 1]);
            RgbColor pixel((uint8_t)PACKET_BUFFER[i+2], (uint8_t)PACKET_BUFFER[i+3], (uint8_t)PACKET_BUFFER[i+4]);
            ledstrip_audio.SetPixelColor(N, pixel);   
        } 

        ledstrip_audio.Show();
    }
}
