#include <pebble.h>
#include <math.h>
#include <settings.h>
#include <calendarUtils.h>
#include <calendarWindow.h>

Window *calendar_window;
Layer *days_layer;
struct tm * curTime;
void setColors(GContext* ctx){
        window_set_background_color(calendar_window, GColorWhite);
        graphics_context_set_stroke_color(ctx, GColorBlack);
        graphics_context_set_fill_color(ctx, GColorWhite);
        graphics_context_set_text_color(ctx, GColorBlack);
}
void setInvColors(GContext* ctx){
        graphics_context_set_stroke_color(ctx, GColorWhite);
        graphics_context_set_fill_color(ctx, GColorBlack);
        graphics_context_set_text_color(ctx, GColorWhite);
}

void draw_date(GContext* ctx, struct tm *currentTime, int cal_y){

        char str[20],aux[20];
        strftime(aux, sizeof(aux), " %Y",currentTime );
        strftime(str, sizeof(str), "%B",currentTime );
        strcpy(str,traduction(str));
        strcat(str,aux);
        
        int date_h = graphics_text_layout_get_content_size(
                        str,
                        fonts_get_system_font(FONT_KEY_GOTHIC_24), 
                        GRect(0,0,PEB_X,PEB_Y),
                        GTextOverflowModeWordWrap, 
                        GTextAlignmentCenter).h;
        
    
        
        // Draw the MONTH/YEAR String
        graphics_draw_text(
            ctx, 
            str,  
            fonts_get_system_font(FONT_KEY_GOTHIC_24),
            GRect(0,cal_y-DATE_H-DATE_OFF,PEB_X,date_h),
            GTextOverflowModeWordWrap, 
            GTextAlignmentCenter, 
            NULL);
        
}
void draw_labels(GContext* ctx, int cal_x, int cal_y, int cell_x){
    int i,j;
    
    for(i=0;i<7;i++){
        
        // Adjust labels by specified offset
        j = i+1;
        if(j>6) j-=7;
        if(j<0) j+=7;
        graphics_draw_text(
            ctx, 
            daysOfWeek[j], 
            fonts_get_system_font(FONT_KEY_GOTHIC_14), 
            GRect(cal_x+1+i*cell_x, cal_y-16, cell_x-1, 15), 
            GTextOverflowModeWordWrap, 
            GTextAlignmentCenter, 
            NULL); 
    }
}
void draw_grid( GContext* ctx,
                int rows,   int cols,
                int cell_x, int cell_y,
                int cal_x,  int cal_y){
    int i;        
    // horizontal lines
    for(i=1;i<=rows;i++){
        graphics_draw_line(ctx, GPoint(0, PEB_Y-i*cell_y), GPoint(PEB_X, PEB_Y-i*cell_y));
    }
    // vertical lines
    for(i=1;i<cols;i++){
        graphics_draw_line(ctx, GPoint(cal_x+i*cell_x, cal_y), GPoint(cal_x+i*cell_x, PEB_Y));
    }
}
void days_layer_update_callback(Layer *me, GContext* ctx) {
    
    // Find the Target Month
    time_t now = time(NULL);
    struct tm *currentTime = localtime(&now);
    currentTime->tm_mon=currentTime->tm_mon+offset;
    if(offset!=0)
        currentTime->tm_mday=1;
    now = p_mktime(currentTime);
    currentTime = localtime(&now);
    
    // Extract the info we need to know about the month to build a calendar
    int year = currentTime->tm_year+1900;                         // Year
    int mon = currentTime->tm_mon;                                // Month
    int today = currentTime->tm_mday;                             // Day of Month (only relevent if this we are looking at the current month)
    int dom = daysInMonth(mon,year);                              // Days in the target month

    
    // Find the number of weeks that we will be showing,
    // and the day of month that we will start on
        
    // We will always start on the first day of the week
    // if this is has us starting in the previous month,
    // we will just hide those days if, we do not want to show them
    
    int rows,start;
        int idow = wdayOfFirst(currentTime->tm_wday,currentTime->tm_mday) - 1; if(idow<0) idow+=7;
        rows = ceil(((float) (idow + dom))/7);
        start = 1-idow;
    
    // Find the last day that will show on the calendar
    int end = start + rows*7;
    
    // Size & position of Day squares
    // First the constants that setup the screen size
    int label_h = LABEL_H;
    
    int cols = 7;      // number of columns (days of the week)
    int cell_x = floor(PEB_X/cols);   // width of columns 
    
    
    int cal_x,cal_y;
    
    cal_y = PEB_Y;
    
	cal_y -= DATE_H;
	
        cal_y -= LABEL_H;
    int cell_y = floor(cal_y/rows);
    
    if(cell_y>30)
        cell_y = 30;
    
    cal_y = PEB_Y-rows*cell_y;
    cal_x = floor((PEB_X-1-(cell_x*cols))/2);
    
    GFont font = fonts_get_system_font(FONT_KEY_GOTHIC_14);
    int fh = 15;
    int fo = 9;
    setColors(ctx);
    
    int i,day;
    int dow = 0;
    int wknum = 0;
    char buff[8];
    
    draw_grid(ctx,rows,cols,cell_x,cell_y,cal_x,cal_y);
    
    draw_labels(ctx,cal_x,cal_y,cell_x);
        
    for(i=start;i<=end;i++){
    
        if(dow > 6){
            dow = 0;
            wknum ++;
        }        
        
        if(i==today && offset == 0){
            setInvColors(ctx);
            graphics_fill_rect(
                ctx,
                GRect(
                    cal_x+dow*cell_x+1 + ((dow==0&&cols==7)?-2:0), 
                    PEB_Y-(rows-wknum)*cell_y+1, 
                    cell_x-1 + ((dow==0&&cols==7)?2:0) + ((dow==6)?3:0), 
                    cell_y-1)
                ,0
                ,GCornerNone);
        }
  
        if(i<1){
                dow++;
                continue;
        }else if(i>dom){
                dow++;
                continue;
        }else{
            day = i;
        
        }
        snprintf(buff,sizeof(buff),"%d", day);

        graphics_draw_text(
            ctx, 
            buff,  
            font, 
            GRect(
                cal_x+1+dow*cell_x, 
                PEB_Y-(-0.5+rows-wknum)*cell_y-fo, 
                cell_x-1, 
                fh), 
            GTextOverflowModeWordWrap, 
            GTextAlignmentCenter, 
            NULL); 
        
        setColors(ctx);
        font = fonts_get_system_font(FONT_KEY_GOTHIC_14);
        fh = 15;
        fo = 9;

        dow++;   
    }
    
	currentTime->tm_year = year - 1900;
	currentTime->tm_mon = mon;
	currentTime->tm_mday = today;
	draw_date(ctx,currentTime,cal_y-label_h);
}

