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
		vector<vector<vector<Sint32>>> tiles;
		list<Entity *> entities;
		list<Door *> doors;

	public:
		Map();
		~Map();
		string getName();
		string getAuthor();
		unsigned int getWidth();
		unsigned int getHeight();
		Sint32 getTile(int x, int y, int z);
		void setTile(int x, int y, int z, Sint32 value);
		list<Entity*> getEntities();
		int loadMap(string filename2);
		int loadMap(string filename2, list_t* entlist);
		void addEntity(Entity *);
		Entity * getEntityAt(int index);
		void addDoor(Door *);
};

// Door class (used in map generation)
class Door {
	private:
		Sint32 x, y;
		Sint32 dir; // 0: east, 1: south, 2: west, 3: north
	public:
		Door(Sint32 x, Sint32 y, Sint32 dir);
};