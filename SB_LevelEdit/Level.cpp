#include "zlib.h"
#include "Level.h"

Level::Level() {
	props = new std::vector<LEVEL_PROP*>(0);
	loaded = false;

	for(int i = 0; i < 4; i++) {
		playerstart[i].player = 0xffff;
	}
}

Level::~Level() {
	for(unsigned int i = 0; i < props->size(); i++) {
		delete props->at(i);
	}
	delete props;
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
	if(header.version != LEVEL_VERSION) // Invalid version
		return;

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
