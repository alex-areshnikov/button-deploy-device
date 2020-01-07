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
		bool isErred();
		bool hasCurrentKey();
		bool hasStateKey();
		bool hasDesiredKey();
		bool hasStepKey();
		bool hasErrorKey();
		bool erred();
};