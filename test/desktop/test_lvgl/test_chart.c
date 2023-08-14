#ifdef USE_SDL
#include "unity.h"
#include "lvgl.h"
#include "app_hal.h"

static lv_obj_t * active_screen = NULL;
static lv_obj_t * chart = NULL;

static lv_color_t red_color;

void setUp(void) {
  	lv_init();
	hal_setup();

    active_screen = lv_scr_act();
    chart = lv_chart_create(active_screen);

    red_color = lv_palette_main(LV_PALETTE_RED);
}

void tearDown(void) {
  // clean stuff up here
}


void test_chart_add_series(void)
{
    lv_chart_series_t * red_series;

    red_series = lv_chart_add_series(chart, red_color, LV_CHART_AXIS_SECONDARY_Y);

    TEST_ASSERT_NOT_NULL_MESSAGE(red_series, "Red series not added to chart");
    //TEST_ASSERT_NULL_MESSAGE(red_series->x_points, "X points in non scatter chart should not point to anything");

    lv_chart_remove_series(chart, red_series);
}

void test_chart_set_point_count_increments(void)
{
    lv_chart_series_t * red_series;
    red_series = lv_chart_add_series(chart, red_color, LV_CHART_AXIS_SECONDARY_Y);

    uint16_t points_in_serie = lv_chart_get_point_count(chart);
    uint16_t new_point_count = points_in_serie * 2;
    lv_chart_set_point_count(chart, new_point_count);

    TEST_ASSERT_EQUAL_MESSAGE(new_point_count, lv_chart_get_point_count(chart),
                              "Actual points in chart are less than expected");
    lv_chart_remove_series(chart, red_series);
}

void test_chart_set_point_count_decrements(void)
{
    lv_chart_series_t * red_series;
    red_series = lv_chart_add_series(chart, red_color, LV_CHART_AXIS_SECONDARY_Y);
    uint16_t points_in_serie = lv_chart_get_point_count(chart);
    uint16_t new_point_count = points_in_serie / 2;

    lv_chart_set_point_count(chart, new_point_count);

    TEST_ASSERT_EQUAL_MESSAGE(new_point_count, lv_chart_get_point_count(chart),
                              "Actual points in chart are more than expected");
    lv_chart_remove_series(chart, red_series);
}

void test_chart_set_point_count_as_same(void)
{
    lv_chart_series_t * red_series;
    red_series = lv_chart_add_series(chart, red_color, LV_CHART_AXIS_SECONDARY_Y);
    uint16_t points_in_serie = lv_chart_get_point_count(chart);
    uint16_t new_point_count = points_in_serie;

    lv_chart_set_point_count(chart, new_point_count);

    TEST_ASSERT_EQUAL_MESSAGE(new_point_count, lv_chart_get_point_count(chart),
                              "Actual points is not equal to original point count");
    lv_chart_remove_series(chart, red_series);
}

void test_chart_set_new_point_count_as_zero(void)
{
    lv_chart_series_t * red_series;
    red_series = lv_chart_add_series(chart, red_color, LV_CHART_AXIS_SECONDARY_Y);

    lv_chart_set_point_count(chart, 0u);

    TEST_ASSERT_EQUAL_MESSAGE(1u, lv_chart_get_point_count(chart), "Actual points in chart are more than 1");
    lv_chart_remove_series(chart, red_series);
}

void test_chart_point_is_added_at_the_end_of_a_serie(void)
{
    lv_chart_series_t * red_series;
    red_series = lv_chart_add_series(chart, red_color, LV_CHART_AXIS_SECONDARY_Y);

    TEST_ASSERT_NOT_NULL_MESSAGE(red_series, "Red series not added to chart");

    lv_chart_remove_series(chart, red_series);
}

void test_chart_one_point_when_setting_point_count_to_zero(void)
{
    lv_chart_set_point_count(chart, 0u);
    TEST_ASSERT_EQUAL(1u, lv_chart_get_point_count(chart));
}


/*To add test use: RUN_TEST(test_name) macro.*/
int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_chart_add_series);
  RUN_TEST(test_chart_set_point_count_increments);
  RUN_TEST(test_chart_set_point_count_decrements);
  RUN_TEST(test_chart_set_point_count_as_same);
  RUN_TEST(test_chart_set_new_point_count_as_zero);
  RUN_TEST(test_chart_point_is_added_at_the_end_of_a_serie);
  RUN_TEST(test_chart_one_point_when_setting_point_count_to_zero);
  return UNITY_END();
}


/**
  * main function for native dev-platform
  */
int main(void) {
  runUnityTests();
}

#endif