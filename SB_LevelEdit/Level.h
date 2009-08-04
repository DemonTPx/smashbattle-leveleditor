#pragma once

#include <vector>

// Tile sizes, dimensions and count
#define TILE_W 32
#define TILE_H 32
#define TILE_COLS 20
#define TILE_ROWS 15
#define TILE_COUNT (TILE_COLS * TILE_ROWS)

#define LEVEL_ID 0x5342 // "SB"
#define LEVEL_VERSION 1

#define LEVEL_BLOCK_PSTART	0x1
#define LEVEL_BLOCK_PROP	0x2
//#define LEVEL_BLOCK_STORY	0x10
//#define LEVEL_BLOCK_NPC	0x11

struct LEVEL_POINT {
	short x;
	short y;
};

struct LEVEL_RECT {
	short x;
	short y;
	short w;
	short h;
};

struct LEVEL_HEADER {
	unsigned short id;
	unsigned short version;
	char name[20];
	char author[20];
	bool multiplayer;
	unsigned short max_players;
	unsigned int background_color;
	char filename_tiles[30];
	char filename_background[30];
	char filename_props[30];
};

struct LEVEL_TILE {
	unsigned short tile;
	unsigned short hp;
	bool indestructible;
	bool bouncing;
	bool show_in_preview;
};

struct LEVEL_PLAYERSTART {
	unsigned short player;
	short x;
	short y;
	bool facing_right;
};

struct LEVEL_PROP {
	LEVEL_RECT src;
	LEVEL_POINT dst;
};

class Level
{
public:
	Level();
	~Level();

	void load(const char * filename);
	void save(const char * filename);

	bool loaded;
	char * filename;

	LEVEL_HEADER header;
	LEVEL_TILE tile[TILE_COUNT];

	LEVEL_PLAYERSTART playerstart[4];

	std::vector<LEVEL_PROP *> * props;
};
