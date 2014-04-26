#include <pebble.h>

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

static void window_load(Window *window) {
  //Add the main window layer
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  futura_med = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_FUTURA_MED_30));
  futura_med_small = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_FUTURA_MED_18));
  futura_hev = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_FUTURA_HEV_32));
 
  //Create bitmap layers for the screen
  first_time_image = bitmap_layer_create((GRect) { .origin = { 2, 70 }, .size = { 34, 34 } });
  second_time_image = bitmap_layer_create((GRect) { .origin = { 2, 110 }, .size = { 34, 34 } });

  //Add bitmampped images to those layers
  bitmap_layer_set_bitmap(first_time_image, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TIME_REMAINING));
  bitmap_layer_set_bitmap(second_time_image, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TIME_REMAINING));
  
  //Create text layers for the text next to the bitmaps
  first_time = text_layer_create((GRect) { .origin = { 38, 70 }, .size = { 106, 34 } });
  second_time = text_layer_create((GRect) { .origin = { 38, 110 }, .size = { 106, 34 } });
  route_name = text_layer_create((GRect) { .origin = { 2, 2 }, .size = { 144, 34 } });
  stop_name = text_layer_create((GRect) { .origin = { 2, 40 }, .size = { 144, 34 } });


  //Set font for text layers
  text_layer_set_font(first_time, futura_med);
  text_layer_set_font(second_time, futura_med);
  text_layer_set_font(route_name, futura_hev);
  text_layer_set_font(stop_name, futura_med_small);
  
  //Add text to those layers
  text_layer_set_text(first_time, "7 Min");
  text_layer_set_text(second_time, "15 Min");
  text_layer_set_text(route_name, "GRE-N");
  text_layer_set_text(stop_name, "North-Oakland");
  
  //Set font for those text layers
  //text_layer_set_font(first_time, );
  //text_layer_set_size(second_time, 30);  

  //Add the bitmap & text layers to the screen
  layer_add_child(window_layer, bitmap_layer_get_layer(first_time_image));
  layer_add_child(window_layer, bitmap_layer_get_layer(second_time_image));
  layer_add_child(window_layer, text_layer_get_layer(first_time));
  layer_add_child(window_layer, text_layer_get_layer(second_time));
  layer_add_child(window_layer, text_layer_get_layer(route_name));
  layer_add_child(window_layer, text_layer_get_layer(stop_name));

  //text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  //text_layer_set_text(text_layer, "Press a button");
  //text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  //layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
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