find_library(MATH_LIBRARY m)
if(MATH_LIBRARY)
    target_link_libraries(MyTarget PUBLIC ${MATH_LIBRARY})
endif()



idf_component_register(SRCS "lvgl_gui.c" INCLUDE_DIRS "include" REQUIRES "lvgl")
