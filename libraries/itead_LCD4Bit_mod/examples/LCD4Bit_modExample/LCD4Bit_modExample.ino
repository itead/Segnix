#include <itead_LCD4Bit_mod.h> 

LCD4Bit_mod lcd = LCD4Bit_mod(2); 

void setup() { 
  lcd.init();
  lcd.clear();
  lcd.cursorTo(1,5);
  lcd.printIn((char *)"setup");
  delay(1000);
}

void loop() {
	static char cnt = 0;
	char str[2]={'0','\0'};
	cnt ++;
	str[0]= cnt%10 + '0';	
	
	lcd.clear();
 	lcd.cursorTo(1,0);
	lcd.printIn((char *)"loop cnt=");
	lcd.printIn(str);
	
	lcd.cursorTo(2,3);
	lcd.printIn((char *)"loop cnt=");
	lcd.printIn(str);
	delay(500);
	lcd.leftScroll(3,500);
	delay(1000);
}
