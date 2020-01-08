#include <ArduinoJson.h>
#include "../../AWS/constants.h"

class DocumentsProcessor {
	private:		
		JsonDocument* awsIotJson;
		int desiredStep;
		bool fingerprintEnroll;

		int digDesiredStep();
		bool containsDesiredKey(String);
		
	public:		
		int fingerprintEnrollId;

		DocumentsProcessor(JsonDocument* jsonDocument);		
		void process();
		int step();
		bool hasValidStep();		
		bool isErred();
		bool hasCurrentKey();
		bool hasStateKey();
		bool hasDesiredKey();			
		bool erred();			
		bool isFingerprintEnroll();
		bool hasFingerprintKey();
};