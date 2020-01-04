#include <ArduinoJson.h>
#include "../../AWS/constants.h"

class MuxDocumentsProcessor {
	private:		
		JsonDocument* awsIotJson;
		int desiredStep;
		int digDesiredStep();
		
	public:
		MuxDocumentsProcessor(JsonDocument* jsonDocument);		

		void process();
		int step();
		bool hasValidStep();		
		bool hasCurrentKey();
		bool hasStateKey();
		bool hasDesiredKey();
		bool hasStepKey();
};