void get_event_days(int o){
        
    time_t now = time(NULL);
    struct tm *currentTime = localtime(&now);
        
    int year = currentTime->tm_year;
    int month = currentTime->tm_mon+offset+o;
    
    factorDate(&month,&year);
    
    if(year*100+month>0){
        DictionaryIterator *iter;

        if (app_message_outbox_begin(&iter) != APP_MSG_OK) {
            app_log(APP_LOG_LEVEL_DEBUG, "calendarApp.c",364,"App MSG Not ok");
            return;
        }    
        if (dict_write_uint16(iter, GET_EVENT_DAYS, ((uint16_t)year*100+month)) != DICT_OK) {
            app_log(APP_LOG_LEVEL_DEBUG, "calendarApp.c",364,"Dict Not ok");
            return;
        }
        if (app_message_outbox_send() != APP_MSG_OK){
            app_log(APP_LOG_LEVEL_DEBUG, "calendarApp.c",364,"Message Not Sent");
            return;
        }
        app_log(APP_LOG_LEVEL_DEBUG, "calendarApp.c",364,"Message Sent");
    }
}


bool processEncoded(uint8_t encoded[42],bool decoded[32]){
    int index;
    bool tmp;
    bool changed = false;
    for (int byteIndex = 0;  byteIndex < 4; byteIndex++){
        for (int bitIndex = 0;  bitIndex < 8; bitIndex++){
            index = byteIndex*8+bitIndex;
            tmp = (encoded[byteIndex] & (1 << bitIndex)) != 0;
            if( decoded[index] != tmp){
                decoded[index]  = tmp;
                changed = true;
            }
        }
    }
    return changed;
}

void clearCalEvents(){

    time_t now = time(NULL);
    struct tm *currentTime = localtime(&now);
        
    int year = currentTime->tm_year;
    int month = currentTime->tm_mon+offset ;
    
    factorDate(&month,&year);
    
    if(year*100+month > 0 && persist_exists(year*100+month)){
        uint8_t this_month[42];
        persist_read_data(year*100+month, this_month, 8);
    }
    month +=1;
    factorDate(&month,&year);
    if(persist_exists(year*100+month)){
        uint8_t next_month[42];
        persist_read_data(year*100+month, next_month, 8);
    
    }
    month -=2;
    factorDate(&month,&year);
    if(persist_exists(year*100+month)){
        uint8_t last_month[42];
        persist_read_data(year*100+month, last_month, 8);
    }

}

void monthChanged(){
    clearCalEvents();
    get_event_days(0);
    
    layer_mark_dirty(days_layer);
}


void up_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    offset--;
    monthChanged();
}
void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    offset++;
    monthChanged();
}
void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    if(offset != 0){
        offset = 0;
        monthChanged();
    }
}
static void click_config_provider(void* context){
    
    window_single_click_subscribe(          BUTTON_ID_SELECT,  select_single_click_handler);
// Up and Down should be repeating clicks, but due to bug in 2.0 firmware repeating clicks cause watch to crash
  window_single_repeating_click_subscribe(BUTTON_ID_UP  , 100,   up_single_click_handler);
  window_single_repeating_click_subscribe(BUTTON_ID_DOWN, 100, down_single_click_handler);
//    window_single_click_subscribe(BUTTON_ID_UP  ,   up_single_click_handler);
//    window_single_click_subscribe(BUTTON_ID_DOWN,   down_single_click_handler);

   
}

void calendar_window_unload(Window *window) {
    layer_destroy(days_layer);
}

void calendar_window_load(Window *window) {
    clearCalEvents();
	window_set_click_config_provider(window, (ClickConfigProvider) click_config_provider);
	 
    window_set_background_color(window, GColorWhite);

    Layer *window_layer = window_get_root_layer(window);
    
    days_layer = layer_create(layer_get_bounds(window_layer));
    layer_set_update_proc(days_layer, days_layer_update_callback);
    layer_add_child(window_layer, days_layer);
   
}
