#include "ButtonsHistory.h"

ButtonsHistory::ButtonsHistory(const char* noPressButtonCode) {
	this->buttonCode = noPressButtonCode;
	this->prevButtonCode = noPressButtonCode;
}

void ButtonsHistory::update(const char* newButtonCode) {
	if(this->prevButtonCode == this->buttonCode && this->buttonCode == newButtonCode) {
		return;
	}

	this->prevButtonCode = this->buttonCode;
	this->buttonCode = newButtonCode;
};

bool ButtonsHistory::isStatusChanged() {
	return(this->prevButtonCode != this->buttonCode);
};

const char* ButtonsHistory::previousButtonCode() {
	return(this->prevButtonCode);
};

const char* ButtonsHistory::currentButtonCode() {
	return(this->buttonCode);
};