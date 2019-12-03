#include "ButtonParser.h"

const char* ButtonParser::NO_BUTTON = "no_button_pressed";
const char* ButtonParser::DOWN_BUTTON = "down_button_pressed";
const char* ButtonParser::SELECT_BUTTON = "select_button_pressed";	
const char* ButtonParser::UP_BUTTON = "up_button_pressed";
const char* ButtonParser::DEPLOY_BUTTON = "deploy_button_pressed";

const char* ButtonParser::buttonCode(int voltage) {
	if(voltage >= DOWN_BUTTON_MIN_VOLTAGE && voltage < DOWN_BUTTON_MAX_VOLTAGE) {    
    return(DOWN_BUTTON);
  }

  if(voltage > SELECT_BUTTON_MIN_VOLTAGE && voltage <= SELECT_BUTTON_MAX_VOLTAGE) {    
    return(SELECT_BUTTON);
  }

  if(voltage > UP_BUTTON_MIN_VOLTAGE && voltage <= UP_BUTTON_MAX_VOLTAGE) {   
    return(UP_BUTTON);
  }

  if(voltage > DEPLOY_BUTTON_MIN_VOLTAGE && voltage <= DEPLOY_BUTTON_MAX_VOLTAGE) {    
    return(DEPLOY_BUTTON);
  }

  return(NO_BUTTON);
};