#include "../ButtonsVoltageReader/ButtonsVoltageReader.h"
#include "../ButtonParser/ButtonParser.h"
#include "../ButtonsHistory/ButtonsHistory.h"

class ButtonsManager {
	private:	
		ButtonsVoltageReader *buttonsVoltageReader;
		ButtonParser *buttonParser;
		ButtonsHistory *buttonsHistory;
		
	public:
		ButtonsManager();

		void process(void (*)(const char*));
		bool isDownButton(const char*);
		bool isUpButton(const char*);
		bool isSelectButton(const char*);
};