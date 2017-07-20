//12/05/2017
/*
* BORCHI MARCO -ABORTino MEGA (Now with Genuino Mega)
* Avoiding obsacle robot with rotate head
* 
*/
//!!!!!! ---> TO TEST <--- !!!!!!!!!!


#include <AFMotor.h>
#include <Servo.h>


// specifica i pin di trig e echo usati per il sensore ad ultrasuoni
const int TrigPin = 50; //14 su arduino 50 su mega
const int EchoPin = 48; //15 su arduino 48 su mega

int distanza;
int durata;

const int LedPin = 51; 
const int LedForPing = 52; //DA VERIFICARE IL NUMERO DI PING
Servo headServo;

int scannedDistanceFW;
const int DangerDistance=25; //La distanza  dall'ostacolo
int scannedPing;


AF_DCMotor motorDX(2);  //Motore di destra connesso all' M2
AF_DCMotor motorSX(3); //Motore di sinistra connesso all' M3

int distanceSX,distanceDX, distanceFOR;

void setup()
{
    Serial.begin(9600);
    //SONAR SENSOR PINS
    pinMode(TrigPin, OUTPUT);
    pinMode(EchoPin, INPUT);
    pinMode(LedPin, OUTPUT);
	pinMode(LedForPing, OUTPUT);
    
    headServo.attach(10); //CONFIG HEAD	
	WATCH_FORWARD(); //SETTA AL CENTRO IL SERVO MOTORE
    motorDX.setSpeed(250) ;
    motorSX.setSpeed(250);
	
    STOP();
}

void loop()
{
//ho rimosso il codice per i comandi wifi tramite il bit -> con arduino mega posso usare la porta Serial1

        delay(10);
        scannedDistanceFW=letturaSensoreUltrasuoni();
        //Serial.println(scannedPing);
        if (scannedDistanceFW>DangerDistance)  //se la distanza rilevata � maggiore della distanza "Pericolosa" va avanti
        {
          
          delay(10); 
          GO_FORWARD();

        }
        else if (scannedDistanceFW<=DangerDistance)//Atrimenti guarda a sinistra poi a destra e sceglie il percorso ottimale
        {
          
            STOP(); //si ferma 
            
            distanceSX=TURN_HEAD_SX(); //Gira la testa a SX e leggo la distanza
            
            distanceDX=TURN_HEAD_DX(); //Gira la testa a DX e leggo la distanza
           
            choseDirection(scannedDistanceFW,distanceSX,distanceDX); 
			
            delay(100);
          
        }
      
}
void BLINK_LED(int blinky, int ledPin)
{
  for(int i=0;i<=blinky;i++)
  {
      digitalWrite(LedPin, HIGH); //accendo il led
      delay(100);
      digitalWrite(LedPin, LOW);
      delay(100);
  }
}

int TURN_HEAD_SX()
{
  
  
    delay(10);
    headServo.write(180); //RUOTA LA TESTA A SX
	BLINK_LED(1,LedPin); //LAMPEGGIA UNA VOLTA QUANDO HA FINITO DI RUOTARE LA TESTA
	delay(1000); //DIAMOLI IL TEMPO DI GIRARE gira pi� a lungo perche � girato           
	distanceSX=letturaSensoreUltrasuoni(); //LEGGO E MEMORIZZO COSA TROVO A SX
	
	BLINK_LED(3,LedPin); //Blinka per indicare se ha finito
    return distanceSX;
}
int  TURN_HEAD_DX()
{
    delay(10);
    headServo.write(0); //RUOTA LA TESTA A DX
	BLINK_LED(1,LedPin); //LAMPEGGIA UNA VOLTA QUANDO HA FINITO DI RUOTARE LA TESTA
	delay(1000);  //DIAMOLI IL TEMPO DI GIRARE     
	distanceDX=letturaSensoreUltrasuoni(); //LEGGO E MEMORIZZO COSA TROVO A DESTRA  
	
	BLINK_LED(3,LedPin); //Blinka per indicare se ha finito
    return distanceDX;
}

void WATCH_FORWARD()
{
    delay(15);
    headServo.write(90); //POSIZIONE INIZIALE
	delay(500);  
  
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


int  letturaSensoreUltrasuoni()
{
    digitalWrite(TrigPin, HIGH);
    delayMicroseconds(2);                  // mantiene alto il trigger per almeno 2us
    digitalWrite(TrigPin, LOW);
    delayMicroseconds(5); 
    durata = pulseIn(EchoPin, HIGH);
	BLINK_LED(2,LedForPing); //Lampeggia due volte per indicare che ha rilevato la distanza
  return  distanza = (durata/2)/29;
}
//Potrebbe essere superfluo passare la distanza rilevata a dritto
void choseDirection(int distFW,int distSX, int distDX)
{
	int differenceFW=0,differenceSX=0,differenceDX=0;
	
	differenceFW=distFW-DangerDistance; //distFW � sempre minore rispento a DangerDistance
	differenceSX=distSX-DangerDistance;
	differenceDX=distDX-DangerDistance;
	
	GO_BACKWARD(5); //Indietreggia per dargli spazio di girare
	STOP();
	delay(50);
	
	
	if (differenceSX>differenceDX && differenceSX>differenceFW)
	{
		TURN_SX(5);
	}
	else if (differenceDX>differenceSX  && differenceDX>differenceFW)
	{
		TURN_DX(5); 
	}
	else if (differenceSX==differenceDX && differenceSX>differenceFW)  
	{
		//un pizzico di casualit�
		long rand =random(99); //numero random da 0 a 99 (sono 100 numeri)
		if (rand>=50)
		{
			TURN_DX(5);
		}
		else
		{
			TURN_SX(5);
		}
	}
	else if (differenceFW>=differenceSX && differenceFW>=differenceDX)
	{
		GO_BACKWARD(5); //Indietreggia e riesegue la scansione	
	}
	else //tutti per tutti gli altri casi si va a caso :D
	{
		long rando =random(99);
		if (rando>=50)
		{
			TURN_DX(5);
		}
		else
		{
			TURN_SX(5);
		}
	}
	
	
     
    delay(50);
	WATCH_FORWARD(); //Per sicurezza rimposto l'head al centro
	STOP(); //Per sicurezza stoppo di nuovo i motori
}
