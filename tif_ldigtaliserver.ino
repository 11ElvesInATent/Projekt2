#include <SPI.h>
#include <WiFi.h>

char ssid[] = "ZyXEL_53CC";     //  your network SSID (name) 
char pass[] = "YNYNNH3RPW3EV";    // your network password

int status = WL_IDLE_STATUS;

boolean door_status = false;

WiFiServer server(80);

String HTTP_req;

void setup() {
  // start serial port for debugging purposes
  Serial.begin(9600);
  
  // Attach interrupt to pin 2
  attachInterrupt(0, setDoorStatus, RISING);

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  } 
  server.begin();
  // you're connected now, so print out the status:
  printWifiStatus();
}


void loop() {
  
  int varialbel_modstand = map(analogRead(0), 0, 1023, 1020, 0);
            
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: keep-alive");
                    client.println();
                        client.println("<!DOCTYPE html>");
                        client.println("<html>");
                        client.println("<head>");
                        client.println("<title>EN GOD EN</title>");
                        client.println("<script type='text/javascript' src='http://code.jquery.com/jquery-1.10.1.min.js'></script>");
                        client.println("<script>");
                        client.println("$(document).ready(");
                        client.println("function() {");
                        client.println("setInterval(function() {");
                        client.println("var randomnumber = Math.floor(Math.random() * 100);");
                        client.println("$('#show').text(randomnumber);");
                        client.println("}, 3000);");
                        client.println("});");
                        client.println("</script>");
                        client.println("</head>");
                        client.println("<body>");
                        client.println("<div id = 'show' align='center'></div>");
                        client.println("</body>");
                        client.println("</html>");
                        break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
      // close the connection:
      client.stop();
      Serial.println("client disonnected");
  }
}


void printWifiStatus() {
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

void setDoorStatus() {
  door_status = true;
}
