class ButtonParser {
	private:
		static const int DOWN_BUTTON_MIN_VOLTAGE = 600;
		static const int DOWN_BUTTON_MAX_VOLTAGE = 700;
		static const int SELECT_BUTTON_MIN_VOLTAGE = 700;
		static const int SELECT_BUTTON_MAX_VOLTAGE = 800;			
		static const int UP_BUTTON_MIN_VOLTAGE = 800;
		static const int UP_BUTTON_MAX_VOLTAGE = 900;
		static const int DEPLOY_BUTTON_MIN_VOLTAGE = 900;
		static const int DEPLOY_BUTTON_MAX_VOLTAGE = 1024;

	public:
		static const char* NO_BUTTON;
		static const char* DOWN_BUTTON;
		static const char* SELECT_BUTTON;
		static const char* UP_BUTTON;
		static const char* DEPLOY_BUTTON;
		
		const char* buttonCode(int);
};