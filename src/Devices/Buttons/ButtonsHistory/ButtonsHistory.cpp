#include "ButtonsHistory.h"

ButtonsHistory::ButtonsHistory(const char* noPressButtonCode) {
	buttonCode = noPressButtonCode;
	prevButtonCode = noPressButtonCode;
}

void ButtonsHistory::update(const char* newButtonCode) {
	if(prevButtonCode == buttonCode && buttonCode == newButtonCode) {
		return;
	}

	prevButtonCode = buttonCode;
	buttonCode = newButtonCode;
};

bool ButtonsHistory::isStatusChanged() {
	return(prevButtonCode != buttonCode);
};

const char* ButtonsHistory::previousButtonCode() {
	return(prevButtonCode);
};

const char* ButtonsHistory::currentButtonCode() {
	return(buttonCode);
};