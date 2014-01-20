#include <cstring>
#include "zlib.h"
#include "Level.h"

Level::Level() {
	loaded = false;

	props = new std::vector<LEVEL_PROP*>(0);
	powerups = new std::vector<LEVEL_POWERUP*>(0);
	powerup_dispensers = new std::vector<LEVEL_POWERUP_DISPENSER*>(0);
	npcs = new std::vector<LEVEL_NPC*>(0);
	npc_dispensers = new std::vector<LEVEL_NPC_DISPENSER*>(0);

	for(int i = 0; i < 4; i++) {
		memset(&playerstart[i], 0, sizeof(LEVEL_PLAYERSTART));
		playerstart[i].player = 0xffff;
	}

	memset(&mission, 0, sizeof(LEVEL_MISSION));
	mission.type = LM_TYPE_NONE;
}

Level::~Level() {
	for(unsigned int i = 0; i < props->size(); i++) {
		delete props->at(i);
	}
	delete props;
	
	for(unsigned int i = 0; i < powerups->size(); i++) {
		delete powerups->at(i);
	}
	delete powerups;
	
	for(unsigned int i = 0; i < powerup_dispensers->size(); i++) {
		delete powerup_dispensers->at(i);
	}
	delete powerup_dispensers;

	for(unsigned int i = 0; i < npcs->size(); i++) {
		delete npcs->at(i);
	}
	delete npcs;

	for(unsigned int i = 0; i < npc_dispensers->size(); i++) {
		delete npc_dispensers->at(i);
	}
	delete npc_dispensers;
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
	LEVEL_POWERUP * powerup;
	LEVEL_POWERUP_DISPENSER * powerup_dispenser;
	LEVEL_NPC * npc;
	LEVEL_NPC_DISPENSER * npc_dispenser;

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
		strncpy(meta.filename_music, "victory.ogg", 30);
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
				if(pstart.player == 0xffff) break;
				memcpy(&playerstart[pstart.player], &pstart, sizeof(LEVEL_PLAYERSTART));
				break;
			// Props
			case LEVEL_BLOCK_PROP:
				prop = new LEVEL_PROP();
				gzread(file, prop, sizeof(LEVEL_PROP));
				props->push_back(prop);
				break;
			// Mission
			case LEVEL_BLOCK_MISSION:
				gzread(file, &mission, sizeof(LEVEL_MISSION));
				break;
			// Powerups
			case LEVEL_BLOCK_POWERUP:
				powerup = new LEVEL_POWERUP();
				gzread(file, powerup, sizeof(LEVEL_POWERUP));
				powerups->push_back(powerup);
				break;
			// Powerup dispensers
			case LEVEL_BLOCK_POWERUP_DISPENSER:
				powerup_dispenser = new LEVEL_POWERUP_DISPENSER();
				gzread(file, powerup_dispenser, sizeof(LEVEL_POWERUP_DISPENSER));
				powerup_dispensers->push_back(powerup_dispenser);
				break;
			// NPC
			case LEVEL_BLOCK_NPC:
				npc = new LEVEL_NPC();
				gzread(file, npc, sizeof(LEVEL_NPC));
				npcs->push_back(npc);
				break;
			// NPC dispensers
			case LEVEL_BLOCK_NPC_DISPENSER:
				npc_dispenser = new LEVEL_NPC_DISPENSER();
				gzread(file, npc_dispenser, sizeof(LEVEL_NPC_DISPENSER));
				npc_dispensers->push_back(npc_dispenser);
				break;
		}
	}

	gzclose(file);

	loaded = true;
}

void Level::save(const char * filename) {
	gzFile file;
	
	LEVEL_PROP * prop;
	LEVEL_POWERUP * powerup;
	LEVEL_POWERUP_DISPENSER * powerup_dispenser;
	LEVEL_NPC * npc;
	LEVEL_NPC_DISPENSER * npc_dispenser;

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

	// Mission
	block_id = LEVEL_BLOCK_MISSION;
	if(mission.type != LM_TYPE_NONE) {
		gzwrite(file, (char*)&block_id, sizeof(unsigned short));
		gzwrite(file, (char*)&mission, sizeof(LEVEL_MISSION));
	}

	// Powerups
	block_id = LEVEL_BLOCK_POWERUP;
	for(unsigned int i = 0; i < powerups->size(); i++) {
		powerup = powerups->at(i);
		gzwrite(file, (char*)&block_id, sizeof(unsigned short));
		gzwrite(file, (char*)powerup, sizeof(LEVEL_POWERUP));
	}

	// Powerup dispensers
	block_id = LEVEL_BLOCK_POWERUP_DISPENSER;
	for(unsigned int i = 0; i < powerup_dispensers->size(); i++) {
		powerup_dispenser = powerup_dispensers->at(i);
		gzwrite(file, (char*)&block_id, sizeof(unsigned short));
		gzwrite(file, (char*)powerup_dispenser, sizeof(LEVEL_POWERUP_DISPENSER));
	}

	// NPCs
	block_id = LEVEL_BLOCK_NPC;
	for(unsigned int i = 0; i < npcs->size(); i++) {
		npc = npcs->at(i);
		gzwrite(file, (char*)&block_id, sizeof(unsigned short));
		gzwrite(file, (char*)npc, sizeof(LEVEL_NPC));
	}

	// NPC dispensers
	block_id = LEVEL_BLOCK_NPC_DISPENSER;
	for(unsigned int i = 0; i < npc_dispensers->size(); i++) {
		npc_dispenser = npc_dispensers->at(i);
		gzwrite(file, (char*)&block_id, sizeof(unsigned short));
		gzwrite(file, (char*)npc_dispenser, sizeof(LEVEL_NPC_DISPENSER));
	}

	gzclose(file);
}
