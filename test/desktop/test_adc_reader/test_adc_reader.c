#include "unity.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


/*HELPER FUNCTIONS*/
static time_t t;
#define RES_VAL 0.8058f

uint16_t get_random_raw_value () {
    srand((unsigned) time(&t));
    return (uint16_t)rand() % 4096;
}


int adc_calc_volt(int16_t data) {
    return (data*RES_VAL);
}
/*END OF HELPER FUNCTIONS*/

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_should_check_mid_voltage_value(void) {
  //given
  int16_t raw = 2043;
  int voltage;

  //when
  voltage = adc_calc_volt(raw);

  //then
  TEST_ASSERT_EQUAL_MESSAGE(1646, voltage, "Max voltage value should be 1646 mV");
}

void test_should_check_max_voltage_value (void) {
  //given
  int16_t raw = 4096;
  int voltage;

  //when
  voltage = adc_calc_volt(raw);

  //then
  TEST_ASSERT_EQUAL_MESSAGE(3300, voltage, "Max voltage value should be 3300 mV");
}

void test_function_should_check_volt_is_not_zero(void) {
  //given
  int16_t data = get_random_raw_value();
  TEST_ASSERT_NOT_EQUAL(0, data);
  int voltage;

  //when
  voltage = adc_calc_volt(data);

  //then
  TEST_ASSERT_NOT_EQUAL_MESSAGE(0, voltage, "Volt data shouldn't be zero.");
}


/*To add test use: RUN_TEST(test_name) macro.*/
int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_function_should_check_volt_is_not_zero);
  RUN_TEST(test_should_check_max_voltage_value);
  RUN_TEST(test_should_check_mid_voltage_value);
  return UNITY_END();
}


/**
  * main function for native dev-platform
  */
int main(void) {
  runUnityTests();
}