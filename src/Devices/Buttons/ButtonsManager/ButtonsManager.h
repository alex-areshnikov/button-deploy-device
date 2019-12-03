#include "../ButtonsVoltageReader/ButtonsVoltageReader.h"
#include "../ButtonParser/ButtonParser.h"
#include "../ButtonsHistory/ButtonsHistory.h"
#include "../../../Services/Wakeupper/Wakeupper.h"

class ButtonsManager {
	private:		
		static const int WAKEUP_INTERVAL_MS = 50;

		Wakeupper *wakeupper;
		ButtonsVoltageReader *buttonsVoltageReader;
		ButtonParser *buttonParser;
		ButtonsHistory *buttonsHistory;
		
	public:
		ButtonsManager();

		void process(void (*buttonsActionCallback)(const char*));
		bool isDownButton(const char*);
		bool isUpButton(const char*);
		bool isSelectButton(const char*);
};