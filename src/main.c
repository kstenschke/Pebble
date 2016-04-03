#include <pebble.h>

#define PI 3.14159265

Window *my_window;
static Layer *canvas_layer;
GRect bounds;
bool has_time = false;
bool dots_on = true;
static char buffer[] = "00:00";

void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{

}

static int draw_dots(GContext *ctx, int x, int y, bool blink) {
  if(!blink || dots_on) {
    dots_on = !dots_on;
    
    graphics_context_set_fill_color(ctx, GColorWhite);
    
    uint16_t radius = 2;
    
    GPoint center = GPoint(x+4, y+9);
    graphics_fill_circle(ctx, center, radius);
      
    center = GPoint(x+4, y+23);
    graphics_fill_circle(ctx, center, radius);
  }
  
  return x+9;
}

static int draw_one(GContext *ctx, int x, int y) {
  graphics_fill_rect(ctx, GRect(x+12, y-1, 4, 35), 0, GCornerNone);
  
  return x+16;
}

static int draw_zero(GContext *ctx, int x, int y) {
  graphics_fill_rect(ctx, GRect(x, y, 4, 32), 0, GCornerNone);
  
  graphics_fill_rect(ctx, GRect(x, y-1, 16, 4), 0, GCornerNone);
  graphics_fill_rect(ctx, GRect(x, y+30, 16, 4), 0, GCornerNone);
  
  draw_one(ctx, x, y);
  
  return x+16;
}

static int draw_two(GContext *ctx, int x, int y) {
  graphics_fill_rect(ctx, GRect(x, y+17, 4, 16), 0, GCornerNone);
  
  graphics_fill_rect(ctx, GRect(x, y-1, 16, 4), 0, GCornerNone);
  graphics_fill_rect(ctx, GRect(x, y+14, 16, 4), 0, GCornerNone);  
  graphics_fill_rect(ctx, GRect(x, y+30, 16, 4), 0, GCornerNone);
  
  graphics_fill_rect(ctx, GRect(x+12, y, 4, 16), 0, GCornerNone);
  
  return x+16;
}

static int draw_three(GContext *ctx, int x, int y) {
  graphics_fill_rect(ctx, GRect(x, y-1, 16, 4), 0, GCornerNone);
  graphics_fill_rect(ctx, GRect(x, y+14, 16, 4), 0, GCornerNone);  
  graphics_fill_rect(ctx, GRect(x, y+30, 16, 4), 0, GCornerNone);
  
  draw_one(ctx, x, y);
  
  return x+16;
}

static int draw_four(GContext *ctx, int x, int y) {
  graphics_fill_rect(ctx, GRect(x, y, 4, 16), 0, GCornerNone);
  graphics_fill_rect(ctx, GRect(x, y+14, 16, 4), 0, GCornerNone);  
  
  draw_one(ctx, x, y);
  
  return x+16;
}

static int draw_five(GContext *ctx, int x, int y) {
  graphics_fill_rect(ctx, GRect(x, y, 4, 16), 0, GCornerNone);
  
  graphics_fill_rect(ctx, GRect(x, y-1, 16, 4), 0, GCornerNone);
  graphics_fill_rect(ctx, GRect(x, y+14, 16, 4), 0, GCornerNone);  
  graphics_fill_rect(ctx, GRect(x, y+30, 16, 4), 0, GCornerNone);
  
  graphics_fill_rect(ctx, GRect(x+12, y+17, 4, 16), 0, GCornerNone);
  
  return x+16;
}

static int draw_six(GContext *ctx, int x, int y) {
  graphics_fill_rect(ctx, GRect(x, y, 4, 16), 0, GCornerNone);
  graphics_fill_rect(ctx, GRect(x, y+17, 4, 16), 0, GCornerNone);
  
  graphics_fill_rect(ctx, GRect(x, y-1, 16, 4), 0, GCornerNone);
  graphics_fill_rect(ctx, GRect(x, y+14, 16, 4), 0, GCornerNone);  
  graphics_fill_rect(ctx, GRect(x, y+30, 16, 4), 0, GCornerNone);
  
  graphics_fill_rect(ctx, GRect(x+12, y+17, 4, 16), 0, GCornerNone);
  
  return x+16;
}

static int draw_seven(GContext *ctx, int x, int y) {
  graphics_fill_rect(ctx, GRect(x, y-1, 16, 4), 0, GCornerNone);
  
  draw_one(ctx, x, y);
  
  return x+16;
}

static int draw_eight(GContext *ctx, int x, int y) {
  graphics_fill_rect(ctx, GRect(x, y+14, 16, 4), 0, GCornerNone);  
  
  draw_zero(ctx, x, y);
  
  return x+16;
}

static int draw_nine(GContext *ctx, int x, int y) {
  graphics_fill_rect(ctx, GRect(x, y, 4, 16), 0, GCornerNone);
  
  draw_three(ctx, x, y);
  
  return x+16;
}

static void draw_text(GContext *ctx, int x, int y) {
//  graphics_fill_rect(ctx, GRect(0, y-12, 144, 1), 0, GCornerNone);
  
  
}

static int draw_number(char c, GContext *ctx, int x, int y) {
  switch(c) {
    case '1':
      return draw_one(ctx, x, y);
    case '2':
      return draw_two(ctx, x, y);
    case '3':
      return draw_three(ctx, x, y);
    case '4':
      return draw_four(ctx, x, y);
    case '5':
      return draw_five(ctx, x, y);
    case '6':
      return draw_six(ctx, x, y);
    case '7':
      return draw_seven(ctx, x, y);
    case '8':
      return draw_eight(ctx, x, y);
    case '9':
      return draw_nine(ctx, x, y);
  }
  
  return draw_zero(ctx, x, y);
}

static void canvas_update_proc(Layer *layer, GContext *ctx) {
  if(has_time) {
    graphics_context_set_text_color(ctx, GColorBlack);
    graphics_context_set_fill_color(ctx, GColorWhite);
    
    int offset = 5;
    int x = 26;
    int y = 60;
    
    x = draw_number(buffer[0], ctx, x, y);
    x = draw_number(buffer[1], ctx, x+offset, y);
    x = draw_dots(ctx, x+offset, y, false);  
    x = draw_number(buffer[3], ctx, x+offset, y);
    x = draw_number(buffer[4], ctx, x+offset, y);
  }
}

static void handle_tick(struct tm *tick_time, TimeUnits units_changed) {
  strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  has_time = true;
  layer_mark_dirty(canvas_layer);
}

void handle_init(void) {
  my_window = window_create();
  window_set_background_color(my_window, GColorBlack);

  // Create canvas layer
  bounds = layer_get_bounds(window_get_root_layer(my_window));
  canvas_layer = layer_create(bounds);

  // Assign the custom drawing procedure
  layer_set_update_proc(canvas_layer, canvas_update_proc);
  
  // Add the layer, publish the window
  layer_add_child(window_get_root_layer(my_window), canvas_layer);
  window_stack_push(my_window, true);
  
  // Add update per minute
  tick_timer_service_subscribe(SECOND_UNIT, (TickHandler)handle_tick);
}

void handle_deinit(void) {
  layer_destroy(canvas_layer);
  window_destroy(my_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
