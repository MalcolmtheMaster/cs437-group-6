#include "Equipment.h"

Equipment::Equipment() {
	this->statData.insert(std::pair<std::string, int> ("attack", 0));
	this->statData.insert(std::pair<std::string, int> ("magic", 0));	
	this->statData.insert(std::pair<std::string, int> ("defense", 0));	
	this->statData.insert(std::pair<std::string, int> ("hp", 0));	
	this->statData.insert(std::pair<std::string, int> ("mp", 0));	
}

Equipment::Equipment(int atk, int mag, int def, int health, int mana, std::string sprite) {
	this->statData.insert(std::pair<std::string, int> ("attack", atk));
	this->statData.insert(std::pair<std::string, int> ("magic", mag));	
	this->statData.insert(std::pair<std::string, int> ("defense", def));	
	this->statData.insert(std::pair<std::string, int> ("hp", health));	
	this->statData.insert(std::pair<std::string, int> ("mp", mana));i
	this->icon = resources.getSprite(sprite);	
}

Equipment::~Equipment() {

}

void Equipment::setEquipped(bool equip) {
	this -> equipped = equip;
}
