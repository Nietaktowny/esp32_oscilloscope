#pragma once

#define POINTS_NUMBER 3001
#define Y_MIN_VALUE -100
#define Y_MAX_VALUE 100
#define V_DIVISION_LINES 8
#define H_DIVISION_LINES 8
#define CHART_WIDTH 320
#define CHART_HEIGHT 240
#define HEX_CHART_BACKGROUND_COLOR 0x1d1f1d
#define HEX_SERIES_ONE_COLOR 0xf5e042

void gui_init(void);

void generate_example_values(void);

lv_obj_t *gui_get_chart(void);
lv_chart_series_t *gui_get_ser1(void);

void gui_set_number_of_division_lines(uint8_t h_div, uint8_t v_div);
void gui_set_chart_point_count(uint16_t count);
void gui_set_chart_y_axis_range(int16_t y_min, int16_t y_max);