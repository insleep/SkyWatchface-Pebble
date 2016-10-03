#include <pebble.h>

// Declarations
Window *window;
TextLayer *time_layer;
TextLayer *date_layer;
TextLayer *day_layer;
TextLayer *battery_layer;
TextLayer *bt_layer;
TextLayer *black_layer; // Not a text layer, but doesn't matter

static void handle_bt(bool connected) {
  text_layer_set_text(bt_layer, connected ? "connected" : "disconnected");
}

static void handle_battery(BatteryChargeState charge_state) {
  static char battery_text[] = "charging";

  if (charge_state.is_charging) {
    snprintf(battery_text, sizeof(battery_text), "charging");
  } else {
    snprintf(battery_text, sizeof(battery_text), "%d%%", charge_state.charge_percent);
  }
  text_layer_set_text(battery_layer, battery_text);
}

void handle_timechanges(struct tm *tick_time, TimeUnits units_changed) {
  static char time_buffer[10];
  static char date_buffer[10];
  static char day_buffer[10];
  
  // Set the time (12hr or 24hr)
  if(clock_is_24h_style() == true) {
    // Use 2h hour format
    strftime(time_buffer, sizeof(time_buffer), "%H:%M", tick_time);
    text_layer_set_text(time_layer, time_buffer);
  } else {
    // Use 12 hour format
    strftime(time_buffer, sizeof(time_buffer), "%I:%M", tick_time);
    text_layer_set_text(time_layer, time_buffer);
  }
  
  // Set the date (m-d-y)
  strftime(date_buffer, sizeof(date_buffer), "%m-%d-%y", tick_time);
  text_layer_set_text(date_layer, date_buffer);
  
  // Set the full day name
  strftime(day_buffer, sizeof(day_buffer), "%A", tick_time);
  text_layer_set_text(day_layer, day_buffer);
  
    if (!clock_is_24h_style() && (time_buffer[0] == '0')) {
    memmove(time_buffer, &time_buffer[1], sizeof(time_buffer) - 1);
  }
  
  text_layer_set_text(time_layer, time_buffer);
  handle_battery(battery_state_service_peek());
}

void handle_init(void) {
  
  window = window_create();
  Layer *root_layer = window_get_root_layer(window);
  GRect frame = layer_get_frame(root_layer);
  
  #if defined(PBL_ROUND)
    // Create the black layer (previously inverter layer)
    black_layer = text_layer_create(GRect(0, 100, frame.size.w /* width */, frame.size.h / 2 /* height */));
    text_layer_set_background_color(black_layer, GColorBlack);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(black_layer));
  
    // Create the time layer
    time_layer = text_layer_create(GRect(0, 51, frame.size.w /* width */, 154 /* height */));
    text_layer_set_text_color(time_layer, GColorBlack);
    text_layer_set_background_color(time_layer, GColorClear);
    text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
    text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
    text_layer_set_text(time_layer, "Loading...");
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer));
  
    // Create the day layer
    day_layer = text_layer_create(GRect(0, 101, frame.size.w /* width */, 56 /* height */));
    text_layer_set_text_color(day_layer, GColorWhite);
    text_layer_set_background_color(day_layer, GColorClear);
    text_layer_set_font(day_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    text_layer_set_text_alignment(day_layer, GTextAlignmentCenter);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(day_layer));
  
    // Create the date layer
    date_layer = text_layer_create(GRect(0, 133, frame.size.w /* width */, 56 /* height */));
    text_layer_set_text_color(date_layer, GColorWhite);
    text_layer_set_background_color(date_layer, GColorClear);
    text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
    text_layer_set_font(date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));
  
    // Create the Bluetooth layer
    bt_layer = text_layer_create(GRect(0, 10, frame.size.w /* width */, 34 /* height */));
    text_layer_set_text_color(bt_layer, GColorBlack);
    text_layer_set_background_color(bt_layer, GColorClear);
    text_layer_set_font(bt_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    text_layer_set_text_alignment(bt_layer, GTextAlignmentCenter);
    handle_bt(bluetooth_connection_service_peek());

    // Create the battery layer
    battery_layer = text_layer_create(GRect(0, 30, frame.size.w /* width */, 34 /* height */));
    text_layer_set_text_color(battery_layer, GColorBlack);
    text_layer_set_background_color(battery_layer, GColorClear);
    text_layer_set_font(battery_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    text_layer_set_text_alignment(battery_layer, GTextAlignmentCenter);
    text_layer_set_text(battery_layer, "charging");
  #elif defined(PBL_RECT)
    // Create the black layer (previously inverter layer)
    black_layer = text_layer_create(GRect(0, 85, frame.size.w /* width */, frame.size.h /2 /* height */));
    text_layer_set_background_color(black_layer, GColorBlack);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(black_layer));
  
    // Create the time layer
    time_layer = text_layer_create(GRect(0, 36, 144 /* width */, 154 /* height */));
    text_layer_set_text_color(time_layer, GColorBlack);
    text_layer_set_background_color(time_layer, GColorClear);
    text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
    text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
    text_layer_set_text(time_layer, "Loading...");
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer));
  
    // Create the day layer
    day_layer = text_layer_create(GRect(0, 86, 144 /* width */, 56 /* height */));
    text_layer_set_text_color(day_layer, GColorWhite);
    text_layer_set_background_color(day_layer, GColorClear);
    text_layer_set_text_alignment(day_layer, GTextAlignmentCenter);
    text_layer_set_font(day_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(day_layer));
  
    // Create the date layer
    date_layer = text_layer_create(GRect(0, 118, 144 /* width */, 56 /* height */));
    text_layer_set_text_color(date_layer, GColorWhite);
    text_layer_set_background_color(date_layer, GColorClear);
    text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
    text_layer_set_font(date_layer, fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS));
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));
  
    // Create the Bluetooth layer
    bt_layer = text_layer_create(GRect(0, 0, frame.size.w /* width */, 34 /* height */));
    text_layer_set_text_color(bt_layer, GColorBlack);
    text_layer_set_background_color(bt_layer, GColorClear);
    text_layer_set_font(bt_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    text_layer_set_text_alignment(bt_layer, GTextAlignmentLeft);
    handle_bt(bluetooth_connection_service_peek());

    // Create the battery layer
    battery_layer = text_layer_create(GRect(0, 0, frame.size.w /* width */, 34 /* height */));
    text_layer_set_text_color(battery_layer, GColorBlack);
    text_layer_set_background_color(battery_layer, GColorClear);
    text_layer_set_font(battery_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    text_layer_set_text_alignment(battery_layer, GTextAlignmentRight);
    text_layer_set_text(battery_layer, "charging");
  #endif
  
  
  
  time_t now = time(NULL);
  handle_timechanges(localtime(&now), MINUTE_UNIT);
  tick_timer_service_subscribe(MINUTE_UNIT, handle_timechanges);
  
  battery_state_service_subscribe(&handle_battery);
  bluetooth_connection_service_subscribe(&handle_bt);
  
  layer_add_child(root_layer, text_layer_get_layer(bt_layer));
  layer_add_child(root_layer, text_layer_get_layer(battery_layer));
  
  window_stack_push(window, true);
}

  void handle_deinit(void) {
    text_layer_destroy(black_layer);
    text_layer_destroy(time_layer);
    text_layer_destroy(day_layer);
    text_layer_destroy(date_layer);
    text_layer_destroy(bt_layer);
    text_layer_destroy(battery_layer);
    
    window_destroy(window);
  }

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}