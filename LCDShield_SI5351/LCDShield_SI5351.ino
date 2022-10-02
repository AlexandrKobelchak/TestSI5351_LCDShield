/*
 Name:		LCDShield_SI5351.ino
 Created:	02-Oct-22 10:44:37
 Author:	Alexandr
*/
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

enum BUTTON:char { btnRIGHT, btnUP, btnDOWN, btnLEFT, btnSELECT, btnNONE };

volatile BUTTON btnValue = BUTTON::btnNONE;
volatile bool newValue;
volatile bool converted;
float freq = 1000000; // Оглашение переменных -- Частота
float bigStep = 1000; // Шаг изменения частоты при нажатии вверх/вниз
float littleStep = 10; // Шаг изменения частоты при нажатии вправо/влево 

inline BUTTON read_LCD_buttons(int value);
inline void setupAnalogInterrupt();
void printValue();
void sendFrequency(float freq);
void PrintFrequency(float freq);

// Interrupt service routine for the ADC completion
ISR(ADC_vect) {

	BUTTON value = read_LCD_buttons(ADC);

	if (btnValue != value) {

		btnValue = value;
		newValue = true;
	}
	converted = true;
}

inline void setupAnalogInterrupt() {

	//  ADC Setup
	ADCSRA = 0b10001111;      // C1:: Enable ADC & Interrupt, pre-scaler = 128
	ADMUX = 0b01000000;      // C2:: Use AVCC and A0 as input to ADC
	sei();                    // C3:: Set global interrupt flag

	converted = true;
	newValue = false;
}

void sendFrequency(float freq) {

}

void setup() {
	//Serial.begin(9600);
	lcd.begin(16, 2); // Старт библиотеки. Указанием количества символов и строк
	setupAnalogInterrupt();
	updateData();
}


void loop() {

	unsigned long mil = 0;

	if (newValue || mil + 1000 < millis()) {

		updateData();
		newValue = false;
		mil = millis();
		delay(100);             // C5:: give you time to see the ouput
	}

	if (converted) {

		ADCSRA |= bit(ADSC) | bit(ADIE);   // C4:: start converting voltage on A0
		converted = false;
	}

	delay(200); // Пауза 200 мс
}
inline BUTTON read_LCD_buttons(int value) { // Функция считывания нажатия кнопок

	if (value > 1000) return BUTTON::btnNONE;
	if (value < 50)   return BUTTON::btnRIGHT;
	if (value < 150)  return BUTTON::btnUP;
	if (value < 315)  return BUTTON::btnDOWN;
	if (value < 600)  return BUTTON::btnLEFT;
	if (value < 850)  return BUTTON::btnSELECT;
	return BUTTON::btnNONE;
}


void printValue() {

	static char* arr[] = { "RIGHT", "UP", "DOWN", "LEFT", "SELECT", "NONE" };

	lcd.setCursor(0, 1);
	lcd.print("        ");
	lcd.setCursor(0, 1);
	lcd.print(arr[btnValue]);
}

inline void updateStep() {
	if (freq < 100) {
		bigStep = 10; // Определение шага грубой и точной
		littleStep = 1;
	} // настройки в зависимости от частоты
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
}

inline void updateFrequncy(BUTTON btn) {


	switch (btn) // Далее обработка нажатий клавиш          
	{
	case btnRIGHT:
		freq += littleStep;
		break;
	case btnLEFT:
		freq -= littleStep;
		break;
	case btnUP:
		freq += bigStep;
		break;
	case btnDOWN:
		freq == bigStep ? freq -= (bigStep / 10) : freq -= bigStep;
		break;
	case btnSELECT:
		break;
	case btnNONE:
		break;
	}
	if (freq < 1) freq = 1; // Ограничение значений частоты
	if (freq > 40000000) freq = 40000000;

}

void updateData() {
	updateStep();
	updateFrequncy(btnValue);
	sendFrequency(freq);  // Вызов функции отправки частоты
	PrintFrequency(freq);
}

void PrintFrequency(float freq) {

	lcd.setCursor(0, 0); // Далее вывод текущего значения частоты
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
}


