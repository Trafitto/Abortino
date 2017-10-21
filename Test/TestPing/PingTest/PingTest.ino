const int TrigPin = 50; //14 su arduino 50 su mega
const int EchoPin = 48; //15 su arduino 48 su mega
int distanza;
int durata;
int scannedDistanceFW;
const int DangerDistance=25; //La distanza  dall'ostacolo
int scannedPing;

void setup()
{
    Serial.begin(9600);
    //SONAR SENSOR PINS
    pinMode(TrigPin, OUTPUT);
    pinMode(EchoPin, INPUT);
    }
void loop()
{
scannedDistanceFW=letturaSensoreUltrasuoni();
 Serial.print("Distanza Rilevata: ");
         Serial.println(scannedDistanceFW);
         
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
