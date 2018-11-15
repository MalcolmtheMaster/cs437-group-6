#include "skeleton.h"
#include "level_factory.h"
#include "EventManager.h"
#include "Player.h"
#include "gold.h"
#include "alg_agent_based.h"

/**
 * Returns a pointer to a newly created level, which is built according to the parameters specified through the setter
 * methods
 */
std::shared_ptr<Level> LevelFactory::generate_level()
{
  std::shared_ptr<Level> level;
  std::shared_ptr<Map> map;

  // TODO Generate all the level's entities

  std::shared_ptr<Player> player = std::make_shared<Player>(3150, 1000, 10);
  //std::shared_ptr<Skeleton> enemy = std::make_shared<Skeleton>(450, 250);
  //std::shared_ptr<Gold> gold = std::make_shared<Gold>(350, 250);

  //NOTE THAT NUMBER OF ENEMIES HAS TO BE LARGER THAN THE NUMBER OF ROOMS!!!
  AgentBasedGenerator gen = AgentBasedGenerator(128, 106, 1, 10, 0);

  // TODO Can specify the map generation algorithm (load from file, randomly generated, etc)
  switch (algorithm)
  {
    case LEVEL_FILE:
      player->set_position(150, 150);
      map = load("../data/test2.txt");
	    break;
	  case AGENT_BASED:
     
		  map = std::make_shared<Map>(gen.createLevelGrid(15, 40, 32.0));
		  //gen.printLevelGrid();
		  player->set_position(gen.player_x, gen.player_y);
		  //enemy->set_position(3150, 800);
		  //gold->set_position(3050, 800);
		  break;
  }

  // Create the actual level
  level = std::make_shared<Level>(map);

  level->set_player(player);
  level->get_entities().push_back(player);
  //TEMP method to place enemies
  for (int i = 0; i < gen.enemy_coords.size(); i++)
  {
	  std::shared_ptr<Skeleton> enemy = std::make_shared<Skeleton>(gen.enemy_coords[i][0], gen.enemy_coords[i][1]);
	  level->get_entities().push_back(enemy);
  }
  //level->get_entities().push_back(enemy);
  //level->get_entities().push_back(gold);

  return level;
}

std::shared_ptr<Map> LevelFactory::load(std::string filename)
{
  std::ifstream fin;
  std::string linestr;
  std::vector<std::vector<char>> grid;
  std::vector<char> temp;
  fin.open(filename, std::ios::in);

  while (std::getline(fin, linestr))
  {
    for (auto c : linestr)
    {
      temp.push_back(c);
    }
    grid.push_back(temp);
    temp.clear();
  }

  fin.close();
  return std::make_shared<Map>(grid);
}
