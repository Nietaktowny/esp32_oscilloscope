/**
 * @file esp32_oscilloscope.c
 * @author Wojciech Mytych
 * @brief main function of project.
 * @version 0.1
 * @date 2023-08-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "lvgl.h"
#include "app_hal.h"
#include <stdio.h>
#include <stdint.h>
#include "lvgl_gui.h"



#ifdef ESP_PLATFORM
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_task_wdt.h"
#include "esp_system.h"
#include "esp_adc_reader.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

static const char *GUI = "gui task";        ///< This is logging tag used for logs related to lvgl_gui library.
static const char *ADC = "adc task";        ///< This is logging tag used for logs related to esp_adc_reader library.
static const char *HAL = "hal task";        ///< This is logging tag used for logs related to esp_adc_reader library.
static const char *INFO = "info task";      ///< This is logging tag used for logs related to esp_adc_reader library.

static QueueHandle_t read_values;           ///< Queue  used to pass ADC data from reading task to GUI task.
static TaskHandle_t adc_task_handle;
static TaskHandle_t gui_task_handle;

//#define GENERATE_RUN_TIME_STATS             // Uncomment this to generate runtime stats. Works only if CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS is enabled in menuconfig.


#ifdef GENERATE_RUN_TIME_STATS
#define NUM_OF_SPIN_TASKS   6
#define SPIN_ITER           500000  //Actual CPU cycles used will depend on compiler optimization
#define SPIN_TASK_PRIO      2
#define STATS_TASK_PRIO     3
#define STATS_TICKS         pdMS_TO_TICKS(1000)
#define ARRAY_SIZE_OFFSET   5   //Increase this if print_real_time_stats returns ESP_ERR_INVALID_SIZE
static char task_names[NUM_OF_SPIN_TASKS][configMAX_TASK_NAME_LEN];
static SemaphoreHandle_t sync_spin_task;
static SemaphoreHandle_t sync_stats_task;
/**
 * @brief   Function to print the CPU usage of tasks over a given duration.
 *
 * This function will measure and print the CPU usage of tasks over a specified
 * number of ticks (i.e. real time stats). This is implemented by simply calling
 * uxTaskGetSystemState() twice separated by a delay, then calculating the
 * differences of task run times before and after the delay.
 *
 * @note    If any tasks are added or removed during the delay, the stats of
 *          those tasks will not be printed.
 * @note    This function should be called from a high priority task to minimize
 *          inaccuracies with delays.
 * @note    When running in dual core mode, each core will correspond to 50% of
 *          the run time.
 *
 * @param   xTicksToWait    Period of stats measurement
 *
 * @return
 *  - ESP_OK                Success
 *  - ESP_ERR_NO_MEM        Insufficient memory to allocated internal arrays
 *  - ESP_ERR_INVALID_SIZE  Insufficient array size for uxTaskGetSystemState. Trying increasing ARRAY_SIZE_OFFSET
 *  - ESP_ERR_INVALID_STATE Delay duration too short
 */
static esp_err_t print_real_time_stats(TickType_t xTicksToWait)
{
    TaskStatus_t *start_array = NULL, *end_array = NULL;
    UBaseType_t start_array_size, end_array_size;
    uint32_t start_run_time, end_run_time;
    esp_err_t ret;

    //Allocate array to store current task states
    start_array_size = uxTaskGetNumberOfTasks() + ARRAY_SIZE_OFFSET;
    start_array = malloc(sizeof(TaskStatus_t) * start_array_size);
    if (start_array == NULL) {
        ret = ESP_ERR_NO_MEM;
        goto exit;
    }
    //Get current task states
    start_array_size = uxTaskGetSystemState(start_array, start_array_size, &start_run_time);
    if (start_array_size == 0) {
        ret = ESP_ERR_INVALID_SIZE;
        goto exit;
    }

    vTaskDelay(xTicksToWait);

    //Allocate array to store tasks states post delay
    end_array_size = uxTaskGetNumberOfTasks() + ARRAY_SIZE_OFFSET;
    end_array = malloc(sizeof(TaskStatus_t) * end_array_size);
    if (end_array == NULL) {
        ret = ESP_ERR_NO_MEM;
        goto exit;
    }
    //Get post delay task states
    end_array_size = uxTaskGetSystemState(end_array, end_array_size, &end_run_time);
    if (end_array_size == 0) {
        ret = ESP_ERR_INVALID_SIZE;
        goto exit;
    }

    //Calculate total_elapsed_time in units of run time stats clock period.
    uint32_t total_elapsed_time = (end_run_time - start_run_time);
    if (total_elapsed_time == 0) {
        ret = ESP_ERR_INVALID_STATE;
        goto exit;
    }

    printf("| Task | Run Time | Percentage\n");
    //Match each task in start_array to those in the end_array
    for (int i = 0; i < start_array_size; i++) {
        int k = -1;
        for (int j = 0; j < end_array_size; j++) {
            if (start_array[i].xHandle == end_array[j].xHandle) {
                k = j;
                //Mark that task have been matched by overwriting their handles
                start_array[i].xHandle = NULL;
                end_array[j].xHandle = NULL;
                break;
            }
        }
        //Check if matching task found
        if (k >= 0) {
            uint32_t task_elapsed_time = end_array[k].ulRunTimeCounter - start_array[i].ulRunTimeCounter;
            uint32_t percentage_time = (task_elapsed_time * 100UL) / (total_elapsed_time * portNUM_PROCESSORS);
            printf("| %s | %"PRIu32" | %"PRIu32"%%\n", start_array[i].pcTaskName, task_elapsed_time, percentage_time);
        }
    }

    //Print unmatched tasks
    for (int i = 0; i < start_array_size; i++) {
        if (start_array[i].xHandle != NULL) {
            printf("| %s | Deleted\n", start_array[i].pcTaskName);
        }
    }
    for (int i = 0; i < end_array_size; i++) {
        if (end_array[i].xHandle != NULL) {
            printf("| %s | Created\n", end_array[i].pcTaskName);
        }
    }
    ret = ESP_OK;

exit:    //Common return path
    free(start_array);
    free(end_array);
    return ret;
}

