/*
  WiFi UDP Send and Receive String

  This sketch wait an UDP packet on localPort using a WiFi shield.
  When a packet is received an Acknowledge packet is sent to the client on port remotePort

  Circuit:
   WiFi shield attached

  created 30 December 2012
  by dlf (Metodo2 srl)

*/


#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>

int status = WL_IDLE_STATUS;
#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

unsigned int localPort = 2390;      // local port to listen on

char packetBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged";       // a string to send back

WiFiUDP Udp;

struct neoPixelBrightness {
  byte r;
  byte g;
  byte b;
};

const int NUM_OF_PIXELS = 12;

neoPixelBrightness pixels[NUM_OF_PIXELS];

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWiFiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  Udp.begin(localPort);
}

void loop() {

  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) packetBuffer[len] = 0;
    Serial.println("Contents:");
    Serial.println(packetBuffer);

    printNeoPixelBuffer();

    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    Udp.endPacket();
  }
}

void printNeoPixelBuffer() {

  for (int i = 0; i < NUM_OF_PIXELS; i++) {


    Serial.print("Pixel number:\t");
    Serial.print(i);

    Serial.print("\tRed:\t");
    byte redByte = (byte)(packetBuffer[i + 0]);
    char wordBuffer[7];         //the ASCII of the integer will be stored in this char array
    itoa(redByte, wordBuffer, 10); //(integer, yourBuffer, base)
    Serial.print(wordBuffer);

    Serial.print("\tGreen:\t");
    byte greenByte = (byte)(packetBuffer[i + 1]);
    // clear the buffer   char wordBuffer[7];         //the ASCII of the integer will be stored in this char array
    itoa(greenByte, wordBuffer, 10); //(integer, yourBuffer, base)
    Serial.print(wordBuffer);

    Serial.print("\tBlue:\t");
    byte blueByte = (byte)(packetBuffer[i + 2]);
    //clear the buffer   char wordBuffer[7];         //the ASCII of the integer will be stored in this char array
    itoa(blueByte, wordBuffer, 10); //(integer, yourBuffer, base)
    Serial.println(wordBuffer);

    //    Serial.print("\tcolors:\t");
    //    colors = packetBuffer[i];
    //
    //    erial.println("");
  }

}
void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}




