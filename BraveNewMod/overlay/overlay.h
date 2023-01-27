#pragma once
#include <deque>
// Overlay Modules
class Logging {
public:
	bool scroll; std::deque<char*> Items;
	void Render(), Log(const char* fmt...);
};

class LogWindow {
public:
	LogWindow(); void Render(); bool IsOpen;
	std::map<const char*, Logging*> windows;
};

//#define show(EXP) \
	//User.log.windows[#EXP]->Log

class IM_Interface {
public:
	bool IsOpen, ShowDemo;
	//LogWindow log; 
	void MainMenuBar();
	void Render();
};
