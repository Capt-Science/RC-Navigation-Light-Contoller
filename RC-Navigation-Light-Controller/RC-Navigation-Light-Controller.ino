// RC Plane nav lights Arduino 
// by Capt Science


// Pin definitions---------------------------solder to pins as described below
  #define LIGHTS_INPUT 2
  #define LANDING_INPUT 3
  #define NAV_A_PIN 4
  #define NAV_B_PIN 5
  #define STROBE_A_PIN 6
  #define STROBE_B_PIN 7
  #define STROBE_C_PIN 8
  #define LANDING_PIN 9


// PWM settings---------------------------- calibrate in debug mode
  #define SERVO_REVERSED false   // Whether or not the servo channel is reversed
  #define NO_LIGHTS_THRESH 1000 //threshold for no lights to turn on
  #define RUN_LIGHTS_THRESH 1300 // threshold for nav lights on
  #define ALL_LIGHTS_ON 1900 // threshold for all lights on


// Beacon Settings---------------------------fill this out
  #define FADE_MIN 050
  #define FADE_MAX 200
  #define FADE_INTERVAL 3000
  #define FADE_ON_HOLD 10
  #define FADE_OFF_HOLD 1000


// Strobe Settings ---------------------------fill this out
  #define STROBE_ON_TIME 120
  #define STROBE_GAP_TIME 90
  #define STROBEA_WAIT_TIME 180
  #define STROBEB_WAIT_TIME 180
  #define STROBEC_WAIT_TIME 180


// Variable declarations
  double channel[2]; // give accurat measure of channel inputs
  unsigned long currentTime;
  unsigned long strobeAlast;
  unsigned long strobeBlast;
  unsigned long strobeClast;
  unsigned long strobeAon;
  unsigned long strobeBon;
  unsigned long strobeCon;
  int strobeAgap = 0;
  int strobeBgap = 0;
  int strobeCgap = 0;
  unsigned long navAlast;
  unsigned long navBlast;
  int navAon = 1;
  int navBon = 0;
  int fadeDirection = 1;
  int curentFade = FADE_MIN;
  


// not sure about these-----------------------------------
  int strobeAstate = LOW;
  int strobeBstate = LOW;
  int strobeCstate = LOW;
  int navAstate = LOW;
  int navBstate = LOW;
  int landingstate = LOW;
  

// ------------------startup call------------------------------
void setup() {
  // put your setup code here, to run once:
  pinMode(LIGHTS_INPUT, INPUT);
  pinMode(LANDING_INPUT, INPUT);
  pinMode(NAV_A_PIN, OUTPUT);
  pinMode(NAV_B_PIN, OUTPUT);
  pinMode(STROBE_A_PIN, OUTPUT);
  pinMode(STROBE_B_PIN, OUTPUT);
  pinMode(STROBE_C_PIN, OUTPUT);
  pinMode(LANDING_PIN, OUTPUT);


  //serial monitor debug section
  Serial.begin(9600);

}
//------------------repeating call-------------------------------
void loop() {
  // put your main code here, to run repeatedly:

   currentTime = micros();

 
  //determine channel inputs
  channel[0] = pulseIn(2, HIGH);
  channel[1] = pulseIn(3, HIGH);
  
  // section for serial monitor
    //  Serial.println(channel[0]);
    //  Serial.println(channel[1]);
    //  Serial.println(currentTime);


     while(channel[0] <= NO_LIGHTS_THRESH) 
        {
            channel[0] = pulseIn(2, HIGH);                     //continue to check pwm input
            channel[1] = pulseIn(3, HIGH);                     //continue to check pwm input
            //      Serial.println(channel[0]);                // section for serial monitor
            //      Serial.println(channel[1]);                // section for serial monitor
          digitalWrite(13, HIGH);
          delay(90);  //time delay so it doesn't blow up, makes 13 blink to know its working
          digitalWrite(13, LOW);
          delay(60);  //time delay so it doesn't blow up, makes 13 blink to know its working
          digitalWrite(NAV_A_PIN, LOW); //turn off 
          digitalWrite(NAV_B_PIN, LOW); //turn off 
          digitalWrite(STROBE_A_PIN, LOW); //turn off 
          digitalWrite(STROBE_B_PIN, LOW); //turn off 
          digitalWrite(STROBE_C_PIN, LOW); //turn off 
          digitalWrite(LANDING_PIN, LOW); //turn off
        }




  currentTime = micros(); //do this before every loop
  doStrobe();
  doFade();
  doLanding();

// STROBE ROUTINE+++++++++++++++++++++++++++++++++++++++++++++++++++
void doStrobe(){
   // Strobe A light----------------
   switch (strobeAgap){
        case 0:
           if ((currentTime -  strobeAlast) >  STROBEA_WAIT_TIME) {
             digitalWrite(STROBE_A_PIN, HIGH);
             strobeAon = micros();
            }
            if ((currentTime - strobeAon) > STROBE_ON_TIME){
              digitalWrite(STROBE_A_PIN, LOW);
              strobeAgap = micros();
            }
            break;
        default:
            if ((currentTime - strobeAgap) > STROBE_GAP_TIME){
              digitalWrite(STROBE_A_PIN, HIGH);
              strobeAon = micros();
              if ((currentTime - strobeAon) > STROBE_ON_TIME){
                 digitalWrite(STROBE_A_PIN, LOW);
                  strobeAgap = 0;
                  strobeAlast = micros();
                  }
              }
        break;
            }

      // Strobe B light-------------------
   switch (strobeBgap){
        case 0:
           if ((currentTime -  strobeBlast) >  STROBEB_WAIT_TIME) {
             digitalWrite(STROBE_B_PIN, HIGH);
             strobeBon = micros();
            }
            if ((currentTime - strobeBon) > STROBE_ON_TIME){
              digitalWrite(STROBE_B_PIN, LOW);
              strobeBgap = micros();
            }
            break;
        default:
            if ((currentTime - strobeBgap) > STROBE_GAP_TIME){
              digitalWrite(STROBE_B_PIN, HIGH);
              strobeBon = micros();
              if ((currentTime - strobeBon) > STROBE_ON_TIME){
                 digitalWrite(STROBE_B_PIN, LOW);
                  strobeBgap = 0;
                  strobeBlast = micros();
                  }
              }
        break;
            }
   // Strobe C light --------------
   switch (strobeCgap){
        case 0:
           if ((currentTime -  strobeClast) >  STROBEC_WAIT_TIME) {
             digitalWrite(STROBE_C_PIN, HIGH);
             strobeCon = micros();
            }
            if ((currentTime - strobeCon) > STROBE_ON_TIME){
              digitalWrite(STROBE_C_PIN, LOW);
              strobeCgap = micros();
            }
            break;
        default:
            if ((currentTime - strobeCgap) > STROBE_GAP_TIME){
              digitalWrite(STROBE_A_PIN, HIGH);
              strobeAon = micros();
              if ((currentTime - strobeCon) > STROBE_ON_TIME){
                 digitalWrite(STROBE_C_PIN, LOW);
                  strobeCgap = 0;
                  strobeClast = micros();
                  }
              }
        break;
            }
}
// STROBE ROUTINE END+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//FADE ROUTINE+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void doFade(){
    


}
}
}//end
