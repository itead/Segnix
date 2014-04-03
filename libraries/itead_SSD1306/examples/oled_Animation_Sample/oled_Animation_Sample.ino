// # 
// # Editor     : Lauren from DFRobot
// # Date       : 14.11.2012
// # E-Mail		: Lauren.pan@dfrobot.com

// # Product name: OLED 2864 Display module
// # Product SKU : TOY0007
// # Version     : 1.0

// # Update the Adafruit SSD1306 library to make it work
// # Description:
// # 		show a simple animation

// # Connection:
// #        SCL  -> A5(Uno)/D3(Leonardo)
// #        SDA  -> A4(Uno)/D2(Leonardo)
// #        RST  -> D4
// #        DC  -> GND
// #        3.3  -> 3.3v
// #        g  -> GND
// #
// # Product page: 
// # 		http://www.dfrobot.com/index.php?route=product/product&product_id=802#.UKyOjE09ha0

#include <itead_GFX.h>
#include <itead_SSD1306.h>

static unsigned char  Image[] =
{
  0x00, 0x78, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x03, 0x06, 0x01, 0xFB, 0xFC, 0x07, 0xFC, 0x00,
  0x07, 0xFD, 0x0F, 0xF9, 0xFF, 0xCF, 0xFE, 0x00, 0x07, 0xFE, 0x7F, 0xFB, 0xFF, 0xF3, 0xFE, 0x00,
  0x0F, 0xFC, 0xFF, 0xFF, 0xFF, 0xF9, 0xFC, 0x00, 0x0F, 0xF3, 0xFF, 0x57, 0xC7, 0xFE, 0xFA, 0x00,
  0x0E, 0x67, 0xFE, 0x94, 0xA1, 0xFF, 0x72, 0x00, 0x07, 0xEF, 0xF8, 0xA4, 0x90, 0x7F, 0x3C, 0x00,
  0x01, 0xDF, 0xF1, 0x24, 0x48, 0x37, 0x80, 0x00, 0x00, 0x1F, 0xC0, 0x24, 0x40, 0x1B, 0x80, 0x00,
  0x00, 0x1F, 0x80, 0x00, 0x00, 0x0B, 0xC0, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x05, 0xC0, 0x00,
  0x00, 0x37, 0x12, 0x00, 0x04, 0x81, 0xC0, 0x00, 0x00, 0x27, 0x00, 0x00, 0x00, 0x02, 0xC0, 0x00,
  0x00, 0x36, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x01, 0xC0, 0x00,
  0x00, 0x16, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00,
  0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x6C, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00,
  0x00, 0x84, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x92, 0x27, 0x00, 0x07, 0xE0, 0xC8, 0x00,
  0x00, 0x8E, 0x10, 0x40, 0x08, 0x00, 0x88, 0x00, 0x00, 0x42, 0x40, 0x00, 0x00, 0x04, 0x90, 0x00,
  0x00, 0x33, 0x40, 0x03, 0x80, 0x07, 0xC0, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x80, 0x08,
  0x78, 0x02, 0x00, 0x00, 0x08, 0x01, 0x00, 0x06, 0x68, 0x01, 0x00, 0x10, 0x10, 0x01, 0x00, 0x11,
  0x68, 0x00, 0x80, 0x0C, 0xC0, 0x04, 0x00, 0x11, 0xA8, 0x00, 0x60, 0x00, 0x00, 0x08, 0x01, 0xD3,
  0x8F, 0xFF, 0xFF, 0xF0, 0x1F, 0xFF, 0xFF, 0xF5, 0x9F, 0xFF, 0xFF, 0xFC, 0xFF, 0xFF, 0xFF, 0xF1,
  0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF1, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE2,
  0x5F, 0xB0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF4, 0x3F, 0x80, 0x13, 0x60, 0x00, 0x00, 0xFF, 0xF8,
  0x3F, 0x80, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0xF8, 0x3F, 0x80, 0xBF, 0x00, 0x00, 0x00, 0x8F, 0xF8,
  0x3F, 0x00, 0x9F, 0x80, 0x00, 0x00, 0x87, 0xF8, 0x3F, 0x00, 0xFF, 0xF0, 0x00, 0x00, 0x83, 0xF8,
  0x3F, 0x01, 0xFF, 0xFE, 0x00, 0x00, 0x83, 0xF8, 0x3F, 0x03, 0xFF, 0xFF, 0x00, 0x00, 0x83, 0xF0,
  0x1F, 0x05, 0xE0, 0x3C, 0xC0, 0x00, 0xFB, 0xF0, 0x1F, 0x0F, 0x00, 0x07, 0xE0, 0x00, 0x80, 0x00,
  0x1F, 0xF8, 0x00, 0x1F, 0xF0, 0x7C, 0x40, 0x00, 0x00, 0x08, 0x00, 0x7F, 0xFB, 0xC0, 0x60, 0x00,
  0x00, 0x08, 0x01, 0xFF, 0xF8, 0x00, 0x20, 0x00, 0x00, 0x08, 0x03, 0xFF, 0xE0, 0x00, 0x30, 0x00,
  0x00, 0x04, 0x07, 0xFF, 0x40, 0x01, 0xF0, 0x00, 0x00, 0x06, 0x0F, 0xF8, 0x5F, 0xFF, 0xC0, 0x00,
  0x00, 0x02, 0x1F, 0xF1, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x01, 0x3F, 0xE2, 0x00, 0x3E, 0x00, 0x00,
  0x00, 0x00, 0xFF, 0xCC, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xF0, 0x00, 0x1C, 0x00, 0x00,
  0x00, 0x00, 0x1F, 0x80, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x04, 0x00, 0x00,
  0x00, 0x00, 0x0F, 0x80, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xF8, 0x00, 0x00,
  0x00, 0x00, 0x1F, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0xF8, 0x00, 0x00,
  0x00, 0x00, 0x01, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x7D, 0xF7, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0x80, 0x00, 0x00
};

