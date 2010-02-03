#pragma once

#include <vector>

// Tile sizes, dimensions and count
#define TILE_W 32
#define TILE_H 32
#define TILE_COLS 20
#define TILE_ROWS 15
#define TILE_COUNT (TILE_COLS * TILE_ROWS)

#define LEVEL_ID 0x5342 // "SB"
#define LEVEL_VERSION 2

#define LEVEL_META LEVEL_META_2

#define LEVEL_BLOCK_PSTART				0x01
#define LEVEL_BLOCK_PROP				0x02
#define LEVEL_BLOCK_MISSION				0x10
#define LEVEL_BLOCK_POWERUP				0x20
#define LEVEL_BLOCK_POWERUP_DISPENSER	0x21
#define LEVEL_BLOCK_NPC					0x30
#define LEVEL_BLOCK_NPC_DISPENSER		0x31

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
};

struct LEVEL_META_1 {
	char name[20];
	char author[20];
	bool multiplayer;
	unsigned short max_players;
	unsigned int background_color;
	char filename_tiles[30];
	char filename_background[30];
	char filename_props[30];
};

struct LEVEL_META_2 {
	char name[20];
	char author[20];
	bool multiplayer;
	unsigned short max_players;
	unsigned int background_color;
	char filename_tiles[30];
	char filename_background[30];
	char filename_props[30];
	char filename_music[30];
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

enum {
	LM_TYPE_NONE = 0,
	LM_TYPE_KILL_ALL
};

struct LEVEL_MISSION {
	int character;
	int type;
	int bullets;
	int doubledamagebullets;
	int instantkillbullets;
	int bombs;
	int kill_all_time_gold;
	int kill_all_time_silver;
};

enum {
	L_PU_HEALTH,
	L_PU_AMMO,
	L_PU_DOUBLEDAMAGE,
	L_PU_INSTANTKILL,
	L_PU_BOMB,
	L_PU_AIRSTRIKE,
	L_PU_LASERBEAM
};

struct LEVEL_POWERUP {
	int type;
	LEVEL_POINT position;
};

struct LEVEL_POWERUP_DISPENSER {
	bool global;
	LEVEL_POINT position; // (ignored on global)
	int rate; // chance per frame; 60 is ~1 per second
	int max; // max powerups on screen (global only)
	int rate_health;
	int rate_bullet;
	int rate_doubledamage;
	int rate_instantkill;
	int rate_bomb;
	int rate_airstrike;
	int rate_laserbeam;
};

enum {
	L_NPC_CHICK,
	L_NPC_CANNON,
	L_NPC_GATLING
};

struct LEVEL_NPC {
	int type;
	LEVEL_POINT position;
	int move_direction;
};

struct LEVEL_NPC_DISPENSER {
	int type; // NPC type
	LEVEL_POINT position;
	int rate; // chance per frame; 60 is ~1 per second
	int max; // maximum NPCs to dispence
};

class Level
{
public:
	Level();
	~Level();

	void create(const char * filename, LEVEL_META &m);
	void load(const char * filename);
	void save(const char * filename);

	bool loaded;

	LEVEL_HEADER header;
	LEVEL_META meta;
	LEVEL_TILE tile[TILE_COUNT];

	LEVEL_PLAYERSTART playerstart[4];

	LEVEL_MISSION mission;

	std::vector<LEVEL_PROP *> * props;

	std::vector<LEVEL_POWERUP *> * powerups;
	std::vector<LEVEL_POWERUP_DISPENSER *> * powerup_dispensers;

	std::vector<LEVEL_NPC *> * npcs;
	std::vector<LEVEL_NPC_DISPENSER *> * npc_dispensers;
};
