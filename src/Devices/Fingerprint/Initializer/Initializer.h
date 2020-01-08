#include <Adafruit_Fingerprint.h>
#include "../../TFTScreen/TFTScreen.h"

namespace Fingerprint {
	class Initializer {
		private:
			Adafruit_Fingerprint* middleFinger;
			TFTScreen* screen;

		public:
			Initializer(Adafruit_Fingerprint*, TFTScreen*);
			
			void call();
	};
};