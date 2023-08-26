#ifndef DRIVER_H
#define DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Setup and initializing LCD for ILI9341
 * 
 */
int hal_setup(void);

/**
 * @brief Loop running lv_timer_handler.
 * 
 */
void hal_loop(void);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*DRIVER_H*/
