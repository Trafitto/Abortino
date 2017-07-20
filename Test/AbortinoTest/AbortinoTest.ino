#include <AFMotor.h>
#include <Servo.h>

AF_DCMotor motorDX(2);  //Motore di destra connesso all' M2
AF_DCMotor motorSX(3); //Motore di sinistra connesso all' M3


void setup()
{
    Serial.begin(9600);
    
    motorDX.setSpeed(250) ;
    motorSX.setSpeed(250);
	
    STOP();
}

char text;
void loop()
{
   
    while(Serial.available() > 0)
   {
       text=Serial.read();
       
         if (text=='a')
   {
     TestMotori();
   }
   }
 
     
     
   
 
}



void TestMotori()
{
     print("STOP");
  STOP();
  delay(1000);
  print("Vai avanti");
  GO_FORWARD();
  delay(1000);
  print("Vai indietro");
  GO_BACKWARD(5);
  delay(1000);
  print("STOP");
  STOP();
  delay(1000);
  print("Gira a sinistra");
  TURN_SX(5);
  delay(1000);
  print("STOP");
  STOP();
  delay(1000);
  print("Gira a destra");
  TURN_DX(5);
  delay(1000);
    print("STOP");
  STOP();
  delay(1000);
  print("Fine del test motori");
}

void print(String text)
{
  Serial.println(text);  
}



void STOP()
{
	motorSX.run(RELEASE);
	motorDX.run(RELEASE);
	delay(15);
}
void GO_BACKWARD(int nBk) 
{
  for (int i=0;i<nBk;i++)
  { 
    motorSX.run(BACKWARD);
    motorDX.run(BACKWARD);
    delay(15);
  }
  //delay(25);

}
void GO_FORWARD() //questo non ha il loop perche � usato gi� in un loop
{
     motorDX.run(FORWARD); 
     motorSX.run(FORWARD); 
     
}

void TURN_SX(int nSx)
{
  for (int i=0;i<nSx;i++)
  { 
          
     motorSX.run(BACKWARD);
     motorDX.run(RELEASE);
     //delay(15); //SONO SUPERFLUI SE SI VUOLE AVERE UN MOVIMENTO UNIFORME ????
      
 }
 //delay(25);
}

void TURN_DX(int nDx)
{
   for (int i=0;i<nDx;i++)
  {  
     motorSX.run(RELEASE); 
     motorDX.run(BACKWARD);
     //delay(15); //SONO SUPERFLUI SE SI VUOLE AVERE UN MOVIMENTO UNIFORME ????
   }
   //delay(25);  //SONO SUPERFLUI SE SI VUOLE AVERE UN MOVIMENTO UNIFORME ????
}
