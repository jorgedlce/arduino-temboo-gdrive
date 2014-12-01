#define TEMBOO_ACCOUNT "jorgenube"  // Your Temboo account name 
#define TEMBOO_APP_KEY_NAME "myFirstApp"  // Your Temboo app key name
#define TEMBOO_APP_KEY "280d4acc504e4a3489b59a7c9f05c269"  // Your Temboo app key

#define ETHERNET_SHIELD_MAC {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}

/* Setup shield-specific #include statements */
#include <SPI.h>
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <Temboo.h>
#include "TembooAccount.h" // Contains Temboo account information


byte ethernetMACAddress[] = ETHERNET_SHIELD_MAC;
EthernetClient client;

int TempPin = A0;
int HumPin = A1;
int DistPin = A2;

//para temperatura
float tempC;
int reading;
//para humedad
int Hu;
// para Distancia
int lectura;
int cm;



int numRuns = 1;   // Execution count, so this doesn't run forever
int maxRuns = 100;   // Maximum number of times the Choreo should be executed


void setup() {
  Serial.begin(9600);
  
  pinMode(TempPin, INPUT);// Temperatura
  pinMode(HumPin, INPUT);// Humedad
  pinMode(DistPin, INPUT);// Distancia
  
  
  // For debugging, wait until the serial console is connected.
  delay(1000);
  while(!Serial);

  Serial.print("DHCP:");
  if (Ethernet.begin(ethernetMACAddress) == 0) {
    Serial.println("FAIL");
    while(true);
  }
  Serial.println("OK");
  delay(500);

  Serial.println("Setup complete.\n");
}

void loop() {
  if (numRuns <= maxRuns) {
    Serial.println("Running AppendRow - Run #" + String(numRuns++));

    TembooChoreo AppendRowChoreo(client);

    // Invoke the Temboo client
    AppendRowChoreo.begin();

    // Set Temboo account credentials
    AppendRowChoreo.setAccountName(TEMBOO_ACCOUNT);
    AppendRowChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    AppendRowChoreo.setAppKey(TEMBOO_APP_KEY);

    // Set Choreo inputs
    String UsernameValue = "jorgeluisdc.07@gmail.com";
    AppendRowChoreo.addInput("Username", UsernameValue);
    String PasswordValue = "ybmcfixunotguchz";
    AppendRowChoreo.addInput("Password", PasswordValue);
    
    //para temperatura
    reading = analogRead(TempPin);
    tempC=(500 * reading) /1024;
    //para Humedad
    Hu=analogRead(HumPin);
    //Para Distancia
    lectura = analogRead(DistPin); // lectura del sensor 0
    cm = pow(3027.4 / lectura, 1.2134); // conversión a centímetros
    
    
    String RowDataValue = (String)tempC+"c"+","+(String)Hu+","+(String)cm+"cm";
    
    AppendRowChoreo.addInput("RowData", RowDataValue);
    String SpreadsheetTitleValue = "Arduino1";
    AppendRowChoreo.addInput("SpreadsheetTitle", SpreadsheetTitleValue);

    // Identify the Choreo to run
    AppendRowChoreo.setChoreo("/Library/Google/Spreadsheets/AppendRow");

    // Run the Choreo; when results are available, print them to serial
    AppendRowChoreo.run();

    while(AppendRowChoreo.available()) {
      char c = AppendRowChoreo.read();
      Serial.print(c);
    }
    AppendRowChoreo.close();
  }

  Serial.println("\nWaiting...\n");
  delay(1000); // wait 30 seconds between AppendRow calls
}
