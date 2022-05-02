#include "ButtonsManager.h"

ButtonsManager::ButtonsManager() {
  buttonsVoltageReader = new ButtonsVoltageReader();
  buttonParser = new ButtonParser();
  buttonsHistory = new ButtonsHistory(ButtonParser::NO_BUTTON);
};

void ButtonsManager::process(void (*buttonsActionCallback)(const char* code)) {
  int voltage = buttonsVoltageReader->voltage();
  const char* button_code = buttonParser->buttonCode(voltage);  
  buttonsHistory->update(button_code);

  if(buttonsHistory->isStatusChanged()) {
    (*buttonsActionCallback)(buttonsHistory->currentButtonCode());
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
