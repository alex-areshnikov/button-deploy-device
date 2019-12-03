#include "ButtonsManager.h"

ButtonsManager::ButtonsManager() {
  this->wakeupper = new Wakeupper(WAKEUP_INTERVAL_MS);
  this->buttonsVoltageReader = new ButtonsVoltageReader();
  this->buttonParser = new ButtonParser();
  this->buttonsHistory = new ButtonsHistory(ButtonParser::NO_BUTTON);
};

void ButtonsManager::process(void (*buttonsActionCallback)(const char* code)) {
  if (!this->wakeupper->isWakeupTime()) {
    return;
  }

  int voltage = this->buttonsVoltageReader->voltage();
  const char* button_code = this->buttonParser->buttonCode(voltage);  
  this->buttonsHistory->update(button_code);

  if(this->buttonsHistory->isStatusChanged()) {
    (*buttonsActionCallback)(this->buttonsHistory->currentButtonCode());
  }
};

bool ButtonsManager::isDownButton(const char* buttonCode) {
  return(ButtonParser::DOWN_BUTTON == buttonCode);
}

bool ButtonsManager::isUpButton(const char* buttonCode) {
  return(ButtonParser::UP_BUTTON == buttonCode);
}

bool ButtonsManager::isSelectButton(const char* buttonCode) {
  return(ButtonParser::SELECT_BUTTON == buttonCode);
}
