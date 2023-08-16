#include "unity.h"
#include "lvgl.h"
#include "tests.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "lvgl_gui.h"

/*DEFINES*/

#define TABLE_SIZE 500
static float samples [TABLE_SIZE];
#define CYCLES 6
#define TWO_PI (3.141592653589793238 * 2)
#define Y_MAX_VALUE 100

static lv_obj_t * chart;
static lv_chart_series_t * ser1;
static int r;
static time_t t;

/*END OF DEFINES*/

/*HELPER FUNCTIONS*/
void generate_example_sin_wave_values(void) {
  
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
        lv_chart_set_next_value(gui_get_chart(), gui_get_ser1(), (samples[i]*Y_MAX_VALUE));
      }
    }
}

int get_random_value (int range) {
    srand((unsigned) time(&t));
    return (rand() % range);
}
/*END OF HELPER FUNCTIONS*/

/*SETUP BEFORE TESTS*/

void test_lvgl_gui_setup(void) {
    gui_init();
    chart = gui_get_chart();
    ser1 = gui_get_ser1();
}

/******************************************TESTS************************************************/

void test_series_one_color(void) {
    //given
    lv_color_t expected_color;
    uint16_t actual_color_16, expected_color_16;

    //when
    actual_color_16 = ser1->color.full;
    expected_color = lv_color_hex(HEX_SERIES_ONE_COLOR);
    expected_color_16 = expected_color.full;

    //then
    TEST_ASSERT_EQUAL_MESSAGE(expected_color_16, actual_color_16, "Series one color different from expected.");
}

void test_check_chart_background_color (void) {
    //given
    lv_color_t actual_color, expected_color;
    uint16_t actual_color_16, expected_color_16;

    //when
    actual_color = lv_obj_get_style_bg_color(chart, 0);
    expected_color = lv_color_hex(HEX_CHART_BACKGROUND_COLOR);
    //change to uint16
    actual_color_16 = actual_color.full;
    expected_color_16 = expected_color.full;

    //then
    TEST_ASSERT_EQUAL_MESSAGE(expected_color_16, actual_color_16, "Background color different from expected.");
}

void test_chart_update_mode_is_circular (void) {
    //then
    TEST_ASSERT_EQUAL_MESSAGE(LV_CHART_UPDATE_MODE_CIRCULAR, ((lv_chart_t *)chart)->update_mode, "Chart update mode different from circular.");
}

void test_if_changed_y_max_axis_range (void) {
    //given
    int16_t exp_min, exp_max;
    exp_min = get_random_value(3200);
    exp_max = get_random_value(5000);

    //when
    gui_set_chart_y_axis_range(exp_min, exp_max);

    //then
    TEST_ASSERT_EQUAL_MESSAGE(exp_max, ((lv_chart_t *)chart)->ymax[0], "Didn't change y_max value of chart."); 
}

void test_if_changed_y_min_axis_range (void) {
    //given
    int16_t exp_min, exp_max;
    exp_min = get_random_value(3200);
    exp_max = get_random_value(100);

    //when
    gui_set_chart_y_axis_range(exp_min, exp_max);

    //then
    TEST_ASSERT_EQUAL_MESSAGE(exp_min, ((lv_chart_t *)chart)->ymin[0], "Didn't change y_min value of chart."); 
}

void test_if_changed_number_of_vertical_division_lines(void) {
    //given
    uint_fast8_t exp_h, exp_v;
    exp_h = get_random_value(250);
    exp_v = get_random_value(250);

    //when
    gui_set_number_of_division_lines(exp_h, exp_v);

    //then
    TEST_ASSERT_EQUAL_MESSAGE(exp_v, ((lv_chart_t *)chart)->vdiv_cnt, "Wrong number of vertical division lines.");
}

