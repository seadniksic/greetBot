// if (Serial.available())
  // {
  //   temp = Serial.read();
  //   switch (temp)
  //   {
  //     case 0:
  //       myCAM.OV2640_set_JPEG_size(OV2640_160x120);
  //       delay(1000);
  //       Serial.println(F("ACK CMD switch to OV2640_160x120 END"));
  //       temp = 0xff;
  //       break;
  //     case 1:
  //       myCAM.OV2640_set_JPEG_size(OV2640_176x144);
  //       delay(1000);
  //       Serial.println(F("ACK CMD switch to OV2640_176x144 END"));
  //       temp = 0xff;
  //       break;
  //     case 2: 
  //       myCAM.OV2640_set_JPEG_size(OV2640_320x240);
  //       delay(1000);
  //       Serial.println(F("ACK CMD switch to OV2640_320x240 END"));
  //       temp = 0xff;
  //       break;
  //     case 3:
  //       myCAM.OV2640_set_JPEG_size(OV2640_352x288);
  //       delay(1000);
  //       Serial.println(F("ACK CMD switch to OV2640_352x288 END"));
  //       temp = 0xff;
  //       break;
  //     case 4:
  //       myCAM.OV2640_set_JPEG_size(OV2640_640x480);
  //       delay(1000);
  //       Serial.println(F("ACK CMD switch to OV2640_640x480 END"));
  //       temp = 0xff;
  //       break;
  //     case 5:
  //       myCAM.OV2640_set_JPEG_size(OV2640_800x600);
  //       delay(1000);
  //       Serial.println(F("ACK CMD switch to OV2640_800x600 END"));
  //       temp = 0xff;
  //       break;
  //     case 6:
  //       myCAM.OV2640_set_JPEG_size(OV2640_1024x768);
  //       delay(1000);
  //       Serial.println(F("ACK CMD switch to OV2640_1024x768 END"));
  //       temp = 0xff;
  //       break;
  //     case 7:
  //       myCAM.OV2640_set_JPEG_size(OV2640_1280x1024);
  //       delay(1000);
  //       Serial.println(F("ACK CMD switch to OV2640_1280x1024 END"));
  //       temp = 0xff;
  //       break;
  //     case 8:
  //       myCAM.OV2640_set_JPEG_size(OV2640_1600x1200);
  //       delay(1000);
  //       Serial.println(F("ACK CMD switch to OV2640_1600x1200 END"));
  //       temp = 0xff;
  //       break;
  //     case 0x10:
  //       mode = 1;
  //       temp = 0xff;
  //       start_capture = 1;
  //       Serial.println(F("ACK CMD CAM start single shoot. END"));
  //       break;
  //     case 0x11: 
  //       temp = 0xff;
  //       myCAM.set_format(JPEG);
  //       myCAM.InitCAM();
  //       #if !(defined (OV2640_MINI_2MP))
  //       myCAM.set_bit(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);
  //       #endif
  //       break;
  //     case 0x20:
  //       mode = 2;
  //       temp = 0xff;
  //       start_capture = 2;
  //       Serial.println(F("ACK CMD CAM start video streaming. END"));
  //       break;
  //     case 0x30:
  //       mode = 3;
  //       temp = 0xff;
  //       start_capture = 3;
  //       Serial.println(F("ACK CMD CAM start single shoot. END"));
  //       break;
  //     case 0x31:
  //       temp = 0xff;
  //       myCAM.set_format(BMP);
  //       myCAM.InitCAM();
  //       myCAM.wrSensorReg16_8(0x3818, 0x81);
  //       myCAM.wrSensorReg16_8(0x3621, 0xA7);
  //       break;
  //     case 0x40:
  //       myCAM.OV2640_set_Light_Mode(Auto);temp = 0xff;
  //       Serial.println(F("ACK CMD Set to Auto END"));break;
  //     case 0x41:
  //       myCAM.OV2640_set_Light_Mode(Sunny);temp = 0xff;
  //       Serial.println(F("ACK CMD Set to Sunny END"));break;
  //     case 0x42:
  //       myCAM.OV2640_set_Light_Mode(Cloudy);temp = 0xff;
  //       Serial.println(F("ACK CMD Set to Cloudy END"));break;
  //     case 0x43:
  //       myCAM.OV2640_set_Light_Mode(Office);temp = 0xff;
  //       Serial.println(F("ACK CMD Set to Office END"));break;
  //     case 0x44:
  //       myCAM.OV2640_set_Light_Mode(Home);   temp = 0xff;
  //       Serial.println(F("ACK CMD Set to Home END"));break;
  //     case 0x50:
  //       myCAM.OV2640_set_Color_Saturation(Saturation2); temp = 0xff;
  //       Serial.println(F("ACK CMD Set to Saturation+2 END"));break;
  //     case 0x51:
  //       myCAM.OV2640_set_Color_Saturation(Saturation1); temp = 0xff;
  //       Serial.println(F("ACK CMD Set to Saturation+1 END"));break;
  //     case 0x52:
  //       myCAM.OV2640_set_Color_Saturation(Saturation0); temp = 0xff;
  //       Serial.println(F("ACK CMD Set to Saturation+0 END"));break;
  //     case 0x53:
  //       myCAM. OV2640_set_Color_Saturation(Saturation_1); temp = 0xff;
  //       Serial.println(F("ACK CMD Set to Saturation-1 END"));break;
  //     case 0x54:
  //       myCAM.OV2640_set_Color_Saturation(Saturation_2); temp = 0xff;
  //       Serial.println(F("ACK CMD Set to Saturation-2 END"));break; 
  //     case 0x60:
  //       myCAM.OV2640_set_Brightness(Brightness2); temp = 0xff;
  //       Serial.println(F("ACK CMD Set to Brightness+2 END"));break;
  //     case 0x61:
  //       myCAM.OV2640_set_Brightness(Brightness1); temp = 0xff;
  //       Serial.println(F("ACK CMD Set to Brightness+1 END"));break;
  //     case 0x62:
  //       myCAM.OV2640_set_Brightness(Brightness0); temp = 0xff;
  //       Serial.println(F("ACK CMD Set to Brightness+0 END"));break;
  //     case 0x63:
  //       myCAM.OV2640_set_Brightness(Brightness_1); temp = 0xff;
  //       Serial.println(F("ACK CMD Set to Brightness-1 END"));break;
  //     case 0x64:
  //       myCAM.OV2640_set_Brightness(Brightness_2); temp = 0xff;
  //       Serial.println(F("ACK CMD Set to Brightness-2 END"));break; 
  //     case 0x70:
  //       myCAM.OV2640_set_Contrast(Contrast2);temp = 0xff;
  //       Serial.println(F("ACK CMD Set to Contrast+2 END"));break; 
  //     case 0x71:
  //       myCAM.OV2640_set_Contrast(Contrast1);temp = 0xff;
  //       Serial.println(F("ACK CMD Set to Contrast+1 END"));break;
  //     case 0x72:
  //       myCAM.OV2640_set_Contrast(Contrast0);temp = 0xff;
  //       Serial.println(F("ACK CMD Set to Contrast+0 END"));break;
  //     case 0x73:
  //       myCAM.OV2640_set_Contrast(Contrast_1);temp = 0xff;
  //       Serial.println(F("ACK CMD Set to Contrast-1 END"));break;
  //     case 0x74:
  //       myCAM.OV2640_set_Contrast(Contrast_2);temp = 0xff;
  //       Serial.println(F("ACK CMD Set to Contrast-2 END"));break;
  //     case 0x80:
  //       myCAM.OV2640_set_Special_effects(Antique);temp = 0xff;
  //       Serial.println(F("ACK CMD Set to Antique END"));break;
  //     case 0x81:
  //       myCAM.OV2640_set_Special_effects(Bluish);temp = 0xff;
  //       Serial.println(F("ACK CMD Set to Bluish END"));break;
  //     case 0x82:
  //       myCAM.OV2640_set_Special_effects(Greenish);temp = 0xff;
  //       Serial.println(F("ACK CMD Set to Greenish END"));break;  
  //     case 0x83:
  //       myCAM.OV2640_set_Special_effects(Reddish);temp = 0xff;
  //       Serial.println(F("ACK CMD Set to Reddish END"));break;  
  //     case 0x84:
  //       myCAM.OV2640_set_Special_effects(BW);temp = 0xff;
  //       Serial.println(F("ACK CMD Set to BW END"));break; 
  //     case 0x85:
  //       myCAM.OV2640_set_Special_effects(Negative);temp = 0xff;
  //       Serial.println(F("ACK CMD Set to Negative END"));break; 
  //     case 0x86:
  //       myCAM.OV2640_set_Special_effects(BWnegative);temp = 0xff;
  //       Serial.println(F("ACK CMD Set to BWnegative END"));break;   
  //     case 0x87:
  //       myCAM.OV2640_set_Special_effects(Normal);temp = 0xff;
  //       Serial.println(F("ACK CMD Set to Normal END"));break;     
  //   }
  // }
  // if (mode == 1)
  // {
 
  // }
