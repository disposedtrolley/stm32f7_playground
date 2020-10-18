cmake_minimum_required(VERSION 3.13.1)

set(BOARD nucleo_f767zi)
set(DTC_OVERLAY_FILE nucleo_f767zi.overlay)

find_package(Zephyr)
project(stm32f7_playground)

target_sources(app PRIVATE
  src/main.cpp)