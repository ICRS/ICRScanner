#include <lvgl.h>
#include "config.h"

#include <Arduino.h>
#include <Adafruit_PN532.h>
#include <Wire.h>
#include <mutex>

#include <TFT_eSPI.h>

static void lv_port_indev_init(void);

/*Set to your screen resolution and rotation*/
#define TFT_HOR_RES   135
#define TFT_VER_RES   240
#define TFT_ROTATION  LV_DISPLAY_ROTATION_90

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

// #if LV_USE_LOG != 0
void my_print( lv_log_level_t level, const char * buf )
{
    LV_UNUSED(level);
    Serial.println(buf);
    Serial.flush();
}
// #endif
lv_indev_t *indev_button;
int my_btn_read() {
    if (!digitalRead(ENT_BUTTON_PIN)) return 0;
    // if (!digitalRead(NAV_BUTTON_PIN)) return 1;
    return -1;
}

void button_read(lv_indev_t * indev, lv_indev_data_t * data){
    static uint32_t last_btn = 0;   /* Store the last pressed button */
    int btn_pr = my_btn_read();     /* Get the ID (0,1,2...) of the pressed button */
    if(btn_pr >= 0) {               /* Is there a button press? (E.g. -1 indicated no button was pressed) */
       last_btn = btn_pr;           /* Save the ID of the pressed button */
       data->state = LV_INDEV_STATE_PRESSED;  /* Set the pressed state */
    } else {
       data->state = LV_INDEV_STATE_RELEASED; /* Set the released state */
    }

    data->btn_id = last_btn;         /* Save the last button */
}

/* LVGL calls it when a rendered image needs to copied to the display*/
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    /*Copy `px map` to the `area`*/

    /*For example ("my_..." functions needs to be implemented by you)
    uint32_t w = lv_area_get_width(area);
    uint32_t h = lv_area_get_height(area);

    my_set_window(area->x1, area->y1, w, h);
    my_draw_bitmaps(px_map, w * h);
     */

    /*Call it to tell LVGL you are ready*/
    lv_display_flush_ready(disp);
}

/*use Arduinos millis() as tick source*/
static uint32_t my_tick(void)
{
    return millis();
}

int i = 0;
lv_obj_t *scr_0;
lv_obj_t *scr_1;

void ent_button_long(lv_event_t* e) {
    e;
    if (i == 0) {
        lv_screen_load(scr_1);
        i = 1;
    } else {
        lv_screen_load(scr_0);
        i = 0;
    }
    Serial.println("Long Press");
}

void ent_button_long_repeat(lv_event_t* e) {
    e;
    Serial.println("Long Press Repeat");
}

void ent_button_pressed(lv_event_t* e) {
    e;
    Serial.println("ENT Pressed");
}

void setup_screens()
{
    scr_0 = lv_obj_create(NULL);
    scr_1 = lv_obj_create(NULL);
}

void setup()
{
    pinMode(NAV_BUTTON_PIN, INPUT_PULLUP);
    pinMode(ENT_BUTTON_PIN, INPUT_PULLUP);

    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

    Serial.begin(115200);
    Serial.println(LVGL_Arduino);

    lv_init();
    // lv_port_indev_init();

    /*Set a tick source so that LVGL will know how much time elapsed. */
    lv_tick_set_cb(my_tick);

    /* register print function for debugging */
#if LV_USE_LOG != 0
    lv_log_register_print_cb(my_print);
#endif

    lv_display_t *disp;
    // #if LV_USE_TFT_ESPI
    /*TFT_eSPI can be enabled lv_conf.h to initialize the display in a simple way*/
    disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, sizeof(draw_buf));
    lv_display_set_rotation(disp, TFT_ROTATION);
    setup_screens();
    // #else
    //     /*Else create a display yourself*/
    //     disp = lv_display_create(TFT_HOR_RES, TFT_VER_RES);
    //     lv_display_set_flush_cb(disp, my_disp_flush);
    //     lv_display_set_buffers(disp, draw_buf, NULL, sizeof(draw_buf), LV_DISPLAY_RENDER_MODE_PARTIAL);
    // #endif

    // indev_encoder_4th;
    /*Initialize the (dummy) input device driver*/
    indev_button = lv_indev_create();
    lv_indev_set_type(indev_button, LV_INDEV_TYPE_BUTTON);
    lv_indev_set_read_cb(indev_button, button_read);
    static const lv_point_t btn_points[1] = {
        {239, 134},   /*Button 0 -> x:10; y:10*/
        // {40, 100},  /*Button 1 -> x:40; y:100*/
    };
    lv_indev_set_button_points(indev_button, btn_points);
    // lv_indev_set_mode(indev_button, LV_INDEV_MODE_TIMER);
    lv_indev_enable(indev_button, true);
    // lv_indev_add_event_cb(indev_button, ent_button_pressed, LV_EVENT_ALL, NULL);
    lv_indev_add_event_cb(indev_button, ent_button_pressed, LV_EVENT_SHORT_CLICKED, NULL);
    lv_indev_add_event_cb(indev_button, ent_button_long, LV_EVENT_LONG_PRESSED, NULL);
    // lv_indev_add_event_cb(indev_button, ent_button_long_repeat, LV_EVENT_LONG_PRESSED_REPEAT, NULL);

    lv_obj_t *label = lv_label_create(scr_0);
    lv_label_set_text(label, "Hello Arduino, I'm LVGL!");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *label_3 = lv_label_create(scr_1);
    lv_label_set_text(label_3, "Screen 2!");
    lv_obj_align(label_3, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *label2 = lv_label_create(scr_0);
    lv_label_set_text(label2, "ICRS");

    static lv_style_t style;
    lv_style_init(&style);
    lv_color_t color{(uint8_t)0b11111111, (uint8_t)0x0, (uint8_t)0x0};
    lv_style_set_text_color(&style, color);
    lv_obj_add_style(label2, &style, 0);
    lv_obj_align(label2, LV_ALIGN_TOP_MID, 0, 0);
    lv_screen_load(scr_0);

    Serial.println("Setup done");
}

void loop()
{
    lv_timer_handler(); /* let the GUI do its work */
    delay(5);           /* let this time pass */
}
