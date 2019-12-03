#include "ButtonsVoltageReader.h"

int ButtonsVoltageReader::voltage() {
	return(analogRead(ButtonsVoltageReader::VOLTAGE_PIN));
}