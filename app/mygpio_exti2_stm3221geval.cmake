include(${PROJECT_UBINOS_DIR}/config/ubinos_stm3221geval.cmake)

include(${PROJECT_LIBRARY_DIR}/stm32cubef2/config/stm32cubef2.cmake)

####

set(INCLUDE__APP TRUE)
set(APP__NAME "mygpio_exti2")

get_filename_component(_tmp_source_dir "${CMAKE_CURRENT_LIST_DIR}/mygpio_exti2" ABSOLUTE)

file(GLOB_RECURSE _tmp_sources
    "${_tmp_source_dir}/*.c"
    "${_tmp_source_dir}/*.cpp"
    "${_tmp_source_dir}/*.S"
    "${_tmp_source_dir}/*.s")

set(PROJECT_APP_SOURCES ${PROJECT_APP_SOURCES} ${_tmp_sources})

get_filename_component(_tmp_source_dir "${PROJECT_LIBRARY_DIR}/stm32cubef2/source/STM32Cube_FW_F2/Projects/STM322xG_EVAL/Examples/GPIO/GPIO_EXTI" ABSOLUTE)

include_directories(${_tmp_source_dir}/Inc)