#define OLED_RESET 9 
#define DELAY 200
SSD1306 display(OLED_RESET);
int i=0;

void setup()   {                

  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);     // initialize with the I2C addr 0x3C (for the 128x64)
  display.clearDisplay();   // clears the screen and buffer

  /*****Display Part1*****/
  display.drawCircle(15,10, 10, WHITE);
  display.drawLine(15,20,12,40, WHITE); 
  display.drawLine(14,27,6,22,WHITE); 
  display.drawLine(6,22,2,16,WHITE);
  display.drawLine(12,40,29,30, WHITE);
  display.drawLine(29,30,34,40, WHITE);
  display.drawLine(12,40,17,55, WHITE);
  display.drawLine(17,55,9,65, WHITE);
  display.drawLine(12,40,29,30, WHITE);
  display.drawLine(29,30,34,40, WHITE);
  display.display();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(40,15);
  display.println("Chinese "); 
  display.setCursor(52,30); 
  display.println("Kongfu"); 
  display.setCursor(64,45); 
  display.println(" Show "); 
  display.display();
  delay(4000);
  display.clearDisplay();   


  /*****Display Part2*****/
  /*****POSITION1-2*****/
  for(i=0;i<6;i++)
  {       
    display.drawCircle(64, 20+2*i, 10, WHITE);
    display.drawLine(64,30+2*i,64,54+0.6*i, WHITE);
    //Left Hand
    display.drawLine(64,35+1.8*i,54-0.4*i,45+0.6*i, WHITE);
    display.drawLine(54-0.4*i,45+0.6*i,59-2.6*5,45+i, WHITE);
    //Right Hand
    display.drawLine(64,35+1.8*i,74+1.2*i,45-0.2*i, WHITE);
    display.drawLine(74+1.2*i,45-0.2*i,69+1.2*i,45+i, WHITE);
    //Left Leg
    display.drawLine(64,54+0.6*i,59-2*i,59-0.4*i, WHITE);
    display.drawLine(59-2*i,59-0.4*i,54-i,64, WHITE);
    //Right Leg  
    display.drawLine(64,54+0.6*i,69+2*i,59-0.4*i, WHITE);
    display.drawLine(69+2*i,59-0.4*i,74+i,64, WHITE);

    display.display();
    delay(DELAY);

    display.clearDisplay();
  }
  /*****POSITION2-3*****/
  for(i=0;i<6;i++)
  {  
    display.drawCircle(64+2*i,30,10, WHITE);
    display.drawLine(64+2*i,40,64,57, WHITE);
    //Left Hand
    display.drawLine(64+1.2*i,44+0.2*i,52+1.6*i,48-0.2*i, WHITE);
    display.drawLine(52+1.6*i,48-0.2*i,46+3.6*i,50, WHITE);
    //Right Hand
    display.drawLine(64+1.2*i,44+0.2*i,80,44+0.2*i, WHITE);
    display.drawLine(80,44+0.2*i,75+2.2*i,50-i, WHITE);
    //Left Leg
    display.drawLine(64,57,49,57, WHITE);
    display.drawLine(49,57,49,64, WHITE); 
    //Right Leg  
    display.drawLine(64,57,79,57, WHITE);
    display.drawLine(79,57,79,64, WHITE);

    display.display();
    delay(DELAY);

    display.clearDisplay();  
  } 

  /*****POSITION3-4*****/
  for(i=0;i<6;i++)
  {  
    display.drawCircle(74-2.8*i,30-2*i,10, WHITE);
    display.drawLine(74-2.8*i,40-2*i,64-0.8*i,57-0.8*i, WHITE);
    //Left Hand
    display.drawLine(70-2*i,45-2*i,60-2.6*i,47-2.4*i, WHITE);
    display.drawLine(60-2.6*i,47-2.4*i,64-4.4*i,50-4*i, WHITE);
    //Right Hand
    display.drawLine(70-2*i,45-2*i,80-1.6*i,45+0.4*i, WHITE);
    display.drawLine(80-1.6*i,45+0.4*i,86-3.8*i,45+0.8*i, WHITE);
    //Left Leg
    display.drawLine(64-0.8*i,57-0.8*i,49+0.2*i,57-0.4*i, WHITE);
    display.drawLine(49+0.2*i,57-0.4*i,49-i,64-0.2*i, WHITE); 
    //Right Leg  
    display.drawLine(64-0.8*i,57-0.8*i,79-1.8*i,57+0.6*i, WHITE);
    display.drawLine(79-1.8*i,57+0.6*i,79-2.8*i,64, WHITE);

    display.display();
    delay(DELAY);

    display.clearDisplay();  
  } 

  /*****POSITION4-5*****/
  for(i=0;i<6;i++)
  {  
    display.drawCircle(60-1.2*i,20,10,  WHITE);
    display.drawLine(60-1.2*i,30,60-1.2*i,53-0.2*i, WHITE);
    //Left Hand
    display.drawLine(60-1.2*i,35,47-1.6*i,35, WHITE);
    display.drawLine(47-1.6*i,35,42-1.6*i,30, WHITE);
    //Right Hand
    display.drawLine(60-1.2*i,35,72-5.8*i,47-0.2*i, WHITE);
    display.drawLine(72-5.8*i,47-0.2*i,67-5.4*i,49-i, WHITE);
    //Left Leg
    display.drawLine(60-1.2*i,53-0.2*i,50-0.2*i,55-0.6*i, WHITE);
    display.drawLine(50-0.2*i,55-0.6*i,44,63, WHITE); 
    //Right Leg  
    display.drawLine(60-1.2*i,53-0.2*i,70-1.2*i,60-0.4*i, WHITE);
    display.drawLine(70-1.2*i,60-0.4*i,65+1.8*i,64, WHITE);

    display.display();
    delay(DELAY);

    display.clearDisplay();  
  } 

  /*****POSITION5-6*****/
  for(i=0;i<6;i++)
  {  
    display.drawCircle(54,20+i,10,  WHITE);
    display.drawLine(54,30+i,54,52+i, WHITE);
    //Left Hand
    display.drawLine(54,35+i,39,35+i, WHITE);
    display.drawLine(39,35+i,34,30+i, WHITE);
    //Right Hand
    display.drawLine(54,35+i,43+5.2*i,46-1.2*i, WHITE);
    display.drawLine(43+5.2*i,46-1.2*i,40+6.8*i,44-1.8*i, WHITE);
    //Left Leg
    display.drawLine(54,52+i,49-0.4*i,52+i, WHITE);
    display.drawLine(49-0.4*i,52+i,44-1.6*i,63, WHITE); 
    //Right Leg  
    display.drawLine(54,52+i,64-0.6*i,58-0.2*i, WHITE);
    display.drawLine(64-0.6*i,58-0.2*i,74-0.4*i,64-0.2*i, WHITE);

    display.display();
    delay(DELAY);

    display.clearDisplay();  
  } 
  /*****POSITION6-7*****/
  for(i=0;i<6;i++)
  {  
    display.drawCircle(54+3.6*i,25-0.6*i,10,  WHITE);
    display.drawLine(54+3.6*i,35-0.6*i,54+3.6*i,57-1.8*i, WHITE);
    //Left Hand
    display.drawLine(54+3.6*i,40-0.2*i,39+9.6*i,40-0.2*i, WHITE);
    display.drawLine(39+9.6*i,40-0.2*i,34+11.6*i,35+0.8*i, WHITE);
    //Right Hand
    display.drawLine(54+3.6*i,40,69-1.4*i,40, WHITE);
    display.drawLine(69-1.4*i,40,74-4*i,35+2.6*i, WHITE);
    //Left Leg
    display.drawLine(54+3.6*i,57-1.8*i,47+4*i,57-0.2*i, WHITE);
    display.drawLine(47+4*i,57-0.2*i,36+5.2*i,63+0.2*i, WHITE); 
    //Right Leg  
    display.drawLine(54+3.6*i,57-2.2*i,61+3*i,57-2.2*i, WHITE);
    display.drawLine(61+3*i,57-2.2*i,72+2.4*i,63+0.2*i, WHITE);

    display.display();
    delay(DELAY);

    display.clearDisplay();  
  } 

  /*****POSITION7-8*****/
  for(i=0;i<6;i++)
  {  
    display.drawCircle(72+0.8*i,22-0.4*i,10,  WHITE);
    display.drawLine(72+0.8*i,32-0.4*i,72+0.8*i,48+1.2*i, WHITE);
    //Right Hand
    display.drawLine(72+0.8*i,40-0.8*i,62+4.4*i,40-0.8*i, WHITE);
    display.drawLine(62+4.4*i,40-0.8*i,54+4.4*i,48+0.4*i, WHITE);
    //Left Hand
    display.drawLine(72+0.8*i,39-0.8*i,87-4.2*i,39-0.8*i, WHITE);
    display.drawLine(87-4.2*i,39-0.8*i,92-6.4*i,39-0.8*i, WHITE); 
    //Left Leg
    display.drawLine(72+0.8*i,48+0.8*i,67-2.2*i,56-2.2*i, WHITE);
    display.drawLine(67-2.2*i,56-2.2*i,62-1.6*i,64-2.8*i, WHITE);
    //Right Leg  
    display.drawLine(72+0.8*i,46+1.6*i,76,46+2.8*i, WHITE);
    display.drawLine(76,46+2.8*i,84-1.6*i,64, WHITE);

    display.display();
    delay(DELAY);

    display.clearDisplay();  
  } 
  delay(400);
  /*****POSITION8-1*****/
  for(i=0;i<6;i++)
  {  
    display.drawCircle(76-2.4*i,20,10,  WHITE);
    display.drawLine(76-2.4*i,30,76-2.4*i,54, WHITE);
    //Left Hand
    display.drawLine(76-2.4*i,35,66-2.4*i,35+2*i, WHITE);
    display.drawLine(66-2.4*i,35+2*i,60-0.2*i,35+2*i, WHITE);
    //Right Hand
    display.drawLine(76-2.4*i,36-0.2*i,84-2*i,44+0.2*i, WHITE);
    display.drawLine(84-2*i,44+0.2*i,76-1.4*i,50-i, WHITE);
    //Left Leg
    display.drawLine(76-2.4*i,52+0.4*i,56+0.6*i,45+2.8*i, WHITE);
    display.drawLine(56+0.6*i,45+2.8*i,54,50+2.8*i, WHITE); 
    //Right Leg  
    display.drawLine(76-2.4*i,54,76-1.4*i,60-0.2*i, WHITE);
    display.drawLine(76-1.4*i,60-0.2*i,76-0.4*i,64, WHITE);

    display.display();
    delay(DELAY);

    display.clearDisplay();  
  } 
  delay(2000);

  display.drawBitmap(0,0,Image,64,64, WHITE);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(66,20);
  display.println("Welcome to"); 
  display.setCursor(80,32); 
  display.println("Visit"); 
  display.setCursor(62,44); 
  display.println("www.DFRobot"); 
  display.setCursor(80,56);
  display.println(".com! "); 
  display.display();  
  display.startscrollleft(0x00, 0x0F); 
  delay(21450);
  display.stopscroll();
}
void loop() {

}          



