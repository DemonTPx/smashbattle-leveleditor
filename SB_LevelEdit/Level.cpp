#include "zlib.h"
#include "Level.h"

Level::Level() {
	props = new std::vector<LEVEL_PROP*>(0);
	loaded = false;
}

Level::~Level() {
	props->clear();
	delete props;
}

void Level::load(const char * filename) {
	gzFile file;
	LEVEL_PLAYERSTART pstart;
	LEVEL_PROP * prop;
	unsigned short block_id;

	file = gzopen(filename, "rb");
	gzread(file, &header, sizeof(LEVEL_HEADER));
	
	if(header.id != LEVEL_ID) // Invalid file
		return;
	if(header.version != LEVEL_VERSION) // Invalid version
		return;

	gzread(file, &tile, sizeof(tile));

	while(!gzeof(file)) {
		gzread(file, &block_id, sizeof(block_id));
		switch(block_id) {
			case LEVEL_BLOCK_PSTART:
				gzread(file, &pstart, sizeof(LEVEL_PLAYERSTART));
				memcpy(&playerstart[pstart.player], &pstart, sizeof(LEVEL_PLAYERSTART));
				break;
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
