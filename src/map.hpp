/*-------------------------------------------------------------------------------

BARONY
File: main.hpp
Desc: contains some prototypes as well as various type definitions

Copyright 2013-2016 (c) Turning Wheel LLC, all rights reserved.
See LICENSE for details.

-------------------------------------------------------------------------------*/

#include "main.hpp";
#include "entity.hpp";

// game world structure
class Map {
	private:
		char name[32];   // name of the map
		char author[32]; // author of the map
		unsigned int width, height;  // size of the map
		Sint32 *tiles;
		list<Entity *> entities;

	public:
		Map();
		string getName();
		string getAuthor();
		unsigned int getWidth();
		unsigned int getHeight();
		Sint32 *getTiles();
		list<Entity*> getEntities();
		int loadMap(string filename2);
		int loadMap(string filename2, list_t* entlist);
		void addEntity(Entity *);
		Entity * getEntityAt(int index);
};