/**
 * @file lvgl_gui.c
 * @author Wojciech Mytych
 * @brief Main source file of lvgl_gui library.
 * @version 1.0
 * @date 2023-08-18
 * 
 * @copyright Copyright (c) 2023
 * 
 * @todo Add error checking to library.
 * 
 */
#include "lvgl.h"
#include "lvgl_gui.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

/**********************************************************STATIC FUNCTIONS PROTOTYPES***********************************************************************************/
static void gui_create_chart (void);
static void gui_create_series (void);

/**************************************************************************************************************************************************************************/

#define TABLE_SIZE 100

static float samples [TABLE_SIZE];
#define CYCLES 5
#define TWO_PI (3.141592653589793238 * 2)



static lv_obj_t * chart;          ///< Object used to reference chart.
static lv_chart_series_t * ser1;  ///< Object used to reference first series.
static lv_obj_t * active_screen;  ///< Object used to reference currently active screen.


void gui_set_number_of_division_lines (uint8_t h_div, uint8_t v_div) {
  if (h_div > 0 && v_div > 0)
  {
    lv_chart_set_div_line_count(chart, h_div, v_div);
  } else {
    printf("Wrong number for division lines.");
    ///TODO Macro for logging based on framework (native/esp-idf)
  }
}

void gui_set_chart_point_count(uint16_t count) {
  lv_chart_set_point_count(chart, count);
}

void gui_set_chart_y_axis_range (int16_t y_min, int16_t y_max) {
  lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y,(lv_coord_t) y_min,(lv_coord_t) y_max);
}

void gui_set_point (int16_t value) {
  lv_chart_set_next_value(chart, ser1, value);
}

void gui_set_ticks(int16_t major_ticks, int16_t minor_ticks) {
  lv_obj_set_style_text_font(chart, &lv_font_montserrat_8, 0);
  lv_obj_set_style_line_color(chart, lv_color_hex(HEX_TICKS_COLOR), LV_PART_TICKS);
  lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_Y, 4, 2,(lv_coord_t) major_ticks,(lv_coord_t) minor_ticks, true, 35);
}

lv_obj_t* gui_get_chart (void) {
  return chart;
}

lv_chart_series_t* gui_get_ser1 (void) {
  return ser1;
}

/**
 * @brief Function used to create and initialize chart.
 * Here are set base properties of chart and chart object is created.
 * @note This function should be called first before any other functions modifying the chart object. 
 */
static void gui_create_chart (void) {

    /*Create a chart*/
    chart = lv_chart_create(active_screen);
    LV_ASSERT_NULL(chart);
    lv_obj_set_size(chart, CHART_WIDTH, CHART_HEIGHT);
    lv_obj_center(chart);
    lv_obj_set_pos(chart, 15, 0);
    lv_obj_set_style_bg_color(chart, lv_color_hex(HEX_CHART_BACKGROUND_COLOR), 0);  /*Change chart background color to HEX_CHART_BACKGROUND_COLOR*/
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);                                   /*Show lines and points too*/
    gui_set_chart_point_count(POINTS_NUMBER);                                       /*Change number of points on x axis*/
    lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_CIRCULAR);                 /*Circularly add the new data*/
    gui_set_number_of_division_lines(H_DIVISION_LINES, V_DIVISION_LINES);           /*Change number of division lines*/
    gui_set_ticks(9, 4);                                                            /*Set ticks on Y axis.*/
    gui_set_chart_y_axis_range(Y_MIN_VALUE, Y_MAX_VALUE);                           /*Change number of points on y axis*/
}

/**
 * @brief Function used to create and initialize data series on chart.
 * Here are set base properties of series such as color, and axis.
 * @note This function should be called first before any other functions modifying the series objects.
 */
static void gui_create_series (void) {
    /*Add one data serie*/
    ser1 = lv_chart_add_series(chart, lv_color_hex(HEX_SERIES_ONE_COLOR), LV_CHART_AXIS_PRIMARY_Y);
}

void generate_example_values(void) {
  
  float phaseIncrement = TWO_PI/TABLE_SIZE;
  float currentPhase = 0.0;
  int i;

  for (i = 0; i < TABLE_SIZE; i ++) {
    samples[i] = sin(currentPhase);
    currentPhase += phaseIncrement;
  }
}

void generate_example_sin_wave(void) {
      for (int i = 0; i < CYCLES; i++)
    {
      for (int i = 0; i < TABLE_SIZE; i++)
      {
        gui_set_point(samples[i]*3300);
      }
    }
}

void gui_init (void) {
    active_screen = lv_scr_act();
    LV_ASSERT_NULL(active_screen);
    lv_obj_set_style_bg_color(active_screen, lv_color_hex(HEX_CHART_BACKGROUND_COLOR), 0);

    gui_create_chart();
    LV_ASSERT_NULL(chart);

    gui_create_series();
    LV_ASSERT_NULL(ser1);
}