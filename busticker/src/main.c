#include <pebble.h>

enum Bus_Key{
  ROUTE_NAME_KEY = 0x0,
  STOP_NAME_KEY = 0x1,
  FIRST_TIME_KEY = 0x2,
  SECOND_TIME_KEY = 0x3  
};

static Window *window;

static TextLayer *route_name;
static TextLayer *stop_name;
static TextLayer *first_time;
static TextLayer *second_time;
static BitmapLayer *first_time_image;
static BitmapLayer *second_time_image;
static GFont futura_med;
static GFont futura_med_small;
static GFont futura_hev;

AppSync sync;
uint8_t sync_buffer[128];

//Not currently supporting the buttons
/*	Button click event handlers
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Select");
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Up");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Down");
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}
*/

static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", app_message_error);
}

static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
  switch (key) {
    case ROUTE_NAME_KEY:
      text_layer_set_text(route_name, new_tuple->value->cstring);
      layer_mark_dirty((Layer *)route_name);
      break;
    case STOP_NAME_KEY:
      text_layer_set_text(stop_name, new_tuple->value->cstring);
      layer_mark_dirty((Layer *)stop_name);
      break;
    case FIRST_TIME_KEY:
      text_layer_set_text(first_time, new_tuple->value->cstring);
      layer_mark_dirty((Layer *)first_time);
      break;
    case SECOND_TIME_KEY:
      text_layer_set_text(second_time, new_tuple->value->cstring);
      layer_mark_dirty((Layer *)second_time);
      break;
  }
}

static void window_load(Window *window) {
  //Add the main window layer
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  GRect max_window_bounds = GRect(0, 0, bounds.size.w, 168);
  
  //Declare fonts
  futura_med = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_FUTURA_MED_30));
  futura_med_small = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_FUTURA_MED_18));
  futura_hev = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_FUTURA_HEV_32));
 
  //Create bitmap layers for the screen
  first_time_image = bitmap_layer_create((GRect) { .origin = { 2, 80 }, .size = { 34, 34 } });
  second_time_image = bitmap_layer_create((GRect) { .origin = { 2, 110 }, .size = { 34, 34 } });

  //Add bitmampped images to those layers
  bitmap_layer_set_bitmap(first_time_image, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TIME_REMAINING));
  bitmap_layer_set_bitmap(second_time_image, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TIME_REMAINING));
  
  //Create/position the text layers
  route_name = text_layer_create((GRect) { .origin = { 2, 2 }, .size = { 144, 34 } });
  stop_name = text_layer_create((GRect) { .origin = { 3, 37 }, .size = { 144, 39 } });
  first_time = text_layer_create((GRect) { .origin = { 38, 80 }, .size = { 106, 34 } });
  second_time = text_layer_create((GRect) { .origin = { 38, 110 }, .size = { 106, 34 } });
    
  //Set proper overflow
  text_layer_set_overflow_mode(stop_name, GTextOverflowModeFill);
  
  //Set font for text layers
  text_layer_set_font(route_name, futura_hev);
  text_layer_set_font(stop_name, futura_med_small);
  text_layer_set_font(first_time, futura_med);
  text_layer_set_font(second_time, futura_med);
  
  //Get data from iPhone
  Tuplet initial_values[] = {
    TupletCString(ROUTE_NAME_KEY, "N/A"),
    TupletCString(STOP_NAME_KEY, "N/A"),
    TupletCString(FIRST_TIME_KEY, "N/A"),
    TupletCString(SECOND_TIME_KEY, "N/A")
  };
  app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_values, 
                  4, sync_tuple_changed_callback, 
                  sync_error_callback, NULL);

  //Add the bitmap & text layers to the screen
  layer_add_child(window_layer, bitmap_layer_get_layer(first_time_image));
  layer_add_child(window_layer, bitmap_layer_get_layer(second_time_image));
  layer_add_child(window_layer, text_layer_get_layer(first_time));
  layer_add_child(window_layer, text_layer_get_layer(second_time));
  layer_add_child(window_layer, text_layer_get_layer(route_name));
  layer_add_child(window_layer, text_layer_get_layer(stop_name));
}

static void window_unload(Window *window) {
  app_sync_deinit(&sync);
  text_layer_destroy(route_name);
  text_layer_destroy(stop_name);
  text_layer_destroy(first_time);
  text_layer_destroy(second_time);
  bitmap_layer_destroy(first_time_image);
  bitmap_layer_destroy(second_time_image);
  fonts_unload_custom_font(futura_med);
  fonts_unload_custom_font(futura_med_small);
  fonts_unload_custom_font(futura_hev);
}

static void init(void) {
  window = window_create();
  
  //Synchronization with phone
  const int inbound_size = 128;
  const int outbound_size = 128;
  app_message_open(inbound_size, outbound_size);
  //window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
	.load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}