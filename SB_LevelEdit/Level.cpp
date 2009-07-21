#include "zlib.h"
#include "Level.h"

void Level::load(const char * filename) {
	gzFile file;

	file = gzopen(filename, "rb");
	gzread(file, &header, sizeof(LEVEL_HEADER));
	
	if(header.id != LEVEL_ID) // Invalid file
		return;
	if(header.version != LEVEL_VERSION) // Invalid version
		return;

	gzread(file, &tile, sizeof(tile));
	gzclose(file);
}
