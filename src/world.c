#include "dungeon.h"
#include "raymath.h"

World World_init(unsigned int width, unsigned int height, const char* filename)
{
        World world;
        unsigned int x;
        unsigned int y;

        world.width = width;
        world.height = height;
        world.tileset = LoadTexture(filename);
        world.sky_color = BLACK;
        world.draw_roof = true;
        world.gen = DUNGEON;
        world.tree_model = LoadModel("res/tree.obj");
        world.tree_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTexture("res/bark.png");

        world.tiles = calloc(height, sizeof(Tile*));
        if (!world.tiles) {
                fprintf(stderr, "Failed to allocate memory for world.\n");
                exit(1);
        }

        for (y = 0; y < height; y++) {
                world.tiles[y] = calloc(width, sizeof(Tile));
                if (!world.tiles[y]) {
                        fprintf(stderr, "Failed to allocate memory for world.\n");
                        exit(1);
                }
                for (x = 0; x < width; x++) {
                        world.tiles[y][x].type = WALL;
                        world.tiles[y][x].mat = BRICK;
                }
        }

        return world;
}


Vector2 World_generate(World* world)
{
        Vector2 pos;
        switch (world->gen) {
        case DUNGEON:
                pos = World_generate_dungeon(world);
                break;
        case OVERWORLD:
                pos = World_generate_overworld(world);
                break;
        }
        return pos;
}


Vector2 World_generate_overworld(World* world)
{
        unsigned int y;
        unsigned int x;
        world->sky_color = BLACK;
        world->draw_roof = false;

        for (y = 0; y < world->height; y++) {
                for (x = 0; x < world->width; x++) {
                        int odds = GetRandomValue(1, 100);
                        if (odds < 15) {
                                world->tiles[y][x].type = TREE;
                                world->tiles[y][x].mat = GRASS;
                        }
                        else {
                                world->tiles[y][x].type = FLOOR;
                                world->tiles[y][x].mat = GRASS;
                        }
                }
        }
        return (Vector2) {0,0};
}


Vector2 World_generate_dungeon(World* world)
{
        Vector2 pos;
        Vector2 start_pos;
        int height;
        int width;
        int n_rooms;
        int i;
        int exit_x;
        int exit_y;
        n_rooms = GetRandomValue(10, 20);
        Room* rooms = calloc(n_rooms, sizeof(Room));

        int rooms_counter = 0;

        for (i = 0; i < n_rooms; i++) {
                pos.y = GetRandomValue(20, world->height - 20);
                pos.x = GetRandomValue(20, world->width - 20);
                height = GetRandomValue(7, 10);
                width = GetRandomValue(8, 15);

                if (!Room_overlaps(rooms, rooms_counter, pos, height, width)) {
                        rooms[rooms_counter] = Room_init(pos, height, width);
                        printf("Added room at (%f %f)\n", rooms[rooms_counter].center.x, rooms[rooms_counter].center.y);
                        Room_add(rooms[rooms_counter], world);
                        rooms_counter++;
                }
        }

        for (i = 1; i < rooms_counter; i++) {
                Room_connect_centers(rooms[i-1].center, rooms[i].center, world);
        }

        start_pos.x = rooms[1].center.x;
        start_pos.y = rooms[1].center.y;
        exit_x = (int) rooms[rooms_counter - 1].center.x;
        exit_y = (int) rooms[rooms_counter - 1].center.y;
        world->tiles[exit_y][exit_x].type = EXIT;

        free(rooms);

        return start_pos;
}


Vector2 World_get_exit_pos(World* world)
{
        unsigned int y;
        unsigned int x;

        for (y = 0; y < world->height; y++) {
                for (x = 0; x < world->width; x++) {
                        if (world->tiles[y][x].type == EXIT)
                                return (Vector2) { x, y };
                }
        }

        return (Vector2) { -1, -1 };
}


void World_draw(World* world, Player* player)
{
        unsigned int y;
        unsigned int x;

        float view_distance = BLOCK_SIZE * 10;

        for (y = 0; y < world->height; y++) {
                for (x = 0; x < world->width; x++) {
                        Vector3 pos = { BLOCK_SIZE * x, 0.0f, BLOCK_SIZE * y };
                        int type = world->tiles[y][x].type;
                        int mat = world->tiles[y][x].mat;
                        Rectangle rect = {
                                (mat * 128) % world->tileset.width,
                                ((mat * 128) / world->tileset.width) * 128,
                                128,
                                128
                        };
                        float height;

                        switch (type){
                        case TREE:
                        case FLOOR:
                                height = 0.1f;
                                pos.y = -(BLOCK_SIZE / 2);
                                break;
                        case WALL:
                                height = BLOCK_SIZE;
                                break;
                        default:
                                height = BLOCK_SIZE;
                                break;
                        }
                        Vector2 player_pos = { player->cam.position.x, player->cam.position.z };
                        float dist = Vector2Distance(player_pos, (Vector2){ pos.x, pos.z });
                        if (dist <= view_distance) {
                                float dist_normalized = Clamp(dist / view_distance, 0.0f, 1.0f);
                                dist_normalized = sqrtf(dist_normalized);
                                Color tint = LerpColor(WHITE, world->sky_color, dist_normalized);

                                DrawCubeTextureRec(world->tileset, rect, pos, BLOCK_SIZE, height, BLOCK_SIZE, tint);
                                /* roof */
                                if (world->draw_roof) {
                                        pos.y = BLOCK_SIZE;
                                        DrawCubeTextureRec(world->tileset, rect, pos, BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, tint);
                                }
                                if (world->tiles[y][x].type == TREE) {
                                        Vector3 tree_pos = { pos.x, -1.0f, pos.z };
                                        DrawModel(world->tree_model, tree_pos, 1.0f, tint);
                                        /*DrawBillboard(player->cam, world->tree_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture, tree_pos, 2.0f, tint);*/
                                }
                        }
                }
        }
}
