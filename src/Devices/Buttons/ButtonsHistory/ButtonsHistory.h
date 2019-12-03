class ButtonsHistory {
	private:
		const char* buttonCode;
		const char* prevButtonCode;
		const char* noPressButtonCode;

	public:
		ButtonsHistory(const char*);
		void update(const char*);
		bool isStatusChanged();
		const char* currentButtonCode();
		const char* previousButtonCode();
};