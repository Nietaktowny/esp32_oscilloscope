#pragma once


void gui_init (void);

void generate_example_values(void);

lv_obj_t* gui_get_chart (void);
lv_chart_series_t* gui_get_ser1 (void);


void change_number_of_division_lines (uint8_t h_div, uint8_t v_div);