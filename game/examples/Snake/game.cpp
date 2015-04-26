#include "game.h"
#include "game_settings.h"
#include "../bitmaps/font5x7.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

extern Font_t font5x7font;

typedef struct
{
    char name[4];
    int32_t score;
} highscore_t;

highscore_t highscores_list[MAX_HIGHSCORES];

void init_highscores()
{
    int i;
    for(i = 0; i < MAX_HIGHSCORES; i++)
    {
        memcpy(highscores_list[i].name, "---", 4);
        highscores_list[i].score = 0;
    }
}

Point_t food;

Rect_t game_box;

uint32_t cell_size;

/********************************
 *      SNAKE PARAMETERS        *
 ********************************
 */
uint32_t snake_len;
const uint32_t snake_radius = 4;

typedef enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT
} snake_dir_t;

snake_dir_t snake_dir;

Point_t snake_points[MAX_SNAKE_LEN];

/********************************
 *      GAME STATE PARAMETERS   *
 ********************************
 */
typedef enum
{
    GS_MENU,
    GS_HIGHSCORES,
    GS_PLAYING,
    GS_GAME_OVER
} game_state_t;

game_state_t game_state;

#define GS_TRANS_DELAY_LOAD (50)

uint32_t gs_trans_delay_counter;
uint32_t delay_load_counter;
uint32_t food_counter;
int32_t game_score;

bool change_game_state(game_state_t newstate)
{
    if(!gs_trans_delay_counter)
    {
        game_state = newstate;
        gs_trans_delay_counter = GS_TRANS_DELAY_LOAD;
        return true;
    }
    return false;
}

/** \brief Aligns a point with the game grid.
 * Aligns a point with the game grid, defined as
 * a square, edge-aligned grid beginning at 0,0
 * with a cell edge length equal to 2*snake_radius
 */
Point_t grid_align(const Point_t& pt)
{
    Point_t ret = pt;
    if(ret.x <= cell_size)
        ret.x = cell_size*2;
    if(ret.x > game_box.w)
        ret.x = game_box.w;
    if(ret.y <= cell_size)
        ret.y = cell_size*2;
    if(ret.y > game_box.h)
        ret.y = game_box.h;
    ret.x -= ret.x % cell_size;
    ret.y -= ret.y % cell_size;
    return ret;
}

/** \brief Updates the snake.
 * Updates the position of the snake according to
 * snake_dir, and then moves all of the trailing
 * snake parts after their leaders
 */
void update_snake()
{
    uint32_t i;
    for(i = snake_len - 1; i > 0; i--)
    {
        snake_points[i] = snake_points[i-1];
    }
    switch (snake_dir)
    {
    case UP:
        snake_points[0].y -= cell_size;
        break;
    case DOWN:
        snake_points[0].y += cell_size;
        break;
    case LEFT:
        snake_points[0].x -= cell_size;
        break;
    case RIGHT:
        snake_points[0].x += cell_size;
        break;
    }
}

/** \brief Find a new random position for the food particle.
 *
 *
 */
void move_food()
{
    food = grid_align(Point_t(rand() % screen.getWidth(),
                              rand() % screen.getHeight()));
}

void check_game()
{
    uint32_t i;
    for(i = snake_len - 1; i > 0; i--)
    {
        if(snake_points[i] == snake_points[0])
        {
            game_state = GS_GAME_OVER;
            return;
        }
    }

    if(snake_points[0].x <= 0 || snake_points[0].y <= 0 ||
       snake_points[0].x > game_box.w || snake_points[0].y > game_box.h)
    {
        game_state = GS_GAME_OVER;
        return;
    }

    if(snake_points[0] == food)
    {
        int i;
        for(i = 0; i < FOOD_BONUS_LEN; i++)
        {
            snake_points[snake_len + i] = snake_points[snake_len - 1];
        }

        snake_len += FOOD_BONUS_LEN;

        game_score += SCORE_PER_FOOD;

        food_counter++;
        if(food_counter == FOOD_TAKEN_TO_SPEEDUP)
        {
            food_counter = 0;
            if(delay_load_counter > 1)
                delay_load_counter--;
        }
        move_food();
    }

}

int8_t wave_table[] = {-1,-2,-3,-2,-1,0,1,2,3,2,1,0};
int16_t wave_table_index = 0;

void draw_snake()
{
    uint32_t i;
    wave_table_index += 1;
    if(wave_table_index == 120)
        wave_table_index = 0;

    for(i = 0; i < snake_len; i++)
    {
        Point_t snakept = snake_points[i];
        snakept += game_box.upperLeft();
        snakept -= Point_t(snake_radius + 1, snake_radius + 1);

#if DANK_MODE
        Rect_t destrect = Rect_t(
                              -5 + wave_table[(i + wave_table_index / 5 + 6) % 12],
                              -5 + wave_table[(i + wave_table_index / 10) % 12],
                              10,
                              10
                          ) + snakept;
        screen.box(&destrect, WHITE, WHITE);
#else
        screen.circle(&snakept, snake_radius, WHITE, NONE);
#endif
    }
}

