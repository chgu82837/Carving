
void pop_obj_type();
void set_obj_type(bool is_sphere);
void draw_obj();

#define MTL_gray 0
#define MTL_red 1
#define MTL_lavender 2
#define MTL_blue 3
#define MTL_yellow 4
#define MTL_orange 5
#define MTL_green 6
#define MTL_cyan 7
#define MTL_purple 8
#define MTL_sun 9
#define MTL_blood 10
#define MTL_fire 11
#define MTL_wall_gray 12
#define MTL_wall_red 13
#define MTL_wall_lavender 14
#define MTL_wall_blue 15
#define MTL_wall_yellow 16
#define MTL_hurtable_fire 17
#define MTL_hurtable_death 18

#define MTL_MAX 18
#define MTL_WALL_BASE 12
#define MTL_NORMAL_MAX 8
#define MTL_WALL_L 5

void pop_mtl();
void set_mtl(int mtl_i);