//   else if (mode == 2)
//   {
//     while (1)
//     {
//       temp = Serial.read();
//       Serial.println("This is the cam");
//       Serial.println(temp);
//       if (temp == 0x21)
//       {
//         start_capture = 0;
//         mode = 0; 
//         Serial.println(F("ACK CMD CAM stop video streaming. END"));
//         break;
//       }
//       if (start_capture == 2)
//       {
//         myCAM.flush_fifo();
//         myCAM.clear_fifo_flag();
//         //Start capture
//         myCAM.start_capture();
//         start_capture = 0;
//       }
//       if (myCAM.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK))
//       {
//         uint32_t length = 0;
//         length = myCAM.read_fifo_length();
//         if ((length >= MAX_FIFO_SIZE) | (length == 0))
//         {
//           myCAM.clear_fifo_flag();
//           start_capture = 2;
//           continue;
//         }
//         myCAM.CS_LOW();
//         myCAM.set_fifo_burst();//Set fifo burst mode
//         temp =  SPI.transfer(0x00);
//         length --;
//         while ( length-- )
//         {
//           temp_last = temp;
//           temp =  SPI.transfer(0x00);
//           if (is_header == true)  // main condition for image transfer
//           {
//             Serial.write(temp);  
//           }
//           else if ((temp == 0xD8) & (temp_last == 0xFF)) // first condition to activate on image transfer
//           {
//             is_header = true;
//             Serial.println(F("ACK IMG END"));
//             Serial.write(temp_last);
//             Serial.write(temp);
//           }
//           if ( (temp == 0xD9) && (temp_last == 0xFF) ) //If find the end ,break while,
//           break;
//           delayMicroseconds(15);
//         }
//         Serial.println(F("ACK CMD Read Image END "));
//         myCAM.CS_HIGH();
//         myCAM.clear_fifo_flag();
//         start_capture = 2;
//         is_header = false;
//       }
//     }
//   }
//   else if (mode == 3)
//   {
//     if (start_capture == 3)
//     {
//       //Flush the FIFO
//       myCAM.flush_fifo();
//       myCAM.clear_fifo_flag();
//       //Start capture
//       myCAM.start_capture();
//       start_capture = 0;
//     }
//     if (myCAM.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK))
//     {
//       Serial.println(F("ACK CMD CAM Capture Done. END"));delay(50);
//       uint8_t temp, temp_last;
//       uint32_t length = 0;
//       length = myCAM.read_fifo_length();
//       if (length >= MAX_FIFO_SIZE ) 
//       {
//         Serial.println(F("ACK CMD Over size. END"));
//         myCAM.clear_fifo_flag();
//         return;
//       }
//       if (length == 0 ) //0 kb
//       {
//         Serial.println(F("ACK CMD Size is 0. END"));
//         myCAM.clear_fifo_flag();
//         return;
//       }
//       myCAM.CS_LOW();
//       myCAM.set_fifo_burst();//Set fifo burst mode
      
//       Serial.write(0xFF);
//       Serial.write(0xAA);
//       for (temp = 0; temp < BMPIMAGEOFFSET; temp++)
//       {
//         Serial.write(pgm_read_byte(&bmp_header[temp]));
//       }
//       //for old version, enable it else disable
//     // SPI.transfer(0x00);
//       char VH, VL;
//       int i = 0, j = 0;
//       for (i = 0; i < 240; i++)
//       {
//         for (j = 0; j < 320; j++)
//         {
//           VH = SPI.transfer(0x00);;
//           VL = SPI.transfer(0x00);;
//           Serial.write(VL);
//           delayMicroseconds(12);
//           Serial.write(VH);
//           delayMicroseconds(12);
//         }
//       }
//       Serial.write(0xBB);
//       Serial.write(0xCC);
//       myCAM.CS_HIGH();
//       //Clear the capture done flag
//       myCAM.clear_fifo_flag();
//     }
//   }