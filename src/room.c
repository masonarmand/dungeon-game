#include "dungeon.h"

Room Room_init(Vector2 pos, int height, int width)
{
        Room new;
        new.pos = pos;
        new.height = height;
        new.width = width;
        new.center.y = pos.y + (int)(height / 2);
        new.center.x = pos.x + (int)(width / 2);

        return new;
}


void Room_add(Room room, World* world)
{
        unsigned int max_h = room.pos.y + room.height;
        unsigned int max_w = room.pos.x + room.width;
        unsigned int y;
        unsigned int x;

        for (y = room.pos.y; y <= max_h; y++) {
                for (x = room.pos.x; x <= max_w; x++) {
                        if (x >= world->width || y >= world->height)
                                continue;
                        world->tiles[y][x].type = FLOOR;
                        world->tiles[y][x].mat = STONE;
                }
        }
}


bool valid_pos(int x, int y, World* world)
{
        return (x < world->width && x > 0 && y < world->height && y > 0 && world->tiles[y][x].type != FLOOR);
}


void Room_connect_centers(Vector2 center_one, Vector2 center_two, World* world)
{
        int x = (int)center_one.x;
        int y = (int)center_one.y;
        int x1 = (int)center_two.x;
        int y1 = (int)center_two.y;
        int dir;

        while (true) {
                if (abs((x - 1) - x1) < abs(x - x1)) {
                        x--;
                }
                else if (abs((x + 1) - x1) < abs(x - x1)) {
                        x++;
                }
                else if (abs((y + 1) - y1) < abs(y - y1)) {
                        y++;
                }
                else if (abs((y - 1) - y1) < abs(y - y1)) {
                        y--;
                }
                else break;

                if (valid_pos(x, y, world)) {
                        world->tiles[y][x].type = FLOOR;
                        world->tiles[y][x].mat = PATH;
                }
        }
}


bool Room_overlaps(Room* rooms, int rooms_counter, Vector2 pos, int height, int width)
{
        int x = (int) pos.x;
        int y = (int) pos.y;
        int i;

        for (i = 0; i < rooms_counter; i++) {
                if ((x >= rooms[i].pos.x + rooms[i].width || rooms[i].pos.x >= x + width)
                || (y + height <= rooms[i].pos.y || rooms[i].pos.y + rooms[i].height <= y)) {
                        continue;
                }
                return true;
        }
        return false;
}
