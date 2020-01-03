#include "SevSeg.h"
#define ECHOPIN 2
#define TRIGPIN 3
#define ECHOPIND 4
#define TRIGPIND 5
#define ECHOPINT 6
#define TRIGPINT 7

SevSeg sevseg,sevseg1,sevseg2;
byte numDigits = 2;
byte digitPins[] = {8,9};
byte digitPins1[] = {10,11};
byte digitPins2[] = {12,13};
byte segmentPins[] = {22,23,24,25,26,27,28,29};
byte segmentPins1[] = {30,31,32,33,34,35,36,37};
byte segmentPins2[] = {38,39,40,41,42,43,44,45};
bool resistorsOnSegments = false;
byte hardwareConfig = COMMON_ANODE; 
bool updateWithDelays = false;
bool leadingZeros = true;
int waktuS,waktuD,waktuT;
int senS,senD,senT;
bool Status1 = false,Status2 = false,Status3 = false;


void setup() {
  pinMode(ECHOPIN, INPUT);
  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIND, INPUT);
  pinMode(TRIGPIND, OUTPUT);
  pinMode(ECHOPINT, INPUT);
  pinMode(TRIGPINT, OUTPUT);
  for(int i = 46; i<= 53; i++){ 
    pinMode(i, OUTPUT);    
  }
  pinMode(A15, OUTPUT);
    
   sevseg.begin(hardwareConfig, numDigits, digitPins,  segmentPins,resistorsOnSegments, updateWithDelays, leadingZeros);
  sevseg1.begin(hardwareConfig, numDigits, digitPins1, segmentPins1,resistorsOnSegments, updateWithDelays, leadingZeros);
  sevseg2.begin(hardwareConfig, numDigits, digitPins2, segmentPins2,resistorsOnSegments, updateWithDelays, leadingZeros);
  Serial.begin(9600);
}
int sensor1(){
  digitalWrite(TRIGPIN, LOW); 
  digitalWrite(TRIGPIN, HIGH);
  digitalWrite(TRIGPIN, LOW); 
  int distance = pulseIn(ECHOPIN, HIGH);
  distance= distance/58;    
  if(distance <= 5){
    return true;
  }
  else{
    return false;
  }
}
int sensor2(){
  digitalWrite(TRIGPIND, LOW); 
  digitalWrite(TRIGPIND, HIGH);
  digitalWrite(TRIGPIND, LOW); 
  int distance = pulseIn(ECHOPIND, HIGH);
  distance= distance/58;    
  if(distance <= 5){
    return true;
  }
  else{
    return false;
  }
}
int sensor3(){
  digitalWrite(TRIGPINT, LOW); 
  digitalWrite(TRIGPINT, HIGH);
  digitalWrite(TRIGPINT, LOW); 
  int distance = pulseIn(ECHOPINT, HIGH);
  distance= distance/58;    
  if(distance <= 5){
    return true;
  }
  else{
    return false;
  }
}
void loop() {  
  static unsigned long timer = millis();
  if (millis() - timer >= 1000) {
    if(sensor1()){
      senS++;
      
      if(senS >= 4){ // 5 detik
        senS = 0;
      }
    }
    else{
      senS = 0;
    }
    if(sensor2()){
      senD++;      
      if(senD >= 4){ // 5 detik
        senD = 0;
      }
    }
    else{
      senD = 0;
    }    
    if(sensor3()){
      senT++;      
      if(senT >= 4){ // 5 detik
        senT = 0;
      }
    }
    else{
      senT = 0;
    }

    normal();
    
    timer += 1000;
    waktuS--;
    waktuD--;
    waktuT--;
  }
  sevseg.refreshDisplay();
  sevseg1.refreshDisplay();
  sevseg2.refreshDisplay();  
  LampNormal();
}


