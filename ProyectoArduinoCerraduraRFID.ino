#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
 
#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27,20,4);
Servo myservo;

byte interroChar[] = {
  0x00,
  0x04,
  0x00,
  0x04,
  0x08,
  0x10,
  0x11,
  0x0E
};

int ServoClose= 90;
int ServoOpen= 0;
int EstadoServo= ServoClose;

void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  lcd.init();
  lcd.backlight();
  lcd.print("hola");
  mfrc522.PCD_Init();   // Initiate MFRC522
  myservo.attach(4);
  myservo.write(EstadoServo);
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  lcd.createChar(0, interroChar);
  lcd.home();

}
void loop() 
{
  lcd.clear();
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "97 2F 03 0A") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    lcd.setCursor(0, 0);
    lcd.print("ACCESO PERMITIDO");
    lcd.setCursor(3, 1);
    lcd.print("BIENVENIDO");
    myservo.write(ServoOpen);
    Serial.println();
    delay(3000);
    myservo.write(ServoClose);
    delay(3000);
  }
 
 else   {
    Serial.println(" Access denied");
    lcd.setCursor(0, 0);
    lcd.print("ACCESO  DENEGADO");
    lcd.setCursor(2, 1);
    lcd.write(0);
    lcd.print("QUIEN ERES?");
    delay(3000);
  }
} 
