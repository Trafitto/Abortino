//23/13/2017
/*
* BORCHI MARCO -ABORTino MEGA (Now with Genuino Mega)
* Avoiding obsacle robot with rotate head
* 
*/
//!!!!!! ---> TO TEST <--- !!!!!!!!!!


//Motor
#include <AFMotor.h>
#include <Servo.h>

//Display
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Display settings
#define OLED_MOSI  44 //D1 on my display
#define OLED_CLK   42 //D0
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

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
	
	display.begin(SSD1306_SWITCHCAPVCC);
	// Clear the buffer.
	display.clearDisplay();
    
    headServo.attach(10); //CONFIG HEAD	
	WATCH_FORWARD(); //SETTA AL CENTRO IL SERVO MOTORE
    motorDX.setSpeed(250) ;
    motorSX.setSpeed(250);
	
    STOP();
    Serial.println("Avvio");
}

void loop()
{
//ho rimosso il codice per i comandi wifi tramite il bit -> con arduino mega posso usare la porta Serial1

       
        scannedDistanceFW=letturaSensoreUltrasuoni();
      
        Serial.print("Distanza Rilevata: ");
         Serial.println(scannedDistanceFW);
        
        if (scannedDistanceFW>DangerDistance)  //se la distanza rilevata � maggiore della distanza "Pericolosa" va avanti
        {
          
          
          GO_FORWARD();
          
        }
        else if (scannedDistanceFW<=DangerDistance)//Atrimenti guarda a sinistra poi a destra e sceglie il percorso ottimale
        {
          
            STOP(); //si ferma 
            delay(20);
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

void WRITE_DISPLAY(String text )
{
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0,0);
	display.println(text);
  
	display.display();
}

int TURN_HEAD_SX()
{
  
  
    delay(10);
    headServo.write(180); //RUOTA LA TESTA A SX
	BLINK_LED(1,LedPin); //LAMPEGGIA UNA VOLTA QUANDO HA FINITO DI RUOTARE LA TESTA
	delay(1000); //DIAMOLI IL TEMPO DI GIRARE gira pi� a lungo perche � girato           
	distanceSX=letturaSensoreUltrasuoni(); //LEGGO E MEMORIZZO COSA TROVO A SX
        Serial.print("Distanza Rilevata SX: ");
	Serial.println(distanceSX);
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
     Serial.print("Distanza Rilevata DX: ");
	Serial.println(distanceDX);
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

void TURN_SX()
{
  
      Serial.print("Giro a SX ");
     motorSX.run(BACKWARD);
     motorDX.run(RELEASE);
     delay(100); 
      

}

void TURN_DX()
{
 
    Serial.print("Giro a DX ");
     motorSX.run(RELEASE); 
     motorDX.run(BACKWARD);
     delay(100); 

}


int  letturaSensoreUltrasuoni()
{
    digitalWrite(TrigPin, LOW);
    delayMicroseconds(2);                  // mantiene alto il trigger per almeno 2us
    digitalWrite(TrigPin, HIGH);
    delayMicroseconds(5); 
    durata = pulseIn(EchoPin, HIGH);
  return  distanza = (durata/2)/29.1;
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
		TURN_SX();
	}
	else if (differenceDX>differenceSX  && differenceDX>differenceFW)
	{
		TURN_DX(); 
	}
	else if (differenceSX==differenceDX && differenceSX>differenceFW)  
	{
		//un pizzico di casualit�
		long rand =random(99); //numero random da 0 a 99 (sono 100 numeri)
		if (rand>=50)
		{
			TURN_DX();
		}
		else
		{
			TURN_SX();
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
			TURN_DX();
		}
		else
		{
			TURN_SX();
		}
	}
	
	
     
    delay(50);
	WATCH_FORWARD(); //Per sicurezza rimposto l'head al centro
	STOP(); //Per sicurezza stoppo di nuovo i motori

}
