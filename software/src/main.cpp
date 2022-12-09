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
#include <Vector.h>
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
void setup() {

  uint8_t vid, pid;
  uint8_t temp;

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

  //send 0x20 to start video streaming 0x21
  // put your main code here, to run repeatedly:
  uint8_t temp = 0xff, temp_last = 0;
  bool is_header = false;

    temp = Serial.read();
    if (temp == 0x2) {
      start_capture = 0;
    }
    if (temp == 0x1|| start_capture == 2) {
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
        else if ((temp == 0xD8) & (temp_last == 0xFF))
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
      start_capture = 2;
      is_header = false;
      }
    }
    switch (temp)
    {
       case 0x40:
    myCAM.OV2640_set_Light_Mode(Auto);temp = 0xff;
    Serial.println(F("ACK CMD Set to Auto END"));break;
     case 0x41:
    myCAM.OV2640_set_Light_Mode(Sunny);temp = 0xff;
    Serial.println(F("ACK CMD Set to Sunny END"));break;
     case 0x42:
    myCAM.OV2640_set_Light_Mode(Cloudy);temp = 0xff;
    Serial.println(F("ACK CMD Set to Cloudy END"));break;
     case 0x43:
    myCAM.OV2640_set_Light_Mode(Office);temp = 0xff;
    Serial.println(F("ACK CMD Set to Office END"));break;
   case 0x44:
    myCAM.OV2640_set_Light_Mode(Home);   temp = 0xff;
   Serial.println(F("ACK CMD Set to Home END"));break;
   case 0x50:
    myCAM.OV2640_set_Color_Saturation(Saturation2); temp = 0xff;
     Serial.println(F("ACK CMD Set to Saturation+2 END"));break;
   case 0x51:
     myCAM.OV2640_set_Color_Saturation(Saturation1); temp = 0xff;
     Serial.println(F("ACK CMD Set to Saturation+1 END"));break;
   case 0x52:
    myCAM.OV2640_set_Color_Saturation(Saturation0); temp = 0xff;
     Serial.println(F("ACK CMD Set to Saturation+0 END"));break;
    case 0x53:
    myCAM. OV2640_set_Color_Saturation(Saturation_1); temp = 0xff;
     Serial.println(F("ACK CMD Set to Saturation-1 END"));break;
    case 0x54:
     myCAM.OV2640_set_Color_Saturation(Saturation_2); temp = 0xff;
     Serial.println(F("ACK CMD Set to Saturation-2 END"));break; 
   case 0x60:
    myCAM.OV2640_set_Brightness(Brightness2); temp = 0xff;
     Serial.println(F("ACK CMD Set to Brightness+2 END"));break;
   case 0x61:
     myCAM.OV2640_set_Brightness(Brightness1); temp = 0xff;
     Serial.println(F("ACK CMD Set to Brightness+1 END"));break;
   case 0x62:
    myCAM.OV2640_set_Brightness(Brightness0); temp = 0xff;
     Serial.println(F("ACK CMD Set to Brightness+0 END"));break;
    case 0x63:
    myCAM. OV2640_set_Brightness(Brightness_1); temp = 0xff;
     Serial.println(F("ACK CMD Set to Brightness-1 END"));break;
    case 0x64:
     myCAM.OV2640_set_Brightness(Brightness_2); temp = 0xff;
     Serial.println(F("ACK CMD Set to Brightness-2 END"));break; 
    case 0x70:
      myCAM.OV2640_set_Contrast(Contrast2);temp = 0xff;
     Serial.println(F("ACK CMD Set to Contrast+2 END"));break; 
    case 0x71:
      myCAM.OV2640_set_Contrast(Contrast1);temp = 0xff;
     Serial.println(F("ACK CMD Set to Contrast+1 END"));break;
     case 0x72:
      myCAM.OV2640_set_Contrast(Contrast0);temp = 0xff;
     Serial.println(F("ACK CMD Set to Contrast+0 END"));break;
    case 0x73:
      myCAM.OV2640_set_Contrast(Contrast_1);temp = 0xff;
     Serial.println(F("ACK CMD Set to Contrast-1 END"));break;
   case 0x74:
      myCAM.OV2640_set_Contrast(Contrast_2);temp = 0xff;
     Serial.println(F("ACK CMD Set to Contrast-2 END"));break;
   case 0x80:
    myCAM.OV2640_set_Special_effects(Antique);temp = 0xff;
    Serial.println(F("ACK CMD Set to Antique END"));break;
   case 0x81:
    myCAM.OV2640_set_Special_effects(Bluish);temp = 0xff;
    Serial.println(F("ACK CMD Set to Bluish END"));break;
   case 0x82:
    myCAM.OV2640_set_Special_effects(Greenish);temp = 0xff;
    Serial.println(F("ACK CMD Set to Greenish END"));break;  
   case 0x83:
    myCAM.OV2640_set_Special_effects(Reddish);temp = 0xff;
    Serial.println(F("ACK CMD Set to Reddish END"));break;  
   case 0x84:
    myCAM.OV2640_set_Special_effects(BW);temp = 0xff;
    Serial.println(F("ACK CMD Set to BW END"));break; 
  case 0x85:
    myCAM.OV2640_set_Special_effects(Negative);temp = 0xff;
    Serial.println(F("ACK CMD Set to Negative END"));break; 
  case 0x86:
    myCAM.OV2640_set_Special_effects(BWnegative);temp = 0xff;
    Serial.println(F("ACK CMD Set to BWnegative END"));break;   
   case 0x87:
    myCAM.OV2640_set_Special_effects(Normal);temp = 0xff;
    Serial.println(F("ACK CMD Set to Normal END"));break;     
  }
    
  }

