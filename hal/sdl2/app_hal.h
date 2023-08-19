#ifndef DRIVER_H
#define DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Setup and initializing of SDL.
 * 
 */
void hal_setup(void);

/**
 * @brief Loop running lv_task_handler for SDL.
 * 
 */
void hal_loop(void);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*DRIVER_H*/
