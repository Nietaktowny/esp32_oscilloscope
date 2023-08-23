#pragma once

#define POINTS_NUMBER                   500
#define Y_MIN_VALUE                     -5000
#define Y_MAX_VALUE                     5000
/**
 * @file lvgl_gui.h
 * @author Wojciech Mytych
 * @brief Main header file of lvgl_gui library.
 * @version 0.1
 * @date 2023-08-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#define V_DIVISION_LINES                8           ///< Basic number of vertical division lines when first initialing chart.
#define H_DIVISION_LINES                8           ///< Basic number of horizontal division lines when first initialing chart.
#define CHART_WIDTH                     320
#define CHART_HEIGHT                    240
#define HEX_CHART_BACKGROUND_COLOR      0x1d1f1d    ///< Chart background color - basic setup is black.
#define HEX_SERIES_ONE_COLOR            0xf5e042    ///< First series color - basic setup is yellow.

/**
 * @brief Entry point of lvgl_gui library.
 * This function should be called before any other from libary,
 * as this is where all objects are initialized.
 */
void gui_init (void);

/**
 * @brief Generates values to generate example sin wave.
 * 
 */
void generate_example_values(void);

/**
 * @brief Generates example sin wave.
 * 
 */
void generate_example_sin_wave(void);

/**
 * @brief Getter function used to get chat object.
 * 
 * @return lv_obj_t* Chart object.
 */
lv_obj_t* gui_get_chart (void);

/**
 * @brief Getter function used to get first series object.
 * 
 * @return lv_chart_series_t* First series object.
 */
lv_chart_series_t* gui_get_ser1 (void);

/**
 * @brief Changes number of horizontal and vertical division lines.
 * 
 * @param h_div Number of horizontal division lines.
 * @param v_div Number of vertical division lines.
 */
void gui_set_number_of_division_lines (uint8_t h_div, uint8_t v_div);

/**
 * @brief Changes number of points on x axis.
 * 
 * @param count Number of points on x axis.
 */
void gui_set_chart_point_count(uint16_t count);

/**
 * @brief Change range of points on y axis.
 * 
 * @param y_min Min range of y axis.
 * @param y_max Max range of y axis.
 */
void gui_set_chart_y_axis_range (int16_t y_min, int16_t y_max);

/**
 * @brief Set next point on chart.
 * 
 * @param value Value of point on y axis.
 */
void gui_set_point (int16_t value);