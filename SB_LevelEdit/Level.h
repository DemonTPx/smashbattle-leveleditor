#pragma once

// Tile sizes, dimensions and count
#define TILE_W 32
#define TILE_H 32
#define TILE_COLS 20
#define TILE_ROWS 15
#define TILE_COUNT (TILE_COLS * TILE_ROWS)

#define LEVEL_ID 'SB'
#define LEVEL_VERSION 1

struct LEVEL_TILE {
	unsigned short tile;
	unsigned short hp;
	bool indestructible;
	bool bouncing;
	bool show_in_preview;
	unsigned short player_start;
};

struct LEVEL_HEADER {
	unsigned short id;
	unsigned short version;
	char name[20];
	char author[20];
	unsigned short max_players;
	char filename_tiles[30];
	char filename_background[30];
};

class Level
{
public:
	void load(const char * filename);

	LEVEL_HEADER header;
	LEVEL_TILE tile[TILE_COUNT];
};
