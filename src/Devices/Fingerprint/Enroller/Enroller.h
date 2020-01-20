#include <Adafruit_Fingerprint.h>
#include "../../TFTScreen/TFTScreen.h"

namespace Fingerprint {
	class Enroller {
			Adafruit_Fingerprint* middleFinger;
			TFTScreen* screen;
			int enrollId;

			void scanFinger();
			uint8_t convertImage(int);
			void waitNoFinger();
			uint8_t checkImages();
			uint8_t stroreFingerprint();

			void reportError();

		public:
			static const uint8_t ENROLL_FAIL = -1;
			static const uint8_t ENROLL_OK = 1;

			Enroller(Adafruit_Fingerprint*, TFTScreen*, int);

			uint8_t call();
	};
};