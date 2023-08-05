/* file: main.c
 * ------------
 * Dungeon crawler game/engine created in Raylib
 *
 * Author: Mason Armand
 * Date Created: June 19th, 2023
 * Last Modified: August 8th, 2023
 */
#include "dungeon.h"
#include "rlgl.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int main(void)
{
        World world;
        Player player = Player_init();
        bool cursor_enabled = false;
        float delta = 0.0f;
        float last_frame = 0.0f;

        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "dungeon");
        /*SetTargetFPS(60);*/

        world = World_init(100, 100, "res/blocks.png");
        world.gen = DUNGEON;
        player.pos = World_generate(&world);
        player.cam.position = (Vector3) {player.pos.x * BLOCK_SIZE, BLOCK_SIZE / 4, player.pos.y * BLOCK_SIZE};
        player.target_pos = player.cam.position;
        player.target_rot = player.cam.position;
        printf("%f %f\n", player.pos.x, player.pos.y);

        Texture2D compass = LoadTexture("res/hands/compass.png");

        while (!WindowShouldClose()) {
                /* update */
                delta = GetTime() - last_frame;
                last_frame = GetTime();

                Player_process_movement(&player, delta);

                /* render */
                BeginDrawing();
                        ClearBackground(world.sky_color);
                        BeginMode3D(player.cam);
                        rlEnableBackfaceCulling();
                                World_draw(&world, &player);
                        rlDisableBackfaceCulling();
                        EndMode3D();
                        DrawFPS(10, 10);
                        DrawTexture(compass, 418, 277, GRAY);
                EndDrawing();
        }
        CloseWindow();
        return 0;
}
