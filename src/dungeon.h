#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define BLOCK_SIZE 2
#define TRANSITION_SPEED 0.05f
#define ROT_TRANSITION_SPEED 0.05f

typedef enum {
        STONE,
        BRICK,
        PATH,
        GRASS
} TileMaterial;

typedef enum {
        FLOOR,
        WALL,
        TREE
} TileType;

typedef enum {
        NORTH,
        EAST,
        SOUTH,
        WEST
} PlayerDirection;

typedef enum {
        DUNGEON,
        OVERWORLD
} WorldGenerationType;


typedef struct {
       TileType type;
       TileMaterial mat;
} Tile;

typedef struct {
        unsigned int width;
        unsigned int height;
        Texture2D tileset;
        Tile** tiles;
        Color sky_color;
        WorldGenerationType gen;
        Model tree_model;
        bool draw_roof;
} World;

typedef struct {
        Camera cam;
        Vector2 pos;
        Vector3 target_pos;
        Vector3 target_rot;
        float progress;
        float rot_progress;
        PlayerDirection dir;
} Player;

typedef struct {
        int height;
        int width;
        Vector2 pos;
        Vector2 center;
} Room;

/*
 * Functions
 */

/* world.c functions */
World World_init(unsigned int width, unsigned int height, const char* filename);
Vector2 World_generate(World* world);
Vector2 World_generate_overworld(World* world);
Vector2 World_generate_dungeon(World* world);
Vector2 World_get_exit_pos(World* world);
void World_draw(World* world, Player* player);

/* room.c functions */
Room Room_init(Vector2 pos, int height, int width);
void Room_add(Room room, World* world);
bool valid_pos(int x, int y, World* world);
void Room_connect_centers(Vector2 center_one, Vector2 center_two, World* world);
bool Room_overlaps(Room* rooms, int rooms_counter, Vector2 pos, int height, int width);

/* player.c functions */
Player Player_init(void);
void Player_process_movement(Player* player, float delta);

/* util.c */
Color LerpColor(Color near, Color far, float t);
void DrawCubeTexture(Texture2D texture, Vector3 position, float width, float height, float length, Color color);
void DrawCubeTextureRec(Texture2D texture, Rectangle source, Vector3 position, float width, float height, float length, Color color);
