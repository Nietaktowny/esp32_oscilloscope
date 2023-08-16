#ifdef USE_SDL
#include "unity.h"
#include "lvgl.h"
#include "tests.h"

#include "lvgl_gui.h"

/*DEFINES FOR HELPER FUNCTIONS*/

#define TABLE_SIZE 500
float samples [TABLE_SIZE];
#define CYCLES 6
#define TWO_PI (3.141592653589793238 * 2)

/*END OF DEFINEF FOR HELPER FUNCTIONS*/

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

/*END OF HELPER FUNCTIONS*/

void test_function_should_doBlahAndBlah(void) {
  UnityAssertEqualNumber(1, 1, "TEST TEST", 12, UNITY_DISPLAY_STYLE_INT16);
}

/*To add test use: RUN_TEST(test_name) macro.*/
int run_lvgl_gui_tests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_function_should_doBlahAndBlah);
  return UNITY_END();
}
#endif