void draw_food()
{
    Point_t foodpt = food;
    foodpt += game_box.upperLeft();
    foodpt -= Point_t(snake_radius + 1, snake_radius + 1);
    screen.circle(&foodpt, snake_radius, WHITE, NONE);
}

void draw_game_window()
{
    screen.box(&game_box, WHITE, NONE);
}

void draw_score()
{
    static Point_t score_anchor(screen.getWidth() - 5, 5);

    static char numbuf[64];

    snprintf(numbuf, 64, "Score: %d", game_score);

    screen.textOption(font5x7font,
                      score_anchor,
                      numbuf,
                      FAP_TOP_RIGHT,
                      FJ_RIGHT,
                      MODE_OVERWRITE_INVERT);
}

void add_highscore(const char* name, uint32_t score)
{
    static uint32_t highscore_index = 0;

    memcpy(highscores_list[highscore_index].name, name, 4);
    highscores_list[highscore_index].score = score;

    highscore_index++;
    if(highscore_index == MAX_HIGHSCORES)
        highscore_index = 0;
}

void init_snake_game()
{
    game_score = 0;

    snake_len = INITIAL_SNAKE_LEN;

    delay_load_counter = DEFAULT_SNAKE_SPEED_DELAY;

    uint32_t i;
    for(i = 0; i < snake_len; i++)
    {
        snake_points[i] = grid_align(Point_t(screen.getWidth()/2, screen.getHeight()/2));
    }

    move_food();

    snake_dir = UP;
}

void Game_init()
{
    // Enable double-buffering
    TVOut_EnableDoubleBuffer(true);

    // Enable buffer clearing on double buffer swap
    TVOut_ClearBufferOnSwap(true);

    /* TODO:    Your code goes here
     *
     *          Any code that needs to run once
     *          to set up your game goes here
     */

     game_box = Rect_t(0,20, screen.getWidth()-10, screen.getHeight()-25);
     cell_size = 10;
     game_box.w -= game_box.w % cell_size;
     game_box.h -= game_box.h % cell_size;
     game_box.x = (screen.getWidth() - game_box.w)/2;


    init_highscores();

    game_state = GS_MENU;
}

Remote_buttons_t rbs;

void menu()
{
#define MAX_MENU_ITEM 1
    static uint8_t selected_menu_item = 0;

    if(rbs.buttons.a)
    {
        switch(selected_menu_item)
        {
        case 0:
            if(change_game_state(GS_PLAYING))
                init_snake_game();
            break;
        case 1:
            change_game_state(GS_HIGHSCORES);
            break;
        }
    }
    if(rbs.buttons.down)
    {
        if(selected_menu_item < MAX_MENU_ITEM)
        {
            selected_menu_item++;
        }
    }
    if(rbs.buttons.up)
    {
        if(selected_menu_item)
        {
            selected_menu_item--;
        }
    }

    static Point_t menu_anchor(screen.getWidth()/2, screen.getHeight()/5 * 3);
    static Point_t text1_anchor = menu_anchor + Point_t(0,0);
    static Point_t text2_anchor = menu_anchor + Point_t(0,30);

    static Point_t title_anchor(screen.getWidth()/2, screen.getHeight()/5);
    static PWMTimer title_flash_timer(50, 25);

    Rect_t destrect(0,0,100,20);
    if(selected_menu_item == 0)
        destrect.centerOn(text1_anchor);
    if(selected_menu_item == 1)
        destrect.centerOn(text2_anchor);

    if(title_flash_timer())
        screen.textOption(font5x7font,
                          title_anchor,
                          "Snake!",
                          FAP_CENTER,
                          FJ_CENTER,
                          MODE_OVERWRITE_INVERT);

    screen.box(&destrect, WHITE, NONE);
    screen.textOption(font5x7font,
                      text1_anchor,
                      "PLAY GAME",
                      FAP_CENTER,
                      FJ_CENTER,
                      MODE_OVERWRITE_INVERT);
    screen.textOption(font5x7font,
                      text2_anchor,
                      "HIGHSCORES",
                      FAP_CENTER,
                      FJ_CENTER,
                      MODE_OVERWRITE_INVERT);
}

