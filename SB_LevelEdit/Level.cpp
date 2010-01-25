#include "zlib.h"
#include "Level.h"

Level::Level() {
	props = new std::vector<LEVEL_PROP*>(0);
	loaded = false;

	for(int i = 0; i < 4; i++) {
		memset(&playerstart[i], 0, sizeof(LEVEL_PLAYERSTART));
		playerstart[i].player = 0xffff;
	}
}

Level::~Level() {
	for(unsigned int i = 0; i < props->size(); i++) {
		delete props->at(i);
	}
	delete props;
}

void Level::create(const char * filename, LEVEL_META &m) {
	memcpy(&meta, &m, sizeof(LEVEL_META));

	LEVEL_TILE t;
	int i;

	t.tile = -1;
	t.hp = 50;
	t.indestructible = false;
	t.show_in_preview = false;
	t.bouncing = true;

	for(i = 0; i < TILE_COUNT; i++) {
		memcpy(&tile[i], (char*)&t, sizeof(LEVEL_TILE));
	}

	save(filename);

	loaded = true;
}

void Level::load(const char * filename) {
	gzFile file;
	LEVEL_PLAYERSTART pstart;
	LEVEL_PROP * prop;
	unsigned short block_id;

	file = gzopen(filename, "rb");

	// Header
	gzread(file, &header, sizeof(LEVEL_HEADER));
	
	if(header.id != LEVEL_ID) // Invalid file
		return;
	if(header.version != 1 && header.version != LEVEL_VERSION) // Invalid version
		return;

	// Read meta data
	if(header.version == 1) {
		// Convert version 1 meta data to version 2 meta data
		LEVEL_META_1 meta1;
		gzread(file, &meta1, sizeof(LEVEL_META_1));
		memset(&meta, 0, sizeof(LEVEL_META));
		memcpy(&meta, &meta1, sizeof(LEVEL_META_1));
		strncpy(meta.filename_music, "battle.ogg", 30);
	} else {
		gzread(file, &meta, sizeof(LEVEL_META));
	}

	// Tiles
	gzread(file, &tile, sizeof(tile));

	while(!gzeof(file)) {
		gzread(file, &block_id, sizeof(block_id));
		switch(block_id) {
			// Player starts
			case LEVEL_BLOCK_PSTART:
				gzread(file, &pstart, sizeof(LEVEL_PLAYERSTART));
				memcpy(&playerstart[pstart.player], &pstart, sizeof(LEVEL_PLAYERSTART));
				break;
			// Props
			case LEVEL_BLOCK_PROP:
				prop = new LEVEL_PROP();
				gzread(file, prop, sizeof(LEVEL_PROP));
				props->push_back(prop);
				break;
		}
	}

	gzclose(file);

	loaded = true;
}

void Level::save(const char * filename) {
	gzFile file;
	LEVEL_PROP * prop;
	unsigned short block_id;

	file = gzopen(filename, "wb");
	
	// Header
	header.id = LEVEL_ID;
	header.version = LEVEL_VERSION;
	
	gzwrite(file, (char *)&header, sizeof(LEVEL_HEADER));

	gzwrite(file, (char *)&meta, sizeof(LEVEL_META));

	// Tiles
	gzwrite(file, &tile, sizeof(tile));

	// Player starts
	block_id = LEVEL_BLOCK_PSTART;
	for(unsigned int i = 0; i < 4; i++) {
		if(playerstart->player >= 0 && playerstart->player < 4) {
			gzwrite(file, (char*)&block_id, sizeof(unsigned short));
			gzwrite(file, (char*)&playerstart[i], sizeof(LEVEL_PLAYERSTART));
		}
	}

	// Props
	block_id = LEVEL_BLOCK_PROP;
	for(unsigned int i = 0; i < props->size(); i++) {
		prop = props->at(i);
		gzwrite(file, (char*)&block_id, sizeof(unsigned short));
		gzwrite(file, (char*)prop, sizeof(LEVEL_PROP));
	}

	gzclose(file);
}
