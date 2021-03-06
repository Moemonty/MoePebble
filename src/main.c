#include <pebble.h>
static Window *s_main_window;

static TextLayer *s_time_layer;

static TextLayer *s_weather_layer;

static GFont s_time_font;

//Loading Font
static  GFont s_time_font;
static GFont s_weather_font;



static BitmapLayer *s_background_layer;

static GBitmap *s_background_bitmap;

static void update_time() {
  
  time_t temp = time(NULL);
  
  struct tm *tick_time = localtime(&temp);
  
  static char buffer[] = "00:00";
  
  if(clock_is_24h_style() == true) {
    strftime(buffer, sizeof("00:00"), "%H:%M" , tick_time);
    
  } else {
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }
  
  text_layer_set_text(s_time_layer, buffer);
  
}



static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}


static void main_window_load(Window *window) {
  
  //background bitmap
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
  
  s_background_layer = bitmap_layer_create(GRect(0,0,144,168));
  
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
  
  
  
  //Open Data Weather Layer
  s_weather_layer = text_layer_create(GRect(0, 120, 144, 25));
  
  text_layer_set_background_color(s_weather_layer, GColorClear);
  
  text_layer_set_text_color(s_weather_layer, GColorWhite);
  
  text_layer_set_text_alignment(s_weather_layer, GTextAlignmentCenter);

  text_layer_set_text(s_weather_layer, "Moemonty");
  
  
  s_time_layer = text_layer_create(GRect(5,52,139,50));
  
  text_layer_set_background_color(s_time_layer, GColorClear);
  
  text_layer_set_text_color(s_time_layer, GColorBlack);
  
  
  //weahter text layer
  
  s_weather_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_20));
  text_layer_set_font(s_weather_layer, s_weather_font);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weather_layer));
  
  
  //text_layer_set_text(s_time_layer, "00:00");
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_48));
  
  text_layer_set_font(s_time_layer, s_time_font);
  
  //text_layer_set_font(s_time_layer, fonts_get_system_font(RESOURCE_ID_FONT_PERFECT_DOS_48));
  
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  
}

static void main_window_unload(Window *window) {
    gbitmap_destroy(s_background_bitmap);
    text_layer_destroy(s_time_layer);
    fonts_unload_custom_font(s_time_font);
    bitmap_layer_destroy(s_background_layer);
    text_layer_destroy(s_weather_layer);
    fonts_unload_custom_font(s_weather_font);
}
  


  
static void init() {
  
  //Register with TickTimerService
  
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);  
  
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  update_time();
}

static void deinit() {
  window_destroy(s_main_window);
}


int main(void) {
  init();
  app_event_loop();
  deinit();
}