void LampNormal(){
    if(waktuS != 0){
      if(waktuS >= 5){
        Lampu(1,1);
      }
      else if(waktuS ==4 || waktuS ==3 || waktuS ==2 || waktuS == 1){
        Lampu(1,2);
      }
      else if(waktuD ==4 || waktuD ==3 ){
        Lampu(1,2);     
       }   
      else if(waktuD ==2 || waktuD ==1 || waktuD ==0 ){
        Lampu(1,1);     
       }        
      else{
        Lampu(1,3);
        if(!Status1){
          if(senS >=3){
            waktuD =waktuD + 5;
            waktuT =waktuT + 5;
            Status1 = true;   
            Status2 = false;
            Status3 = false;                            
          }            
        }  
      }
    }
    if(waktuD != 0){
      if(waktuD >= 5){
        Lampu(2,1);
      }
      else if(waktuD ==4 || waktuD ==3 || waktuD ==2 || waktuD == 1){
        Lampu(2,2);     
       }
      else if(waktuT ==4 || waktuT ==3 ){
        Lampu(2,2);
      }
      else if(waktuT == 2 || waktuT == 1 || waktuT == 0 ){
        Lampu(2,1);              
      }
      else{
        Lampu(2,3);         
          if(!Status2){
            if(senD >=3){
              waktuS =waktuS + 5;
              waktuT =waktuT + 5;
              Status2 = true; 
              Status1 = false;
              Status3 = false;                                             
            }            
          } 
      }
    }
    if(waktuT != 0){
      if(waktuT >= 5){
        Lampu(3,1);//
      }
      else if(waktuT ==4 || waktuT ==3 || waktuT ==2 || waktuT == 1){
        Lampu(3,2);
      } 
      else if(waktuS ==4 || waktuS ==3){
        Lampu(3,2);
      } 
      else if(waktuS == 2 ||waktuS == 1 || waktuS == 0){
        Lampu(3,1);         
      }
      else{
        Lampu(3,3);
          if(!Status3){ // kalau false
            if(senT >=3){ // ketika sensor terhalang 4 detik
              waktuD =waktuD + 5; // penambahan waktu 5 detik
              waktuS =waktuS + 5;
              Status3 = true; 
              Status2 = false;
              Status1 = false;              
            }            
          }  
      }
    }
}
void Lampu (int simpang, int warna){
  // warna 1: merah, 2:kuning, 3 : hijau
  if(simpang == 1 && warna == 1){
    digitalWrite(46,HIGH);
    digitalWrite(47,LOW);
    digitalWrite(48,LOW);    
  }
  else if(simpang == 1 && warna == 2){
    digitalWrite(46,LOW);
    digitalWrite(47,HIGH);
    digitalWrite(48,LOW);    
  }
  else if(simpang == 1 && warna == 3){
    digitalWrite(46,LOW);
    digitalWrite(47,LOW);
    digitalWrite(48,HIGH);    
  }  
  else if(simpang == 2 && warna == 1){
    digitalWrite(49,HIGH);
    digitalWrite(50,LOW);
    digitalWrite(51,LOW);    
  }
  else if(simpang == 2 && warna == 2){
    digitalWrite(49,LOW);
    digitalWrite(50,HIGH);
    digitalWrite(51,LOW);    
  }
  else if(simpang == 2 && warna == 3){
    digitalWrite(49,LOW);
    digitalWrite(50,LOW);
    digitalWrite(51,HIGH);    
  }
  else if(simpang == 3 && warna == 1){
    digitalWrite(52,HIGH);
    digitalWrite(53,LOW);
    digitalWrite(A15,LOW);    
  }
  else if(simpang == 3 && warna == 2){
    digitalWrite(52,LOW);
    digitalWrite(53,HIGH);
    digitalWrite(A15,LOW);    
  }
  else if(simpang == 3 && warna == 3){
    digitalWrite(52,LOW);
    digitalWrite(53,LOW);
    digitalWrite(A15,HIGH);       
  }        
  
}
void normal(){
    if(waktuS == 0){
      waktuD = 15;
      waktuT = 30;
    }else if(waktuD == 0){
      waktuT = 15;
      waktuS = 30; 
    }else if(waktuT == 0){
      waktuS = 15;
      waktuD = 30;
    }
    if(waktuS != 0){
      if(waktuS > 0){
        sevseg.setNumber(waktuS);
      }
      else{
        sevseg.setNumber(waktuD);
      }
    }
    if(waktuD != 0){
      if(waktuD > 0){
        sevseg1.setNumber(waktuD);        
      }
      else{
        sevseg1.setNumber(waktuT);
      }
    }
    if(waktuT != 0){
      if(waktuT > 0){
        sevseg2.setNumber(waktuT);
      }
      else{
        sevseg2.setNumber(waktuS);
      }
    }  
}

