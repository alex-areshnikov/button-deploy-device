#include "MuxManager.h"

MuxManager::MuxManager(int s1, int s2, int s3, int s4) {
  muxController = new CD74HC4067(s1, s2, s3, s4);
  
  readyStep();
};

void MuxManager::step(int step, bool erred) {  
  this->erred = erred;
  if(erred) { erredStep = step; }
  set(step);
};

int MuxManager::step() {
  return currentStep;
};

void MuxManager::readyStep() {  
  step(READY_STEP);
};

void MuxManager::nextStep() {
  if(++currentStep > 15) {
    currentStep = 0;
  }

  step(currentStep);
};

void MuxManager::prevStep() {
  if(--currentStep < 0) {
    currentStep = 15;
  }

  step(currentStep);
};

void MuxManager::ledCheck() {
  for(int i=0; i<15; i++) {
    nextStep();
    delay(50);
  }

  for(int i=0; i<15; i++) {
    prevStep();
    delay(50);
  }
};

void MuxManager::process() {
  if(!erred) return;

  if(currentStep == ERROR_STEP) {
    set(erredStep);
  } else {
    set(ERROR_STEP);
  }
};

// private

void MuxManager::set(int step) {
  currentStep = step;
  muxController->channel(step);
}
