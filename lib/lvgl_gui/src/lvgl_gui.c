#include "lvgl.h"
#include "lvgl_gui.h"

static lv_obj_t * chart;
static lv_obj_t * active_screen;
static lv_style_t chart_style;

static void draw_example_chart (void) {

    /*Create a chart*/

    active_screen = lv_scr_act();
    chart = lv_chart_create(active_screen);
    lv_obj_set_size(chart, 320, 240);
    lv_obj_center(chart);
    lv_obj_add_style(chart, &chart_style, LV_PART_MAIN);
  // lv_obj_set_style_bg_color(chart, lv_color_hex(0x42819B), 0);
    lv_style_set_bg_color(&chart_style, lv_color_hex(0x1d1f1d));
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);   /*Show lines and points too*/

    /*Add two data series*/
    lv_chart_series_t * ser1 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_series_t * ser2 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_SECONDARY_Y);

    /*Set the next points on 'ser1'*/
    lv_chart_set_next_value(chart, ser1, 10);
    lv_chart_set_next_value(chart, ser1, 10);
    lv_chart_set_next_value(chart, ser1, 10);
    lv_chart_set_next_value(chart, ser1, 10);
    lv_chart_set_next_value(chart, ser1, 10);
    lv_chart_set_next_value(chart, ser1, 10);
    lv_chart_set_next_value(chart, ser1, 10);
    lv_chart_set_next_value(chart, ser1, 30);
    lv_chart_set_next_value(chart, ser1, 70);
    lv_chart_set_next_value(chart, ser1, 90);

    /*Directly set points on 'ser2'*/
    ser2->y_points[0] = 90;
    ser2->y_points[1] = 70;
    ser2->y_points[2] = 65;
    ser2->y_points[3] = 65;
    ser2->y_points[4] = 65;
    ser2->y_points[5] = 65;
    ser2->y_points[6] = 65;
    ser2->y_points[7] = 65;
    ser2->y_points[8] = 65;
    ser2->y_points[9] = 65;

    lv_chart_refresh(chart); /*Required after direct set*/
}


void gui_init (void) {
    draw_example_chart();
}