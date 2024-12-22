//----------------------------------------------------------------
//-- Otto DIY invests time and resources providing open source code and hardware, 
//-- please support by purchasing kits from https://www.ottodiy.com/
//-- Make sure to have installed all libraries: https://github.com/OttoDIY/OttoDIYLib
//-----------------------------------------------------------------
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <Otto.h> //-- Otto Library
Otto Otto;  //This is Otto!

#define LeftLeg 23 // left leg pin, servo[0]
#define RightLeg 26 // right leg pin, servo[1]
#define LeftFoot 25 // left foot pin, servo[2]
#define RightFoot 5 // right foot pin, servo[3]
#define Buzzer 27 //buzzer pin

/*
#define LeftLeg 2 
#define RightLeg 3
#define LeftFoot 4 
#define RightFoot 5 
#define Buzzer  13 
*/
double angle_rad = PI/180.0;
double angle_deg = 180.0/PI;
int YL;
int YR;
int RL;
int RR;

void setup(){
      Otto.init(LeftLeg, RightLeg, LeftFoot, RightFoot, true, Buzzer); //Set the servo pins and Buzzer pin
    Serial.begin(9600);
    YL = EEPROM.read(0);
    if (YL > 128) YL -= 256;
    YR = EEPROM.read(1);
    if (YR > 128) YR -= 256;
    RL = EEPROM.read(2);
    if (RL > 128) RL -= 256;
    RR = EEPROM.read(3);
    if (RR > 128) RR -= 256;
    Otto.home();
    Serial.println("OTTO CALIBRATION PROGRAM");
    Serial.println("PRESS a or z for adjusting Left Leg");
   Serial.println("PRESS s or x for adjusting Left Foot");
    Serial.println("PRESS k or m for adjusting Right Leg");
    Serial.println("PRESS j or n for adjusting Right Foot");
    Serial.println();
    Serial.println("PRESS f to test Otto walking");
    Serial.println("PRESS h to restart esp"); 
   
}

void loop(){
    int charRead = 0;

    if((Serial.available()) > (0)){
        charRead = Serial.read();
    }
    if(((charRead)==('a' ))){
        YL++;
        Otto.setTrims(YL,YR,RL,RR);
        calib_homePos();
        Otto.saveTrimsOnEEPROM();
        saveServoPositionToEEPROM(); 
    }else{
        if(((charRead)==( 'z' ))){
            YL--;
            Otto.setTrims(YL,YR,RL,RR);
            calib_homePos();
            Otto.saveTrimsOnEEPROM();
            saveServoPositionToEEPROM(); 
        }else{
            if(((charRead)==( 's' ))){
                RL++;
                Otto.setTrims(YL,YR,RL,RR);
                calib_homePos();
                Otto.saveTrimsOnEEPROM();
                saveServoPositionToEEPROM(); 
            }else{
                if(((charRead)==( 'x' ))){
                    RL--;
                    Otto.setTrims(YL,YR,RL,RR);
                    calib_homePos();
                    Otto.saveTrimsOnEEPROM();
                    saveServoPositionToEEPROM(); 
                }else{
                    if(((charRead)==( 'k' ))){
                        YR++;
                        Otto.setTrims(YL,YR,RL,RR);
                        calib_homePos();
                        Otto.saveTrimsOnEEPROM();
                        saveServoPositionToEEPROM(); 
                    }else{
                        if(((charRead)==( 'm' ))){
                            YR--;
                            Otto.setTrims(YL,YR,RL,RR);
                            calib_homePos();
                            Otto.saveTrimsOnEEPROM();
                            saveServoPositionToEEPROM(); 
                        }else{
                            if(((charRead)==( 'j' ))){
                                RR++;
                                Otto.setTrims(YL,YR,RL,RR);
                                calib_homePos();
                                Otto.saveTrimsOnEEPROM();
                                saveServoPositionToEEPROM(); 
                            }else{
                                if(((charRead)==( 'n' ))){
                                    RR--;
                                    Otto.setTrims(YL,YR,RL,RR);
                                    calib_homePos();
                                    Otto.saveTrimsOnEEPROM();
                                    saveServoPositionToEEPROM(); 
                                }else{
                                    if(((charRead)==( 'f' ))){
                                        Otto.walk(1,1000,1);
                                        
                                    }else{
                                        if(((charRead)==( 'h' ))){
                                            ESP.restart();
                                        }else{
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    } 
}

void calib_homePos() {
  int servoPos[4];
  servoPos[0]=90;
  servoPos[1]=90;
  servoPos[2]=90;
  servoPos[3]=90;
  Otto._moveServos(500, servoPos);
  Otto.detachServos();
}

void saveServoPositionToEEPROM() {
    // Simpan posisi servo saat ini ke EEPROM
    EEPROM.write(0, YL);
    EEPROM.write(1, YR);
    EEPROM.write(2, RL);
    EEPROM.write(3, RR);
    EEPROM.commit(); // Pastikan data tersimpan pada beberapa perangkat yang memerlukan commit
    Serial.println("Posisi servo tersimpan ke EEPROM.");
}