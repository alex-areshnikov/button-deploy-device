#include "DocumentsProcessor.h"

DocumentsProcessor::DocumentsProcessor(JsonDocument* jsonDocument) {
	awsIotJson = jsonDocument;
	desiredStep = -1;
	fingerprintEnrollId = -1;
	fingerprintEnroll = false;
};

void DocumentsProcessor::process() {
	if(containsDesiredKey("step")) { 
		desiredStep = digDesiredStep();
	}

	if(hasFingerprintKey()) {
		fingerprintEnroll = (*awsIotJson)[AWS_IOT_CURRENT_KEY][AWS_IOT_STATE_KEY][AWS_IOT_DESIRED_KEY]["fingerprint"]["enroll"].as<bool>();
		fingerprintEnrollId = (*awsIotJson)[AWS_IOT_CURRENT_KEY][AWS_IOT_STATE_KEY][AWS_IOT_DESIRED_KEY]["fingerprint"]["id"].as<int>();
	}
};

bool DocumentsProcessor::hasValidStep() {
	return(desiredStep >=0 && desiredStep < 15);
};

bool DocumentsProcessor::isErred() {
	return(containsDesiredKey("error") && erred());
};

int DocumentsProcessor::step() {
	return(desiredStep);
}

int DocumentsProcessor::digDesiredStep() {
	return((*awsIotJson)[AWS_IOT_CURRENT_KEY][AWS_IOT_STATE_KEY][AWS_IOT_DESIRED_KEY]["step"].as<int>());
};

bool DocumentsProcessor::hasCurrentKey() {
	if((*awsIotJson).containsKey(AWS_IOT_CURRENT_KEY)) { return(true); }
	return(false);
};

bool DocumentsProcessor::hasStateKey() {
	if(!hasCurrentKey()) { return(false); }
	if((*awsIotJson)[AWS_IOT_CURRENT_KEY].containsKey(AWS_IOT_STATE_KEY)) { return(true); }
	return(false);
};

bool DocumentsProcessor::hasDesiredKey() {
	if(!hasStateKey()) { return(false); }
	if((*awsIotJson)[AWS_IOT_CURRENT_KEY][AWS_IOT_STATE_KEY].containsKey(AWS_IOT_DESIRED_KEY)) { return(true); }
	return(false);
};

bool DocumentsProcessor::erred() {
	return((*awsIotJson)[AWS_IOT_CURRENT_KEY][AWS_IOT_STATE_KEY][AWS_IOT_DESIRED_KEY]["error"].as<bool>());
};

bool DocumentsProcessor::hasFingerprintKey() {
	if(!containsDesiredKey("fingerprint")) return(false);
};

bool DocumentsProcessor::isFingerprintEnroll() {
	return (fingerprintEnroll && fingerprintEnrollId > 0 && fingerprintEnrollId < 100);
};

// private

bool DocumentsProcessor::containsDesiredKey(String key) {
	if(!hasDesiredKey()) { return(false); }
	if((*awsIotJson)[AWS_IOT_CURRENT_KEY][AWS_IOT_STATE_KEY][AWS_IOT_DESIRED_KEY].containsKey(key)) { return(true); }
	return(false);
};