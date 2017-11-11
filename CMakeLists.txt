cmake_minimum_required(VERSION 3.9)
project(estacao C)

set(CMAKE_C_STANDARD 11)
set(RASP_GCC_PATH /home/davi/bin/gcc-linaro-6.3.1-2017.05-x86_64_arm-linux-gnueabihf)
set(CUSTOM_INCLUDE ${RASP_GCC_PATH}/usr/src)

add_compile_options(-Wall -Wno-missing-noreturn -Wno-unknown-pragmas)

#add_compile_options(-DAUTH)
#add_compile_options(-DDEBUG)
set(THREADS_PREFER_PTHREAD_FLAG ON)

include_directories(${CUSTOM_INCLUDE}/wiringPi-96344ff/wiringPi)
include_directories(${CUSTOM_INCLUDE}/paho.mqtt.c-master/src)

add_executable(estacao main.c umidade.c umidade.h mqtt.c mqtt.h temperatura.c temperatura.h pressao.c pressao.h vento.c vento.h config.h)

find_library(WIRINGPI NAMES wiringPi PATHS ${RASP_GCC_PATH}/lib NO_DEFAULT_PATH)
find_library(PAHO NAMES "paho-mqtt3c" PATHS ${RASP_GCC_PATH}/lib NO_DEFAULT_PATH)
target_link_libraries(estacao PUBLIC ${WIRINGPI} pthread ${PAHO})
