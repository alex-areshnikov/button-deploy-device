#include "MuxDocumentsProcessor.h"

MuxDocumentsProcessor::MuxDocumentsProcessor(JsonDocument* jsonDocument) {
	awsIotJson = jsonDocument;
	desiredStep = -1;
};

void MuxDocumentsProcessor::process() {
	if(hasStepKey()) { 
		desiredStep = digDesiredStep();
	}
};

bool MuxDocumentsProcessor::hasValidStep() {
	return(desiredStep >=0 && desiredStep < 15);
};

bool MuxDocumentsProcessor::isErred() {
	return(hasErrorKey() && erred());
};

int MuxDocumentsProcessor::step() {
	return(desiredStep);
}

int MuxDocumentsProcessor::digDesiredStep() {
	return((*awsIotJson)[AWS_IOT_CURRENT_KEY][AWS_IOT_STATE_KEY][AWS_IOT_DESIRED_KEY]["step"].as<int>());
};

bool MuxDocumentsProcessor::hasCurrentKey() {
	if((*awsIotJson).containsKey(AWS_IOT_CURRENT_KEY)) { return(true); }
	return(false);
};

bool MuxDocumentsProcessor::hasStateKey() {
	if(!hasCurrentKey()) { return(false); }
	if((*awsIotJson)[AWS_IOT_CURRENT_KEY].containsKey(AWS_IOT_STATE_KEY)) { return(true); }
	return(false);
};

bool MuxDocumentsProcessor::hasDesiredKey() {
	if(!hasStateKey()) { return(false); }
	if((*awsIotJson)[AWS_IOT_CURRENT_KEY][AWS_IOT_STATE_KEY].containsKey(AWS_IOT_DESIRED_KEY)) { return(true); }
	return(false);
};

bool MuxDocumentsProcessor::hasStepKey() {
	if(!hasDesiredKey()) { return(false); }
	if((*awsIotJson)[AWS_IOT_CURRENT_KEY][AWS_IOT_STATE_KEY][AWS_IOT_DESIRED_KEY].containsKey("step")) { return(true); }
	return(false);
};

bool MuxDocumentsProcessor::hasErrorKey() {
	if(!hasDesiredKey()) { return(false); }
	if((*awsIotJson)[AWS_IOT_CURRENT_KEY][AWS_IOT_STATE_KEY][AWS_IOT_DESIRED_KEY].containsKey("error")) { return(true); }
	return(false);
};

bool MuxDocumentsProcessor::erred() {
	return((*awsIotJson)[AWS_IOT_CURRENT_KEY][AWS_IOT_STATE_KEY][AWS_IOT_DESIRED_KEY]["error"].as<bool>());
};