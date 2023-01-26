#include "test.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>

void test(){
    printf(".");
    vTaskDelay(1000/portTICK_RATE_MS);
}