void highscores()
{
    if(rbs.buttons.b || rbs.buttons.a)
    {
        change_game_state(GS_MENU);
    }

    static Point_t hsname_anchor(screen.getWidth()/4, 40);
    static Point_t hsscore_anchor(screen.getWidth()/4 * 3, 40);
    static Point_t hstitle_anchor(screen.getWidth()/2, 20);
    static int16_t score_vstride = 10;

    static char numbuf[20];

    screen.textOption(font5x7font,
                      hstitle_anchor,
                      "HIGHSCORES:",
                      FAP_TOP_CENTER,
                      FJ_CENTER,
                      MODE_OVERWRITE_INVERT);

    int i;
    for(i = 0; i < MAX_HIGHSCORES; i++)
    {
        screen.textOption(font5x7font,
                          hsname_anchor + Point_t(0, score_vstride * i),
                          highscores_list[i].name,
                          FAP_TOP_LEFT,
                          FJ_LEFT,
                          MODE_OVERWRITE_INVERT);
        snprintf(numbuf, 20, "%d", highscores_list[i].score);
        screen.textOption(font5x7font,
                          hsscore_anchor + Point_t(0, score_vstride * i),
                          numbuf,
                          FAP_TOP_LEFT,
                          FJ_LEFT,
                          MODE_OVERWRITE_INVERT);
    }
}

void game_over()
{
    static PWMTimer titleflash_timer(50, 25);
    static PWMTimer selectchar_flash_timer(10, 5);

    static char namebuf[4] = "AAA";

    if(rbs.buttons.b || rbs.buttons.a)
    {
        change_game_state(GS_HIGHSCORES);

        add_highscore(namebuf, game_score);

    }

    static Point_t hsname_anchor(screen.getWidth()/2, screen.getHeight()/5 * 3);
    static Point_t hslabel_anchor(screen.getWidth()/2, screen.getHeight()/2 - 20);
    static Point_t gotitle_anchor(screen.getWidth()/2, 20);

    static char scorebuf[64];

    snprintf(scorebuf, 64, "Score: %d\n\nEnter your name:", game_score);

#define MAX_CHAR (2)
    static uint8_t selected_char = 0;

    Rect_t select_char_box(0,0,8,10);
    select_char_box.centerOn(hsname_anchor + Point_t(-6 + 6*selected_char, 0));

    if(rbs.buttons.p_left)
    {
        if(selected_char)
            selected_char--;
    }

    if(rbs.buttons.p_right)
    {
        if(selected_char < MAX_CHAR)
            selected_char++;
    }

    if(rbs.buttons.p_up)
    {
        if(namebuf[selected_char] < 'Z')
            namebuf[selected_char]++;
    }

    if(rbs.buttons.p_down)
    {
        if('A' < namebuf[selected_char])
            namebuf[selected_char]--;
    }

    if(titleflash_timer())
        screen.textOption(font5x7font,
                          gotitle_anchor,
                          "GAME OVER!",
                          FAP_TOP_CENTER,
                          FJ_CENTER,
                          MODE_OVERWRITE_INVERT);

    screen.textOption(font5x7font,
                      hslabel_anchor,
                      scorebuf,
                      FAP_TOP_CENTER,
                      FJ_CENTER,
                      MODE_OVERWRITE_INVERT);

    screen.textOption(font5x7font,
                      hsname_anchor,
                      namebuf,
                      FAP_CENTER,
                      FJ_CENTER,
                      MODE_OVERWRITE_INVERT);

    if(selectchar_flash_timer())
        screen.box(&select_char_box, INVERT, INVERT);



}

void playing()
{
    if(rbs.buttons.left && snake_dir != RIGHT)
    {
        snake_dir = LEFT;
    }
    if(rbs.buttons.right && snake_dir != LEFT)
    {
        snake_dir = RIGHT;
    }
    if(rbs.buttons.up && snake_dir != DOWN)
    {
        snake_dir = UP;
    }
    if(rbs.buttons.down && snake_dir != UP)
    {
        snake_dir = DOWN;
    }

    if(rbs.buttons.b)
    {
        change_game_state(GS_MENU);
    }

    static uint32_t delay_counter = delay_load_counter;

    if(!(delay_counter--))
    {
        update_snake();
        check_game();
        delay_counter = delay_load_counter;
    }

    draw_game_window();
    draw_food();
    draw_snake();
    draw_score();

    game_score += SCORE_PER_FRAME;
}

void Game_loop()
{
    /* TODO:    Your code goes here
     *
     *          Any code that modifies the screen
     *          which needs to run once per frame
     *          goes here
     */

    Remote_read(&rbs);

    switch(game_state)
    {
    case GS_MENU:
    {
        menu();
    }
    break;
    case GS_PLAYING:
    {
        playing();
    }
    break;
    case GS_HIGHSCORES:
    {
        highscores();
    }
    break;
    case GS_GAME_OVER:
    {
        game_over();
    }
    break;
    }

    if(gs_trans_delay_counter)
        gs_trans_delay_counter--;

    // Swap the buffer (present the drawing to the screen)
    TVOut_Swap();

    // Wait for the frame to complete drawing to the screen
    TVOut_WaitFrames(1);
}
