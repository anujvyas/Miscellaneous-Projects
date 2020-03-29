#include<Blynk.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Keypad.h>

#define Password_Lenght 5 // Give enough room for 4 chars + NULL char
char Data[Password_Lenght]; // 4 is the number of chars it can hold + the null char = 5
char Master[Password_Lenght]= "4466";
byte data_count = 0, master_count = 0;
bool Pass_is_good;
char customKey;
int correct= 15;
int wrong= 2;
int wrong_email = 0;

const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
bool door = true;
byte rowPins[ROWS] = {0, 4, 5, 16}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {13, 12, 14}; //connect to the column pinouts of the keypad
Keypad customKeypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); //initialize an instance of class NewKeypad



char auth[] = "dHON-oA9BzDHSItcujInBT9PFQrrg4tu"; //Put ur tokn here
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "J"; //put wifi name here
char pass[] = "janhavi11"; //wifi password
BlynkTimer timer;



void setup()
{
    // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
  pinMode(correct, OUTPUT);
  pinMode(wrong, OUTPUT);
  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
  delay(3000);
}
//0 stands for door open
//1 stands for door closed
void loop()
{
  Blynk.run();
  timer.run();
  if (door == 0)
  {
    customKey = customKeypad.getKey();

    if (customKey == '#')

    {
      door = 1;
      Blynk.virtualWrite(V4, "System is locked.");
      digitalWrite(correct, LOW); // turn the LED on
      digitalWrite(wrong, HIGH); // turn the LED on

    }
  }

  else Open();
}

void clearData()
{
  while (data_count != 0)
  { // This can be used for any array size,
    Data[data_count--] = 0; //clear array for new data
  }
  return;
}

void Open()
{
  customKey = customKeypad.getKey();
  if (customKey) // makes sure a key is actually pressed, equal to (customKey != NO_KEY)
  {
    Data[data_count] = customKey;
    Blynk.virtualWrite(V5, Data);// store char into data array
    data_count++; // increment data array by 1 to store new char, also keep track of the number of chars entered
  }

  if (data_count == Password_Lenght - 1) // if the array index is equal to the number of expected chars, compare data to master
  {
    if (!strcmp(Data, Master)) // equal to (strcmp(Data, Master) == 0)
    {
      door = 0;
      wrong_email = 0;
      digitalWrite(correct, HIGH); // turn the LED on
      digitalWrite(wrong, LOW); // turn the LED on
      Blynk.virtualWrite(V4, "Access Granted");
    }
    else
    {
      delay(1000);
      door = 1;
      wrong_email = wrong_email + 1;
      Blynk.virtualWrite(V4, "Access Denied.");
      Blynk.notify("Alert: Wrong Password.");
      digitalWrite(wrong, HIGH); // turn the LED on
      digitalWrite(correct, LOW); // turn the LED on
    }
    clearData();
  }
  if (wrong_email == 3)
  {
    Blynk.email("janhavi.zarapkar@gmail.com","Access Denied","Alert!, Someone tried to access your system with invaid password more than 2 consecutive time.");
    wrong_email = 0;
  }
}
void myTimerEvent()
{
  Blynk.virtualWrite(V5, Data);
}