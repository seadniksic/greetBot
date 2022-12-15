// ArduCAM Mini demo (C)2017 Lee
// Web: http://www.ArduCAM.com
// This program is a demo of how to use most of the functions
// of the library with ArduCAM Mini camera, and can run on any Arduino platform.
// This demo was made for ArduCAM_Mini_5MP_Plus.
// It needs to be used in combination with PC software.
// It can test OV2640 functions
// This program requires the ArduCAM V4.0.0 (or later) library and ArduCAM_Mini_5MP_Plus
// and use Arduino IDE 1.6.8 compiler or above
#include <Wire.h>
#include <ArduCAM.h>
#include <SPI.h>
#include "memorysaver.h"
//#include <Vector.h>
#include <Servo.h>
//This demo can only work on OV2640_MINI_2MP platform.

#define BMPIMAGEOFFSET 66
const char bmp_header[BMPIMAGEOFFSET] PROGMEM =
{
  0x42, 0x4D, 0x36, 0x58, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 0x00, 0x28, 0x00,
  0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x00, 0x03, 0x00,
  0x00, 0x00, 0x00, 0x58, 0x02, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0xE0, 0x07, 0x00, 0x00, 0x1F, 0x00,
  0x00, 0x00
};
// set pin 7 as the slave select for the digital pot:
const int CS = 10;
bool is_header = false;
int mode = 0;
uint8_t start_capture = 0;
ArduCAM myCAM( OV2640, CS );
uint8_t read_fifo_burst(ArduCAM myCAM);
int32_t centerX = 160;
int32_t centerY = 120;
int pos = 0;
float Kpx, Kpy;


union ArrayToInteger {
  byte array[2];
  uint32_t integer;
};


Servo cam;
Servo base;
Servo test;
uint8_t cam_angle;
int8_t base_angle;

void setup() {

  uint8_t vid, pid;
  uint8_t temp;
  

  // cam.write(90);
  // base.write(90);

  

  cam.attach(5);
  base.attach(2);

  cam.write(90);
  base.write(90);
  // test.attach(5);

  Wire.begin();
  Serial.begin(3000000);

  Serial.println(F("ACK CMD ArduCAM Start! END"));
  // set the CS as an output:
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);
  // initialize SPI:
  SPI.begin();
  //Reset the CPLD
  myCAM.write_reg(0x07, 0x80);
  delay(100);
  myCAM.write_reg(0x07, 0x00);
  delay(100);

  while(1){
    //Check if the ArduCAM SPI bus is OK
    myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
    temp = myCAM.read_reg(ARDUCHIP_TEST1);
    if (temp != 0x55){
      Serial.println(F("ACK CMD SPI interface Error! END"));
      delay(1000);
      continue;
    }else{
      Serial.println(F("ACK CMD SPI interface OK. END"));
      break;
    }
  }

  while(1){
    //Check if the camera module type is OV2640
    myCAM.wrSensorReg8_8(0xff, 0x01);
    myCAM.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
    myCAM.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
    if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 ))){
      Serial.println(F("ACK CMD Can't find OV2640 module! END"));
      delay(1000);
      continue;
    }
    else{
      Serial.println(F("ACK CMD OV2640 detected. END"));
      break;
    } 
  }

  //Change to JPEG capture mode and initialize the OV2640 module
  myCAM.set_format(JPEG);
  myCAM.InitCAM();
  myCAM.OV2640_set_JPEG_size(OV2640_320x240);
  delay(1000);
  myCAM.clear_fifo_flag();
  
}

void loop() {

  // put your main code here, to run repeatedly:
  uint8_t temp = 0xff, temp_last = 0;
  bool is_header = false;

    temp = Serial.read();
    // if (temp == 0x2) {
    //   start_capture = 0;
    // }
    if (temp == 0x1) {
      myCAM.flush_fifo();
      myCAM.clear_fifo_flag();
      //Start capture
      myCAM.start_capture();
      start_capture = 0;
    }
    if (myCAM.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK)) {
      uint32_t length = 0;
      length = myCAM.read_fifo_length();
      if ((length >= MAX_FIFO_SIZE) | (length == 0))
      {
        myCAM.clear_fifo_flag();
        start_capture = 2;
      } else {
      myCAM.CS_LOW();
      myCAM.set_fifo_burst();//Set fifo burst mode
      temp =  SPI.transfer(0x00);
      Serial.write(0xFF);
      // uint8_t storage[7000];
      // Vector<uint8_t> img(storage);
      length --;
      while ( length-- )
      {
        temp_last = temp;
        temp =  SPI.transfer(0x00);
        if (is_header == true)
        {
          Serial.write(temp);
          // img.push_back(temp);
        }
        else if ((temp == 0xD8) & (temp_last == 0xFF)) //first two bytes of jpeg standard
        {
          is_header = true;
          // Serial.println(F("ACK IMG END"));
          // img.push_back(temp_last);
          // img.push_back(temp);
          Serial.write(temp_last);
          Serial.write(temp);
        }
        if ( (temp == 0xD9) && (temp_last == 0xFF) ) { //If find the end ,break while,
          Serial.write("\n\n\n");
          // Serial.write(img.size());
          break; 
        }
        delayMicroseconds(15);
      }
      myCAM.CS_HIGH();
      myCAM.clear_fifo_flag();
      // start_capture = 0;
      is_header = false;
      }
    }
    if (temp == 0x3) {

      // uint16_t centerFaceX = Serial.parseInt();
      // uint16_t centerFaceY = Serial.parseInt();
      char faceCoords[4];
      int32_t xCoord, yCoord;
      uint16_t coordsBuffer[1] = {0};
      int32_t diffBuffer[1] = {0};
      char *bytes = (char *) diffBuffer; //allows us to store each coordinate as a uint16, but transmit each byte over serial (2 bytes per index)
      int32_t diffX, diffY;

      while (Serial.read() != 0xFF) continue;

      // Serial.readBytes(faceCoords, 4);

      faceCoords[0] = Serial.read();
      faceCoords[1] = Serial.read();
      faceCoords[2] = Serial.read();
      faceCoords[3] = Serial.read();

      xCoord = (faceCoords[1] << 8) + faceCoords[0];
      yCoord = (faceCoords[3] << 8) + faceCoords[2];

      diffX = centerX - xCoord;
      diffY = centerY - yCoord;

      if (xCoord != 0 || yCoord != 0) {
        // face found 

        if (abs(diffX) > 10) {
          base_angle = base.read();
          Kpx = diffX / 17;
          if(diffX < 0) {
            if (base_angle > 10) {
              base.write(base_angle - abs((int) Kpx));
            }
          } else if (diffX >= 0) {

            if (base_angle < 160) {
              //CANNNOT WRITE 1 degree > base angle in this specific direction for some reason
              base.write(base_angle + abs((int) (Kpx + 1)));
            }
          }
        }

        if (abs(diffY) > 1) {
          cam_angle = cam.read();
          Kpy = diffY / 17;
          if(diffY >= 0) {
            if (cam_angle > 10) {
              cam.write(cam_angle - abs((int) Kpy));
            }
          } else if (diffY < 0) {

            if (cam_angle < 160) {
              //CANNNOT WRITE 1 degree > base angle in this specific direction for some reason
              cam.write(cam_angle + abs((int) (Kpy + 1)));
            }
          }
        }

      }

    }
}
  
