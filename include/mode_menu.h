#ifndef CSCI437_MODE_MENU_H
#define CSCI437_MODE_MENU_H

#include "mode.h"

class MenuMode : public Mode
{

public:
	MenuMode();
	~MenuMode() = default;
	void update() override;

private:
	int selectionIndex;

};

#endif //CSI437_MODE_MENU_H