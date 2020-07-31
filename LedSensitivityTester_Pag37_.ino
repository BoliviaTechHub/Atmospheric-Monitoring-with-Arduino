 /**
 * Determining RGB color from a wavelength
 * The code is based on an algorithm from Dan Bruton's Color Science Page.  
 * http://www.midnightkite.com/color.html
 * by Patrick Di Justo
 * 2012 08 28
 * 
 **/

#include <LiquidCrystal.h> // incluimos la libreria de la pantalla lcd
LiquidCrystal lcd (8,9,     4,5,6,7); //reconocemos los pines de uso

#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <Wire.h> 



//Set up the Liquid Crystal Display  
#define LCDIn 3
#define LCDOut 2
SoftwareSerial mySerialPort(LCDIn, LCDOut);

//LCD Display buffers
char databuff[16];
char dispbuff[16];

// Variables needed for RGB calculations
float Gamma = 1.00;
int MaxIntensity = 255;
float fBlue;
float fGreen;
float fRed;
float Factor;

int iR;
int iG;
int iB;

//Our eyes can generally see light wavelengths between 350 and 700 nanometers.
//Here, we start the RGB Led with 350
int i = 350;

//RGB is plugged into these arduino digital pins
const int redOutPin = 3;
const int greenOutPin = 11;
const int blueOutPin = 10;

// LED to be tested is plugged into A0
int testPin = A0;    

// variables to store the value coming from the sensor
int sensorValueTest =0;  
int oldTest =0;
int peaknm =0;

//EEPROM start data
int addr=0;

//Music
int notelen = 12;
int dlx = 130;

void setup()
{
  lcd.begin(16,2); // Reconocemos y empezamos el LCD
  Serial.begin(9600);
  pinMode(LCDOut, OUTPUT);
  pinMode(LCDIn, INPUT);

//Set the RGB LED pins to output

  pinMode(redOutPin, OUTPUT);
  pinMode(greenOutPin, OUTPUT);
  pinMode(blueOutPin, OUTPUT);


// Initialize the LCD display
  mySerialPort.begin(9600);
  mySerialPort.write(0xFE); 
  mySerialPort.write(0x01); 

  // test to see if the RGB LED works
  makeColor(i);
  analogWrite(redOutPin,255-iR);
  analogWrite(greenOutPin, 255-iG);
  analogWrite(blueOutPin, 255-iB);
  delay(5000);
}

void loop()
{ 

// set the RGB LED to a specific color
  makeColor(i);
  analogWrite(redOutPin, 255-iR);
  analogWrite(greenOutPin, 255-iG);
  analogWrite(blueOutPin, 255-iB);
  delay(500);
  
// read the sensitivity of the Test LED
  sensorValueTest= analogRead(testPin);

  if (sensorValueTest > oldTest)
  {
    oldTest = sensorValueTest;
    peaknm = i;
    
  }

// Display the values on the LCD
  sprintf(databuff,"CV:%3d Cnm:%3d",sensorValueTest,i);  
  sprintf(dispbuff,"%-16s",databuff);
  lcd.print(dispbuff);

  sprintf(databuff,"XV:%3d Xnm:%3d",oldTest, peaknm);
  sprintf(dispbuff,"%-16s",databuff);
  lcd.print(dispbuff);
 lcd.print("Hola Mundo!");
  writeData();
  i++;
  

 /* lcd.setCursor (0,0); // (Columna fila) 
lcd.println(sensorValueTest); // imprimimos en la pantalla la palabra electroall en la primera fila
lcd.setCursor (0,1);     // (columna fila) 
lcd.println (peaknm); // imprimimos en la pantalla la palabra electroallm en la seguna fila*/

/* Serial.println(sensorValueTest);
Serial.println(oldTest, peaknm);
  Serial.println(databuff);
Serial.println(dispbuff);*/


  // If we've reached the upper limit of 700 nm, play a little melody  
  if (i>500) 
  { 
    for (int f = 0; f<=100; f++)
    {
     /* tone(7,196,notelen);
      delay(dlx);

      tone(7,131,notelen);
      delay(dlx);

      tone(7,261,notelen);
      delay(dlx);

      tone(7,330,notelen);
      delay(dlx);

      tone(7,294,notelen);*/
      tone(notelen, 261, 200); //envío un tono al pin del altavoz con frecuencia 261 (do) y duración 200 milisegundos. 
   delay(300); //espero 300 milisegundos desde que empezó el comnado anterior (por eso la espera es de 100 hasta el siguiente tono)
  tone(notelen, 261, 200);
   delay(300);
  tone(notelen, 293, 400);
  delay(500);
  tone(notelen, 261, 300);
   delay(400);
  tone(notelen, 349, 300);
   delay(400);
  tone(notelen, 329, 400);
  delay(3000);
    } 
    delay(10000);
  }
}



void writeData()
{
  int quotient = i/256;
  int mod = i % 256;

  EEPROM.write(addr++,quotient);
  EEPROM.write(addr++,mod);

  quotient = sensorValueTest/256;
  mod = sensorValueTest % 256;
  EEPROM.write(addr++,quotient);
  EEPROM.write(addr++,mod);
}



void makeColor(int lambda)
{
  if (lambda >= 350 && lambda <= 439)
  {
    fRed    = -(lambda - (float)440.0) / ((float)440.0 - (float)350.0);
    fGreen = (float)0.0;
    fBlue  = (float)1.0;
  }
  else if (lambda >= (float)440.0 && lambda <= (float)489.0)
  {
    fRed  = 0.0;
    fGreen = (lambda - (float)440.0) / ((float)490.0 - (float)440.0);
    fBlue = 1.0;
  }
  else if (lambda >= (float)490.0 && lambda <= (float)509.0)
  {
    fRed = 0.0;
    fGreen = 1.0;
    fBlue = -(lambda - (float)510.0) / ((float)510.0 - (float)490.0);

  }
  else if (lambda >= (float)510.0 && lambda <= (float)579.0)
  { 
    fRed = (lambda - (float)510.0) / ((float)580.0 - (float)510.0);
    fGreen = 1.0;
    fBlue = 0.0;
  }
  else if (lambda >= (float)580.0 && lambda <= (float)644.0)
  {
    fRed = 1.0;
    fGreen = -(lambda - (float)645.0) / ((float)645.0 - (float)580.0);
    fBlue = 0.0;
  }
  else if (lambda >= 645.0 && lambda <= 780.0)
  {
    fRed = 1.0;
    fGreen = 0.0;
    fBlue = 0.0;
  }
  else
  {
    fRed = 0.0;
    fGreen = 0.0;
    fBlue = 0.0;
  }

  if (lambda >= 350 && lambda <= 419)
  {
    Factor = 0.3 + 0.7*(lambda - (float)350.0) / ((float)420.0 - (float)350.0);
  }
  else if (lambda >= 420 && lambda <= 700)
  {
    Factor = 1.0;
  }
  else if (lambda >= 701 && lambda <= 780)
  {
    Factor = 0.3 + 0.7*((float)780.0 - lambda) / ((float)780.0 - (float)700.0);
  }
  else
  {
    Factor = 0.0;
  }
  iR = factorAdjust(fRed, Factor, MaxIntensity, Gamma);
  iG = factorAdjust(fGreen, Factor, MaxIntensity, Gamma);
  iB = factorAdjust(fBlue, Factor, MaxIntensity, Gamma);
}

int factorAdjust(float C, float Factor, int MaxIntensity, float Gamma)
{
  if(C == 0.0)
  {
    return 0;
  }
  else
  {
    return (int) round(MaxIntensity * pow(C * Factor, Gamma));
  }
}
