int speakerPin = 2;
int flowmeterPin = 3;
int flowcounter = 0;
int literPerMin; // Calculated litres/hour
unsigned long currentTime;
unsigned long oldTime;
int valvePin = 4;
int ps1Pin = A1;  //pressure sensor 1
int ps2Pin = A2;  //pressure sensor 2
int fsPin = A3; //force sensor 1

void flow () // Interrupt function
{
   flowcounter++;
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(valvePin, OUTPUT);           
  pinMode(ps1Pin, INPUT);
  pinMode(ps2Pin, INPUT);
  pinMode(fsPin, INPUT);
  pinMode(flowmeterPin, INPUT);
  attachInterrupt(3,flow,RISING); 
  interrupts(); //Enable ISR
  currentTime = millis();   //get curret time
  oldTime = currentTime;
}

void loop() {
  // put your main code here, to run repeatedly:
  // PULSITILE FLOW
  digitalWrite(valvePin, HIGH);    //Switch Solenoid ON
  delay(400);                      //Wait .4 Second
  digitalWrite(valvePin, LOW);     //Switch Solenoid OFF
  delay(500);                      //Wait .5 Second

  // PRESSURE SENSORS
  float sensorVal1Avg = 0;
  for(int i = 0; i<50; i++){                //Read 50 digial values from ps1 and avg
    int ps1Pin = analogRead(A1);
    sensorVal1Avg = sensorVal1Avg + ps1Pin;
  }
  sensorVal1Avg = sensorVal1Avg/50;
  float psi = (sensorVal1Avg*0.0435)-16.675;  //convert to psi
  float mmHg = psi*51.715;    //convert to mmHg
  Serial.print(" Pressure1: ");   //print digial avg, psi, and mmHg
  Serial.print(sensorVal1Avg);
  Serial.print(" ADC ");
  Serial.print(psi);
  Serial.print("psi or ");
  Serial.print(mmHg);
  Serial.print(" mmHg");
  
  float sensorVal2Avg = 0;  //Repeat for ps2
  for(int i = 0; i<50; i++){
    int sensorVal2 = analogRead(A2);
    sensorVal2Avg = sensorVal2Avg + ps2Pin;
  }
  sensorVal2Avg = sensorVal2Avg/50;
  float psi2 = (sensorVal2Avg*0.0435)-16.675;
  float mmHg2 = psi2*51.715;
  Serial.print(" Pressure2: ");
  Serial.print(sensorVal2Avg);
  Serial.print(" ADC ");
  Serial.print(psi2);
  Serial.print("psi or ");
  Serial.print(mmHg2);
  Serial.print(" mmHg");
  
  //FORCE SENSOR
  float sensorVal3Avg = 0;    // //Read 50 digial values from fs1 and avg
  for(int i = 0; i<50; i++){
    int sensorVal3 = analogRead(A3);
    sensorVal3Avg = sensorVal3Avg + fsPin;
  }
  sensorVal3Avg = sensorVal3Avg/50;
  float grams = (0.61*sensorVal3Avg) - 500;   //convert avg value to grams
  Serial.print(" Force: ");   //print ADC and grams
  Serial.print(sensorVal3Avg);
  Serial.print(" ADC or ");
  Serial.print(grams);
  Serial.print("grams ");
   

  //FLOWMETER
   currentTime = millis();  //get current time
   // Every second, calculate and print litres/hour
   if(currentTime >= (oldTime + 1000))
   {
      oldTime = currentTime; // Updates cloopTime
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      literPerMin = (1000.0 / (millis() - oldTime)*flowcounter / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      flowcounter = 0; // Reset Counter
      Serial.print(" FlowRate: "); //Print flowrate in litres/min
      Serial.print(literPerMin, DEC);   
      Serial.println(" L/min");
   }
}
