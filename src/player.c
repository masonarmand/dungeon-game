#include "dungeon.h"

Player Player_init(void)
{
        Player player = { 0 };

        player.cam.position = (Vector3){ 1.0f, 1.0f, 1.0f }; // Camera position
        player.cam.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
        player.cam.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
        player.cam.fovy = 45.0f;                                // Camera field-of-view Y
        player.cam.projection = CAMERA_PERSPECTIVE;

        return player;
}

void Player_process_movement(Player* player, float delta)
{
        Vector3 movement = {0,0,0};
        Vector3 rotation = {0,0,0};

        float player_height = BLOCK_SIZE / 4;

        int d = 0;
        d = (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) ? 1 : 0;
        if (d == 0)
                d = (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) ? -1 : 0;

        if (d != 0) {

                player->target_pos = (Vector3){
                        player->target_pos.x + d * (player->dir == EAST ? BLOCK_SIZE : player->dir == WEST ? -BLOCK_SIZE : 0),
                        player_height,
                        player->target_pos.z + d * (player->dir == NORTH ? -BLOCK_SIZE : player->dir == SOUTH ? BLOCK_SIZE : 0)
                };

                player->progress = 0.0f;

                switch (player->dir) {
                case NORTH:
                        player->pos.y -= d;
                        break;
                case EAST:
                        player->pos.x += d;
                        break;
                case SOUTH:
                        player->pos.y += d;
                        break;
                case WEST:
                        player->pos.x -= d;
                        break;
                }
        }

        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) {
                player->dir = (player->dir - 1) % 4;
                player->rot_progress = 0.0f;
        }
        if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) {
                player->dir = (player->dir + 1) % 4;
                player->rot_progress = 0.0f;
        }

        player->target_rot = player->cam.position;

        switch (player->dir) {
        case NORTH:
                player->target_rot.z = player->cam.position.z - BLOCK_SIZE;
                break;
        case EAST:
                player->target_rot.x = player->cam.position.x + BLOCK_SIZE;
                break;
        case SOUTH:
                player->target_rot.z = player->cam.position.z + BLOCK_SIZE;
                break;
        case WEST:
                player->target_rot.x = player->cam.position.x - BLOCK_SIZE;
                break;
        }

        if (player->progress < 1.0f) {
                player->cam.position.x = player->cam.position.x + (player->target_pos.x - player->cam.position.x) * player->progress;
                player->cam.position.z = player->cam.position.z + (player->target_pos.z - player->cam.position.z) * player->progress;
                player->progress += TRANSITION_SPEED * delta;
        }
        else {
                player->cam.position = player->target_pos;
        }


        if (player->rot_progress < 1.0f) {
                player->cam.target.x = player->cam.target.x + (player->target_rot.x - player->cam.target.x) * player->rot_progress;
                player->cam.target.z = player->cam.target.z + (player->target_rot.z - player->cam.target.z) * player->rot_progress;
                player->rot_progress += ROT_TRANSITION_SPEED * delta;
        }
        else {
                player->cam.target = player->target_rot;
        }
}
