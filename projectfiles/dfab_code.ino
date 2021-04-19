#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

#define servoon
#define servooff

Servo myservo;

const char *ssid = "DFAB Project";
const char *password = "123456789"; //leave blank if you want an open network
String  t = "UP";

ESP8266WebServer server ( 80 );
String getPage(){
  String page = "<html lang=fr-FR><head><meta http-equiv='refresh' content='10'/>";
  page += "<title>ESP8266 Remote Control</title>";
  page += "<style> body { background-color: #fffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style>";
  page += "</head><body><h1>ESP8266 Remote Control Light Switch</h1>";
  page += "<p><h3>Switch status : </h3>";
  page += t;
  page +="</p>";
  page +="<form action='/' method='POST'>";
  //page += "<ul><li>";
  //page += "<INPUT type='radio' name='DX80' value='1'>ON";
  //page += "<INPUT type='radio' name='DX80' value='0'>OFF</li></ul>";
  //page += "<INPUT type='submit' value='Trigger DX80'>";
  page +="<p>Switch Remote Control: </p>";
  page +="<button name='DX80' type='submit' value='1'>ON (DOWN)</button>";
  page +="                                           ";
  page +="<button name='DX80' type='submit' value='0'>OFF (UP)</button>";
  page +="</form>";
  page += "</body></html>";
  return page;
}
void handleRoot(){
  if ( server.hasArg("DX80") ) {
    handleSubmit();
  } else {
    server.send ( 200, "text/html", getPage() );
  }  
}

void handleSubmit()
  {
    String DX80Value;
    DX80Value = server.arg("DX80");
    Serial.print("Set DX80 to:"); Serial.println(DX80Value);
    
      if ( DX80Value == "1" )
        {           
          t = " SERVO DOWN";
          server.send ( 200, "text/html", getPage() );
          digitalWrite(LED_BUILTIN, 1);
          myservo.write(45);
          delay(1000);
          myservo.write(90);
        } 
    
      else if ( DX80Value == "0" )
        {
          t = "SERVO UP";
          server.send ( 200, "text/html", getPage() );
          digitalWrite(LED_BUILTIN, 0);
          myservo.write(135);
          delay(1000);
          myservo.write(90);
        }
      else
        {
          Serial.println("Error DX80 Value");
        }
}void setup() {
  myservo.attach(5);
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.softAP(ssid, password);
  delay(1000);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  Serial.print("Server MAC address: ");
  Serial.println(WiFi.softAPmacAddress());  // link to the function that manage launch page
  server.on ( "/", handleRoot ); 
  server.begin();
  Serial.println ( "HTTP server started" );
 
}

void loop()
{
  server.handleClient();
}
