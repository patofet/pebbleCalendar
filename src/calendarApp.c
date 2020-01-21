#include <pebble.h>
#include <settings.h>
#include <calendarWindow.h>

static void handle_tick(struct tm *tick_time, TimeUnits units_changed) {
  layer_mark_dirty(days_layer);
}

int main(void) {
    tick_timer_service_subscribe(DAY_UNIT, handle_tick);
    
    calendar_window = window_create();
    window_set_fullscreen(calendar_window, true);
    window_set_window_handlers(calendar_window, (WindowHandlers) {
        .load = calendar_window_load,
        .unload = calendar_window_unload,
    });
    
    window_stack_push(calendar_window, true);
    app_event_loop();
    window_destroy(calendar_window);
}
