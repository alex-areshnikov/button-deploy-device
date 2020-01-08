#include <Adafruit_Fingerprint.h>
#include "../../TFTScreen/TFTScreen.h"
#include "../../../Services/Wakeupper/Wakeupper.h"

namespace Fingerprint {
	class Reader {
		private:
			static const int WAKEUP_INTERVAL_MS = 1000;

			Adafruit_Fingerprint *middleFinger;
			TFTScreen *screen;
			Wakeupper *wakeupper;
			
		public:
			Reader(Adafruit_Fingerprint*, TFTScreen*);

			void process();
	};
};