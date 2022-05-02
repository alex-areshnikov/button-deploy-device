#include <Adafruit_Fingerprint.h>
#include "../../TFTScreen/TFTScreen.h"

namespace Fingerprint {
	class Reader {
		private:
			Adafruit_Fingerprint *middleFinger;
			TFTScreen *screen;
			void (*scanSuccessCallback)(int);
			void (*scanFailureCallback)();
			
		public:
			Reader(Adafruit_Fingerprint*, TFTScreen*, void (*)(int), void (*)());

			void process();
	};
};