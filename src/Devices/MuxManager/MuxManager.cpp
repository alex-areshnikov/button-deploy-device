#include "MuxManager.h"

MuxManager::MuxManager(int s1, int s2, int s3, int s4) {
  wakeupper = new Wakeupper(WAKEUP_INTERVAL_MS);
  muxController = new CD74HC4067(s1, s2, s3, s4);
  
  readyStep();
};

void MuxManager::step(int step, bool erred) {
  currentStep = step;
  this->erred = erred;

  if(erred) { erredStep = step; }

  set(currentStep);
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
  if(!wakeupper->isWakeupTime()) return;

  if(currentStep == ERROR_STEP) {
    currentStep = erredStep;
  } else {
    currentStep = ERROR_STEP;
  }

  set(currentStep);
};

// private

void MuxManager::set(int step) {
  muxController->channel(step);
}
