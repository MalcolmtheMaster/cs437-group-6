#include "controller_menu.h"
#include "mode_menu.h"
#include <iostream>
#include "macros.h"

void MenuController::process_input(float delta)
{

}


void MenuController::handle_event(sf::Event event)
{
	//gets mode object
	//enusres menu mode selection index is within proper range
	auto mode = std::dynamic_pointer_cast<MenuMode>(state->get_mode());

	/*since this function is called inside the poll event loop, there is an OS delay which produces the
	desired cursor movement on the menu*/
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		(*mode).selectionIndex = ((*mode).selectionIndex + NUM_MENU_BUTTONS - 1) % NUM_MENU_BUTTONS;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		(*mode).selectionIndex = ((*mode).selectionIndex + 1) % NUM_MENU_BUTTONS;

	printf("%d\n", (*mode).selectionIndex);
}