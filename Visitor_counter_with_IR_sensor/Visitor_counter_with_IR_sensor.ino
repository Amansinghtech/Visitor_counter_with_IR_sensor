#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#define IR1 10 //IR sensor 1 at pin 10
#define IR2 11 //IR sensor 2 at pin 11
#define Relay 2 // relay Pin 
#define wait 3000 //Duration between 1st IR sensor to 2nd IR sensor
int Persons = 0; //Number of persons inside the room
int Exited = 0; //Number of persons exited the room
int Visited = 0; //Number of persons visited

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

//Flash Screen showing the college name and Department
void FlashScreen() {
  lcd.setCursor(0,0);
  lcd.print("B.R.P. GOVT POLY DMT");
  lcd.setCursor(0,1);
  lcd.print("  Electrical Dept.  ");
  lcd.setCursor(0,2);
  lcd.print("6th Sem (2017-2020)");
  delay(3000);
  lcd.clear();

  //Loading screen for fun
  lcd.setCursor(3,0);
  lcd.print("Hello Stranger");
  lcd.setCursor(0,1);
  int i;
  lcd.print("LOADING:");
  for (i=0; i<11; i++ ){
    delay(300);
    lcd.print("=");
  }
  lcd.print("*");
  
  lcd.setCursor(0,2);
  lcd.print("Booted Firmware OK!");
  lcd.setCursor(8,3);
  lcd.print("****");
  delay(2000);

  //Program screen for counters
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Counting Visitors");
  lcd.setCursor(0,1);
  lcd.print("Persons Inside: 0"); //16,1
  lcd.setCursor(0,2);
  lcd.print("Persons Exited: 0"); //16,2
  lcd.setCursor(0,3);
  lcd.print("Persons Visited: 0"); //17, 3
}

void setup() {
  //Serial.begin(9600);
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(Relay, OUTPUT);
  digitalWrite(Relay, LOW);
  //being the LED screen
  lcd.begin(20,4);
  FlashScreen(); //shows the flash Screen
}

//When sensors sense for any person coming in
void PersonIn(){
  Persons++;
  Visited++;
  if (Exited < 0)
  Exited = 0;
  if (Persons < 0)
  Persons = 0;
}

//When sensors sense for any person going
void PersonOut(){
  //some LCD function
  Persons--;
  Exited++;
  if (Exited > Visited)
  Exited = Visited;
  if (Persons < 0)
  Persons = 0;
}

void loop() {
  int counter = 0;
  TriggerRelay();
  //when someone gets in
  if (digitalRead(IR2))
  {
    while (counter != wait)
    {
      if (digitalRead(IR1))
      {
        //Serial.println("Person In");
        PersonIn();
        break;
      }
      delay(100);
      counter +=100;
    }
    ShowValues();
    delay(1000);
    return 0;
  }
  
//for if someone comes out
  counter = 0;
  if (digitalRead(IR1))
  {
    while (counter != wait)
    {
      if (digitalRead(IR2))
      {
        //some calling function
        //Serial.println("Person out");
        PersonOut();
        break;
      }
      delay(100);
      counter += 100;
    }
    
    ShowValues();
    delay(1000);
    return 0;
  } 
}

//This function will Toggle Relay Switch
void TriggerRelay(){
  if (Persons == 0)
    digitalWrite(Relay, LOW);
  else
    digitalWrite(Relay, HIGH);
}

//Updating Values on Screen
void ShowValues(){
  Serial.println(Persons);
  lcd.setCursor(16,1);
  lcd.print(Persons);
  lcd.setCursor(16,2);
  lcd.print(Exited);
  lcd.setCursor(17,3);
  lcd.print(Visited);

  //bug fix incorrect decimals
  if (Persons < 1000){
    lcd.setCursor(19,1);
    lcd.print(" ");
  }
  
  if (Persons < 100){
    lcd.setCursor(18,1);
    lcd.print("  ");
  }
  
  if (Persons < 10){
    lcd.setCursor(17,1);
    lcd.print("   ");
  }

  if (Exited < 1000){
    lcd.setCursor(19,2);
    lcd.print(" ");
  }
  
  if (Exited < 100){
    lcd.setCursor(18,2);
    lcd.print("  ");
  }
  
  if (Exited < 10){
    lcd.setCursor(17,2);
    lcd.print("   ");
  }

  if (Visited < 1000){
    lcd.setCursor(20,3);
    lcd.print(" ");
  }
  
  if (Visited < 100){
    lcd.setCursor(19,3);
    lcd.print("  ");
  }
  
  if (Visited < 10){
    lcd.setCursor(18,3);
    lcd.print("   ");
  }
}
