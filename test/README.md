# Tests overview

Tests are made using Unity and CMock frameworks.
Refer to:
 - [CMock](https://www.throwtheswitch.org/cmock)
 - [CMock github](https://github.com/ThrowTheSwitch/CMock/tree/master)
 - [Unity](https://www.throwtheswitch.org/unity)
 - [Unity github](https://github.com/ThrowTheSwitch/Unity)
 - [Unity in PlatformIO](https://docs.platformio.org/en/stable/advanced/unit-testing/frameworks/unity.html)

## Structure

This directory is intended for PlatformIO Test Runner and project tests.

Folder structure: 

    ├───desktop
    │   ├───test_lcd_controller
    │   └───test_main
    └───embedded
        ├───test_lcd_controller
        └───test_main

## Classification of tests
Main distinction is made between **desktop** and **embedded** tests.
**Desktop** tests are running in native platform. 
**Embedded** tests are running on hardware, in Espressif platform.

Each approach is configured separately in it's own PlatformIO configuration environment. (`native` for desktop, and `esp-wrover-kit-test` for embedded).

## Single test structure
Tests are made according to the convention below: 

    void test_function_should_connect_to_wifi(void) {
      //given
      prepare_values();
    
      //when
      int retval = call_fun_and_store_val_to_test(prepared_values_t);
    
      //then
      UnityAssertSomething(expected, retval, "NAME", LINE, UNITY_DISPLAY_STYLE);
    }
## Test file structure
Test files are structured as described in PlatformIO [documentation](https://docs.platformio.org/en/stable/advanced/unit-testing/frameworks/unity.html).

Example: 

    #include "unity.h"
    #include "Mockesp_wifi_manager.h"
    
    void setUp(void) {
      // set stuff up here
    }
    
    void tearDown(void) {
      // clean stuff up here
    }
    
    void test_function_should_connect_to_wifi(void) {
      //given
      wifi_manager_connect_ExpectAndReturn(1, 2);
    
      //when
      int retval = wifi_manager_connect(1);
    
      //then
      UnityAssertEqualNumber(2, retval, "TEST LIB", 18, UNITY_DISPLAY_STYLE_INT16);
    }
    
    
    /*To add test use: RUN_TEST(test_name) macro.*/
    int runUnityTests(void) {
      UNITY_BEGIN();
      RUN_TEST(test_function_should_connect_to_wifi);
      return UNITY_END();
    }
    
    
    /**
      * For ESP-IDF framework
      */
    void app_main(void) {
      runUnityTests();
    }

The difference between desktop and embedded tests here is only in main function:

ESP-IDF:

    /**
      * For ESP-IDF framework
      */
    void app_main(void) {
      runUnityTests();
    }

Native:

    /**
      * main function for native dev-platform
      */
    int main(void) {
      runUnityTests();
    }

## List of TODOs

 - [x] Preparing use structure of Unity for unit tests
 - [x] Preparing use of CMock for main app and libraries
 - [x] Preparing basic README for test directory
 - [x] Implement soft reset after embedded tests
 - [ ] Using TDD