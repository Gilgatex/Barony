#include "map.hpp";
#include "sound.hpp";

Map::Map() {
}

Map::~Map() {
}

string Map::getName() {
	return this->name;
}

string Map::getAuthor() {
	return this->name;
}

unsigned int Map::getWidth() {
	return this->width;
}

unsigned int Map::getHeight() {
	return this->height;
}

Sint32 Map::getTile(int x, int y, int z) {
	return this->tiles[x][y][z];
}

void Map::setTile(int x, int y, int z, Sint32 value) {
	this->tiles[x][y][z] = value;
}

list<Entity*> Map::getEntities()
{
	return this->entities;
}

/*-------------------------------------------------------------------------------

loadMap

Loads a map from the given filename

TODO: make the passed entity list a C++ list

-------------------------------------------------------------------------------*/

int Map::loadMap(string filename2) {
	this->loadMap(filename2, NULL);
}

int Map::loadMap(string filename2, list_t* entlist) {
	FILE *fp;
	char valid_data[11];
	Uint32 numentities;
	Uint32 c;
	Sint32 x, y;
	Entity *entity;
	Sint32 sprite;
	string filename;

	string oldmapname;
	oldmapname = this->name;

	if (filename2 == "") {
		filename = "maps/" + filename2;

		if (filename2 != ".." && filename2 != ".") {
			// add extension if missing
			if (!filename.find(".lmp"))
				filename + ".lmp";

			// load the file!
			if ((fp = fopen(filename.c_str(), "rb")) == NULL) {
				printlog("warning: failed to open file '%s' for map loading!\n", filename);
				/*if (game) {
					printlog("error: main map failed to load, aborting.\n");
					mainloop = 0;
				}*/
				return -1;
			}
		}
		else {
			printlog("warning: failed to open file '%s' for map loading!\n", filename);
			/*if (game) {
				printlog("error: main map failed to load, aborting.\n");
				mainloop = 0;
			}*/
			return -1;
		}
		fread(valid_data, sizeof(char), strlen("BARONY"), fp);
		if (strncmp(valid_data, "BARONY", strlen("BARONY"))) {
			printlog("warning: file '%s' is an invalid map file.\n", filename);
			fclose(fp);
			/*if (game) {
				printlog("error: main map failed to load, aborting.\n");
				mainloop = 0;
			}*/
			return -1;
		}
		// remove all the entities
		if (entlist != NULL)
			list_FreeAll(entlist);

		// remove old lights
		list_FreeAll(&light_l);

		fread(this->name, sizeof(char), 32, fp); // map name
		fread(this->author, sizeof(char), 32, fp); // map author
		fread(&this->width, sizeof(Uint32), 1, fp); // map.getWidth()
		fread(&this->height, sizeof(Uint32), 1, fp); // map.getHeight()

		Sint32 *filetiles;
		filetiles = (Sint32 *)malloc(sizeof(Sint32)*this->width*this->height*MAPLAYERS);
		fread(filetiles, sizeof(Sint32), this->width*this->height*MAPLAYERS, fp);
		
		// Now put that ugly mess of tile data into a nice clean 3d vector array
		for (int z = 0; z<MAPLAYERS; z++) 
			for (int y = 0; y<this->height; y++) 
				for (int x = 0; x<this->width; x++) 
					this->tiles[x][y][z] = filetiles[z + y*MAPLAYERS + x*MAPLAYERS*this->height];

		free(filetiles);

		fread(&numentities, sizeof(Uint32), 1, fp); // number of entities on the map
		for (c = 0; c<numentities; c++) {
			fread(&sprite, sizeof(Sint32), 1, fp);
			if (entlist != NULL)
				entity = newEntity(sprite, 0, entlist);
			fread(&x, sizeof(Sint32), 1, fp);
			fread(&y, sizeof(Sint32), 1, fp);
			entity->x = x;
			entity->y = y;
			this->entities.push_back(entity);
		}
		fclose(fp);

		nummonsters = 0;
		minotaurlevel = 0;
		if (oldmapname == this->name)
			levelmusicplaying = FALSE;

		// create new lightmap
		if (lightmap != NULL)
			free(lightmap);
		lightmap = (int *)malloc(sizeof(Sint32)*this->width*this->height);
		if (this->name == "Hell") {
			for (c = 0; c<this->width*this->height; c++)
				lightmap[c] = 0;
		}
		else {
			for (c = 0; c<this->width*this->height; c++)
				lightmap[c] = 32;
		}

		// create a new vismap
		if (vismap != NULL)
			free(vismap);
		vismap = (bool *)calloc(this->width*this->height, sizeof(bool));

		// reset minimap
		for (x = 0; x<64; x++)
			for (y = 0; y<64; y++)
				minimap[y][x] = 0;

		// reset camera
		if (game) {
			camera.x = -32;
			camera.y = -32;
			camera.z = 0;
			camera.ang = 3 * PI / 2;
			camera.vang = 0;
		}
		else {
			camera.x = 2;
			camera.y = 2;
			camera.z = 0;
			camera.ang = 0;
			camera.vang = 0;
		}

		// shoparea
		if (shoparea)
			free(shoparea);
		shoparea = (bool *)malloc(sizeof(bool)*this->width*this->height);
		for (x = 0; x<this->width; x++)
			for (y = 0; y<this->height; y++)
				shoparea[y + x*this->height] = FALSE;

		for (c = 0; c<512; c++)
			keystatus[c] = 0;

		return numentities;
	}
}

void Map::addEntity(Entity * entity) {
	this->entities.push_back(entity);
}

Entity *Map::getEntityAt(int index) {
	if (this->entities.size >= index) {
		list<Entity *>::iterator it = this->entities.begin();
		advance(it, index+1);
		return *it;
	}
	else {
		return NULL;
	}
}

void Map::addDoor(Door * door) {
	this->doors.push_back(door);
}

Door::Door(Sint32 x, Sint32 y, Sint32 dir) {
	this->x = x;
	this->y = y;
	this->dir = dir;
}