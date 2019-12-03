#include "MuxManager.h"

MuxManager::MuxManager(int s1, int s2, int s3, int s4) {
  this->muxController = new CD74HC4067(s1, s2, s3, s4);
  this->currentStep = 0;
  this->readyStep();
};

void MuxManager::readyStep() {
  this->muxController->channel(0);
};

void MuxManager::nextStep() {
  if(++this->currentStep > 15) {
    this->currentStep = 0;
  }

  this->muxController->channel(this->currentStep);
};

void MuxManager::prevStep() {
  if(--this->currentStep < 0) {
    this->currentStep = 15;
  }

  this->muxController->channel(this->currentStep);
};
