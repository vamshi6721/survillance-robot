
const int MR1 = 12;
const int MR2 = 14; 
const int ML1 = 27;
const int ML2 = 26;
const int trigPin = 5;
const int echoPin = 18;
#define SOUND_SPEED 0.034
long duration;
float distanceCm;
#include <Servo.h>
#include <WiFi.h>

const char* ssid     = "Nani";
const char* password = "nani18101";
Servo myservo;
WiFiServer server(80);

void setup()
{
    Serial.begin(115200);
    pinMode(5, OUTPUT);
    pinMode(MR1, OUTPUT);
  pinMode(MR2, OUTPUT);
  myservo.attach(13);
  pinMode(ML1, OUTPUT);
  pinMode(ML2, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);
    // set the LED pin mode

    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.begin();

}

int value = 0;

void loop(){
 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("<pre><br><br><br><br><br><br>"
"                    <button><a href=\"/B\">Forward</a></button><br><br>"
"          <button><a href=\"/L\">Left</a></button>      <button><a href=\"/S\">Stop</a></button>       <button><a href=\"/R\">Right</a></button><br><br>"
"                   <button><a href=\"/F\">Backward</a></button><br></pre>");
            client.print("<button><a href=\"/C\">Current</a></button>");
            client.print("<button><a href=\"/E\">Down</a></button>");
            client.print("<button><a href=\"/D\">Up</a></button>");
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else 
          {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') 
        {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        if (currentLine.endsWith("GET /R")) {
           digitalWrite(MR1,LOW);
      digitalWrite(MR2,HIGH);
      digitalWrite(ML1,LOW);
      digitalWrite(ML2,HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(MR1,HIGH);
      digitalWrite(MR2,LOW);
      digitalWrite(ML1,HIGH);
      digitalWrite(ML2,LOW);               // GET /L turns the LED off
        }
        if (currentLine.endsWith("GET /F")) {
           digitalWrite(MR1,HIGH);//MOVE FRONT
      digitalWrite(MR2,LOW); //MOVE BACK
      //LEFT MOTOR
      digitalWrite(ML1,LOW);//MOVE BACK
      digitalWrite(ML2,HIGH);//MOVE FRONT   
      }
      if (currentLine.endsWith("GET /B")) {
          digitalWrite(MR1,LOW);
      digitalWrite(MR2,HIGH);
      digitalWrite(ML1,HIGH);
      digitalWrite(ML2,LOW);    
      }
      if (currentLine.endsWith("GET /S")) 
      {
          digitalWrite(MR1,LOW); 
          digitalWrite(MR2,LOW);
          digitalWrite(ML1,LOW); 
          digitalWrite(ML2,LOW);               
      }
      //current=135
      //down=180
      //up=90
      //too up=0
      if (currentLine.endsWith("GET /C")) 
      {
           myservo.write(135);              
      }
      if (currentLine.endsWith("GET /D")) 
      {
           myservo.write(180);              
      }
      if (currentLine.endsWith("GET /E")) 
      {
           myservo.write(90);   
           digitalWrite(35,HIGH);           
      }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distanceCm = duration * SOUND_SPEED/2;
    Serial.print("Distance (cm): ");
    Serial.println(distanceCm);
    if(distanceCm>=10)
    {
          digitalWrite(MR1,LOW); 
          digitalWrite(MR2,LOW);
          digitalWrite(ML1,LOW); 
          digitalWrite(ML2,LOW);   
    }
    }
  }