//   if (temp == 1) {
//     start_capture = 1;
//   } else if (temp == 2) {
//     start_capture = 0;
//   }

//   if (start_capture == 1) {

//     myCAM.flush_fifo();
//     myCAM.clear_fifo_flag();
//     //Start capture
//     myCAM.start_capture();
//     start_capture = 0;

//   } 
//   if (myCAM.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK))
//     {
//       uint32_t length = 0;
//       length = myCAM.read_fifo_length();
//       if ((length >= MAX_FIFO_SIZE) | (length == 0))
//       {
//         myCAM.clear_fifo_flag();
//         start_capture = 2;
//         continue;
//       }
//       myCAM.CS_LOW();
//       myCAM.set_fifo_burst();//Set fifo burst mode
//       temp =  SPI.transfer(0x00);
//       length --;
//       while ( length-- )
//       {
//         temp_last = temp;
//         temp =  SPI.transfer(0x00);
//         if (is_header == true)
//         {
//           Serial.write(temp);
//         }
//         else if ((temp == 0xD8) & (temp_last == 0xFF))
//         {
//           is_header = true;
//           Serial.println(F("ACK IMG END"));
//           Serial.write(temp_last);
//           Serial.write(temp);
//         }
//         if ( (temp == 0xD9) && (temp_last == 0xFF) ) //If find the end ,break while,
//         break;
//         delayMicroseconds(15);
//       }
//       myCAM.CS_HIGH();
//       myCAM.clear_fifo_flag();
//       start_capture = 2;
//       is_header = false;
//     }
  
// }
// uint8_t read_fifo_burst(ArduCAM myCAM)
// {
//   uint8_t temp = 0, temp_last = 0;
//   uint32_t length = 0;
//   length = myCAM.read_fifo_length();
//   // Serial.println("Reading Length");
//   // Serial.println(length, DEC);
//   if (length >= MAX_FIFO_SIZE) //512 kb
//   {
//     Serial.println(F("ACK CMD Over size. END"));
//     return 0;
//   }
//   if (length == 0 ) //0 kb
//   {
//     Serial.println(F("ACK CMD Size is 0. END"));
//     return 0;
//   }
//   myCAM.CS_LOW();
//   myCAM.set_fifo_burst();//Set fifo burst mode
//   temp =  SPI.transfer(0x00);
//   Serial.write(0xFF);
//   length --;
//   while ( length-- )
//   {
//     temp_last = temp;
//     temp =  SPI.transfer(0x00); //Send 0x00 to camera, capture return value in temp (one byte)
//     if (is_header == true)
//     {
//       Serial.write(temp);
//     }
//     else if ((temp == 0xD8) & (temp_last == 0xFF))
//     {
//       is_header = true;
//       // Serial.println(F("ACK IMG END"));
//       Serial.write(temp_last);
//       Serial.write(temp);
//     }
//     if ( (temp == 0xD9) && (temp_last == 0xFF) ) //If find the end ,break while,
//     break;
//     delayMicroseconds(15);
//   }
//   myCAM.CS_HIGH();
//   is_header = false;
//   return 1;
// }