static void spin_task(void *arg)
{
    xSemaphoreTake(sync_spin_task, portMAX_DELAY);
    while (1) {
        //Consume CPU cycles
        for (int i = 0; i < SPIN_ITER; i++) {
            __asm__ __volatile__("NOP");
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

static void stats_task(void *arg)
{
    xSemaphoreTake(sync_stats_task, portMAX_DELAY);

    //Start all the spin tasks
    for (int i = 0; i < NUM_OF_SPIN_TASKS; i++) {
        xSemaphoreGive(sync_spin_task);
    }

    //Print real time stats periodically
    while (1) {
        printf("\n\nGetting real time stats over %"PRIu32" ticks\n", STATS_TICKS);
        if (print_real_time_stats(STATS_TICKS) == ESP_OK) {
            printf("Real time stats obtained\n");
        } else {
            printf("Error getting real time stats\n");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

#endif

/**
 * @brief Task running hal loop.
 * It's responsible for calling lv_timer_handler();
 * @param args Additional arguments passed to task.
 */
void hal_task(void* args) {
    while(1) {
        hal_loop();
    }
}

/**
 * @brief Task running GUI.
 * 
 * It receives ADC voltage values from Queue and draws them on chart.
 * 
 * @param args Additional arguments passed to task.
 */
void gui_task (void* args) {
    uint32_t buf = 0;
    BaseType_t xStatus;
    while (1)
    {

        xStatus = xQueueReceive(read_values, &buf, pdMS_TO_TICKS(20));
        
        if(xStatus == pdTRUE) {
            gui_set_point((int16_t)(buf));
        }
        vTaskDelay(pdMS_TO_TICKS(15));
    }
    
}

/**
 * @brief Task reading ADC values.
 * 
 * @param args Additional arguments passed to task.
 */
void adc_task (void* args) {
    adc_reader_init(read_values);
    adc_reader_loop();
}

/**
 * @brief Entry point of application for ESP-IDF framework.
 * 
 */
void app_main(void)
{
    lv_init();

    hal_setup();

    read_values = xQueueCreate(10, sizeof(int));

    gui_init();

    ESP_LOGI(GUI, "Starting hal task...");
    xTaskCreate(hal_task, "hal_task", 4096, NULL, 1, NULL);
    ESP_LOGI(GUI, "Starting gui task...");
    xTaskCreatePinnedToCore(gui_task, "gui_task", 4096, NULL, 1, &gui_task_handle, 1);
    ESP_LOGI(ADC, "Starting adc task...");
    xTaskCreate(adc_task, "adc_task", 4096, NULL, 1, &adc_task_handle);

    #ifdef GENERATE_RUN_TIME_STATS
    ESP_LOGI(INFO, "Starting info task...");
    //Create semaphores to synchronize
    sync_spin_task = xSemaphoreCreateCounting(NUM_OF_SPIN_TASKS, 0);
    sync_stats_task = xSemaphoreCreateBinary();

    //Create spin tasks
    for (int i = 0; i < NUM_OF_SPIN_TASKS; i++) {
        snprintf(task_names[i], configMAX_TASK_NAME_LEN, "spin%d", i);
        xTaskCreatePinnedToCore(spin_task, task_names[i], 1024, NULL, SPIN_TASK_PRIO, NULL, tskNO_AFFINITY);
    }

    //Create and start stats task
    xTaskCreatePinnedToCore(stats_task, "stats", 4096, NULL, STATS_TASK_PRIO, NULL, tskNO_AFFINITY);
    xSemaphoreGive(sync_stats_task);
    #endif
}


#elif (USE_SDL)
/**
 * @brief Entry point of application for native framework.
 * 
 * @return int 0 on success
 */
int main(void)
{
	lv_init();

	hal_setup();

    generate_example_values();

    gui_init();

    generate_example_sin_wave();

	hal_loop();
}
#endif


