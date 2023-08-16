#include "lvgl.h"
#include "lvgl_gui.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef ESP_PLATFORM
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#endif

static lv_obj_t * chart;
static lv_obj_t * active_screen;
static lv_style_t chart_style;

#define POINTS_NUMBER 3001
#define Y_MIN_VALUE -100
#define Y_MAX_VALUE 100

#define TABLE_SIZE 500
float samples [TABLE_SIZE];
#define CYCLES 6


#define TWO_PI (3.141592653589793238 * 2)

void generate_example_values(void) {
  
  float phaseIncrement = TWO_PI/TABLE_SIZE;
  float currentPhase = 0.0;
  int i;

  for (i = 0; i < TABLE_SIZE; i ++) {
    #ifdef ESP_PLATFORM
    vTaskDelay(pdMS_TO_TICKS(10));
    #endif

    samples[i] = sin(currentPhase);
    currentPhase += phaseIncrement;
  }
}

static void draw_example_chart (void) {

    /*Create a chart*/

    active_screen = lv_scr_act();
    chart = lv_chart_create(active_screen);
    lv_obj_set_size(chart, 320, 240);
    lv_obj_center(chart);
    lv_obj_set_style_bg_color(chart, lv_color_hex(0x1d1f1d), 0);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);   /*Show lines and points too*/
    lv_chart_set_point_count(chart, POINTS_NUMBER);
    lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_CIRCULAR);
    lv_coord_t y_max = Y_MAX_VALUE;
    lv_coord_t y_min = Y_MIN_VALUE;
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, y_min, y_max);

    /*Add two data series*/
    lv_chart_series_t * ser1 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    //lv_chart_series_t * ser2 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_SECONDARY_Y);

    for (int i = 0; i < CYCLES; i++)
    {
      for (int i = 0; i < TABLE_SIZE; i++)
      {
        lv_chart_set_next_value(chart, ser1, (samples[i]*100));
      }
    }
    
}


void gui_init (void) {
    draw_example_chart();
}