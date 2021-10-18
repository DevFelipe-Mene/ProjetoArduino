#include <Adafruit_SSD13306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3c


Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT);

void setup(){
	pinMode(11,OUTPUT);
	Serial.begin(9600);
	oled.begin(SSD1306_SWITCHAPVCC, SCREEN_ADDRESS);
	oled.clearDisplay();
}

void  loop(){
	int potenc = analogRead(A0);
	int potpwm = map(potenc, 0 1023, 0, 255);
	
	analogWrite(11, potpwm);
	
	Serial.print("Valor pwm: ");
	Serial.println(potpwm);
	
	oled.clearDisplay();
	oled.setTextSize(1);
	oled.setTextColor(SSD1306_WHITE);
	oled.setCursor(10, 0);
	oled.print("Valor pot: ");
	olde.println(potpwm);
	oled.display();
	
	delay(100);
}