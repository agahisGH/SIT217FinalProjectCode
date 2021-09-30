// Including the necessary libraries for application
#include <SoftwareSerial.h>
#include <ArduinoBlue.h>
#include <DHT.h>

// Define constants for DHT sensor
#define DHTPIN 4      //Pin 4 on arduino connected to sensor
#define DHTTYPE DHT22 //DHT22 sensor is being used (aka AM2302)

// MOTOR A (right side)
int in1 = 13;
int in2 = 12;

// MOTOR B (left side)
int in3 = 11;
int in4 = 10;

// Initialize constants for DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Bluetooth TX in the Arduino D8 pin
const int BLUETOOTH_TX = 8;

// Bluetooth RX in the Arduino D7 pin
const int BLUETOOTH_RX = 7;

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
ArduinoBlue phone(bluetooth); // passes the reference of the bluetooth object, to the ArduinoBlue constructor (application)

void setup()
{
    // Starts serial monitor at 9600 bps
    Serial.begin(9600);
    
    // Starts the DHT sensor
    dht.begin();

    // Starts bluetooth serial at 9600 bps
    bluetooth.begin(9600);

    // Delay 100ms (0.1 sec) just in case the bluetooth module needs a bit of time to get ready
    delay(100);

    // Prints a message in serial to confirm it's ready to go
    Serial.println("setup is now complete.");

    // Sets up all motor pins as output
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);

    // Sets up the LED pins as output
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);

    pinMode(3, OUTPUT);
}

// Moves both motors forwards
void moveForward()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

// Moves right motors forwards
void moveRight()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

// Moves left motors forwards
void moveLeft()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

// Moves both motors backwards
void moveBackward()
{
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

// Turn off all motors
void moveStop()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

// Turns on both LEDs
void ledOn()
{
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);  
}

// Turns off both LEDs
void ledOff()
{
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
}

// Runs a check for temperature and humidity values, and prints data into serial monitor
void dhtSensorCheck()
{
  // Reads and stores data for temperature and humidity into the float variables created
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if ((temp < 21) && (hum < 60)) // Set to closer values for testing/demo purposes
  {
    Serial.println("OPTIMAL temperature & humidity levels detected"); // prints message into serial monitor
    digitalWrite(3, HIGH); // turns on green LED
  } // else if at least one of the variables are not met, then the LED will automatically turn off as if statement doesn't become valid

  // Prints data readings into serial monitor for temperature
  Serial.print("Temperature = ");
  Serial.print(temp);
  Serial.print(" *C ");
  
  // Prints data readings into serial monitor for humidity
  Serial.print("     Humidity = ");
  Serial.print(hum);
  Serial.println(" % ");

  // Delays, and only therefore allows it to check, every 1 second
  delay(1000);
}

void loop()
{
    // Returns the text data sent from the phone
    // After it returns the latest data, empty string "" is sent in subsequent and calls until text data is sent again
    String str = phone.getText();

    dhtSensorCheck();

    // If a text from the phone "Forward" is received, move robot forwards, and print movement command in serial monitor
    if (str == "Forward")
    {
        Serial.println(str);
        moveForward();
    }

    // If a text from the phone "Right" is received, move robot towards right direction, and print movement command in serial monitor
    if (str == "Right")
    {
        Serial.println(str);
        moveRight();
    }

    // If a text from the phone "Left" is received, move robot towards left direction, and print movement command in serial monitor
    if (str == "Left")
    {
        Serial.println(str);
        moveLeft();
    }

    // If a text from the phone "Backward" is received, move robot backwards, and print movement command in serial monitor
    if (str == "Backward")
    {
        Serial.println(str);
        moveBackward();
    }

    // If a text from the phone "Stop" is received, stop robot movement, and print movement command in serial monitor
    if (str == "Stop")
    {
        Serial.println(str);
        moveStop();
    }

    // If a text from the phone "On" is received, the robot's headlights will turn on, and print the command in serial monitor
    if (str == "On")
    {
        Serial.println(str);
        ledOn();
    }

    // If a text from the phone "Off" is received, the robot's headlights will turn off, and print the command in serial monitor
    if (str == "Off")
    {
        Serial.println(str);
        ledOff();
    }
}
