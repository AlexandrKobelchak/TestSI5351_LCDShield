/*
 Name:		LCDShield_SI5351.ino
 Created:	02-Oct-22 10:44:37
 Author:	Alexandr
*/
#include <LiquidCrystal.h>


LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

float freq = 10000; // ��������� ���������� -- �������
float bigStep = 1000; // ��� ��������� ������� ��� ������� �����/����
float littleStep = 10; // ��� ��������� ������� ��� ������� ������/����� 
int lcd_key = 0;
int adc_key_in = 0;
#define btnRIGHT  0 // �������� �������� ��� ������
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

int read_LCD_buttons() { // ������� ���������� ������� ������
	adc_key_in = analogRead(0);
	if (adc_key_in > 1000) return btnNONE;
	if (adc_key_in < 50)   return btnRIGHT;
	if (adc_key_in < 150)  return btnUP;
	if (adc_key_in < 315)  return btnDOWN;
	if (adc_key_in < 600)  return btnLEFT;
	if (adc_key_in < 850)  return btnSELECT;
	return btnNONE;
}

void sendFrequency(double frequency) { // �������������� � �������� 


}

void setup() {

	lcd.begin(16, 2); // ����� ����������. ��������� ���������� �������� � �����
}

// the loop function runs over and over again until power down or reset
void loop() {
  
	lcd.setCursor(0, 0); // ����� ����� �������� �������� �������
	lcd.print("Freq: ");
	lcd.setCursor(6, 0);
	lcd.print("           ");
	lcd.setCursor(6, 0);
	if (freq < 1000) {
		lcd.print(freq);
		lcd.print("Hz");
	}
	if ((freq >= 1000) && (freq < 1000000)) {
		lcd.print(freq / 1000);
		lcd.print("kHz");
	}
	if ((freq >= 1000000) && (freq < 50000000)) {
		lcd.print(freq / 1000000);
		lcd.print("MHz");
	}
	lcd.setCursor(0, 1);
	lcd.print("   Genie v1.0");
	if (freq < 100) {
		bigStep = 10; // ����������� ���� ������ � ������
		littleStep = 1;
	} // ��������� � ����������� �� �������
	if ((freq >= 100) && (freq < 1000)) {
		bigStep = 100;
		littleStep = 1;
	}
	if ((freq >= 1000) && (freq < 10000)) {
		bigStep = 1000;
		littleStep = 10;
	}
	if ((freq >= 10000) && (freq < 100000)) {
		bigStep = 10000;
		littleStep = 100;
	}
	if ((freq >= 100000) && (freq < 1000000)) {
		bigStep = 100000;
		littleStep = 1000;
	}
	if ((freq >= 1000000) && (freq < 10000000)) {
		bigStep = 1000000;
		littleStep = 10000;
	}
	if ((freq >= 10000000) && (freq < 40000000)) {
		bigStep = 10000000;
		littleStep = 100000;
	}
	lcd_key = read_LCD_buttons();  // ���������� ������
	switch (lcd_key) // ����� ��������� ������� ������          
	{
	case btnRIGHT:
	{freq += littleStep;
	break; }
	case btnLEFT:
	{freq -= littleStep;
	break; }
	case btnUP:
	{freq += bigStep;
	break; }
	case btnDOWN:
	{if (freq == bigStep) { freq -= (bigStep / 10); }
	else { freq -= bigStep; }
	break; }
	case btnSELECT:
	{break; }
	case btnNONE:
	{break; }
	}
	if (freq < 1) freq = 1; // ����������� �������� �������
	if (freq > 40000000) freq = 40000000;
	
	sendFrequency(freq);  // ����� ������� �������� �������
	
	delay(200); // ����� 200 ��
}