void test_if_changed_number_of_horizontal_division_lines(void) {
    //given
    uint_fast8_t exp_h, exp_v;
    exp_h = get_random_value(250);
    exp_v = get_random_value(250);

    //when
    gui_set_number_of_division_lines(exp_h, exp_v);

    //then
    TEST_ASSERT_EQUAL_MESSAGE(exp_h, ((lv_chart_t *)chart)->hdiv_cnt, "Wrong number of horizontal division lines.");
}

void test_if_ser1_starts_from_zero (void) {
    //given
    uint16_t expected, actual;
    expected = 0;
    //when
    actual = lv_chart_get_x_start_point(chart, ser1);
    //then
    TEST_ASSERT_EQUAL_MESSAGE(expected, actual, "Starting point for series 1 different from 0");
}

void test_if_changed_point_count(void) {
    //given
    uint16_t expected = get_random_value(RAND_MAX);
    uint16_t actual;

    //when
    gui_set_chart_point_count(expected);
    actual = lv_chart_get_point_count(chart);

    //then
    TEST_ASSERT_EQUAL_MESSAGE(expected, actual, "Number of point counts vary from expected.");
}

void test_if_added_series(void) {
    //given
    lv_chart_series_t * test_series;
    
    //when
    test_series = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_YELLOW), LV_CHART_AXIS_PRIMARY_Y);

    //then
    TEST_ASSERT_NOT_NULL_MESSAGE(test_series, "Pointer from lv_chart_add_series is null");
}

void test_chart_height (void) {
    //given
    lv_coord_t h = 240;
    //then
    TEST_ASSERT_EQUAL_MESSAGE(h, CHART_HEIGHT, "Wrong chart height");
}

void test_chart_width (void) {
    //given
    lv_coord_t w = 320;
    //then
    TEST_ASSERT_EQUAL_MESSAGE(w, CHART_WIDTH, "Wrong chart width");
}

void test_chart_type(void) {
    //then
    TEST_ASSERT_EQUAL_MESSAGE(LV_CHART_TYPE_LINE, lv_chart_get_type(chart), "Chart type not line.");
}

void test_if_ser1_is_null (void) {
    //then
    TEST_ASSERT_NOT_NULL_MESSAGE(gui_get_ser1(), "Series pointer from function is null");
    TEST_ASSERT_NOT_NULL_MESSAGE(ser1, "Series pointer is null");
}

void test_if_chart_is_null (void) {
    //then
    TEST_ASSERT_NOT_NULL_MESSAGE(gui_get_chart(), "Chart object from function is null");
    TEST_ASSERT_NOT_NULL_MESSAGE(chart, "Chart object is null");
}

void test_if_points_number_is_bigger_than_table_size(void) {
    //given
    uint16_t point_count = lv_chart_get_point_count(gui_get_chart());

    //then
    TEST_ASSERT_MESSAGE(point_count>TABLE_SIZE, "Table size bigger than points number in lvgl_gui.c");
}

/**************************************END OF TESTS*********************************************/


/*To add test use: RUN_TEST(test_name) macro.*/
int run_lvgl_gui_tests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_chart_type);
  RUN_TEST(test_if_ser1_is_null);
  RUN_TEST(test_if_points_number_is_bigger_than_table_size);
  RUN_TEST(test_if_chart_is_null);
  RUN_TEST(test_chart_height);
  RUN_TEST(test_chart_width);
  RUN_TEST(test_if_added_series);
  RUN_TEST(test_if_changed_point_count);
  RUN_TEST(test_if_ser1_starts_from_zero);
  RUN_TEST(test_if_changed_number_of_horizontal_division_lines);
  RUN_TEST(test_if_changed_number_of_vertical_division_lines);
  RUN_TEST(test_if_changed_y_min_axis_range);
  RUN_TEST(test_if_changed_y_max_axis_range);
  RUN_TEST(test_chart_update_mode_is_circular);
  RUN_TEST(test_check_chart_background_color);
  RUN_TEST(test_series_one_color);
  return UNITY_END();
}