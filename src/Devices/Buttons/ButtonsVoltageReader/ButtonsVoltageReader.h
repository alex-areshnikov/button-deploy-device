#include <Arduino.h>

class ButtonsVoltageReader {
	private:
		static const int VOLTAGE_PIN = A0;	

	public:
		int voltage();
};