#include <stdio.h>
#include <esp_log.h>
#include <freertos_debug_helper.h>

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include <freertos/timers.h>
#include "test.h"

QueueHandle_t xQueue;
SemaphoreHandle_t xSemaphore;
TimerHandle_t xTimer;
TimerHandle_t xTimerCrash;

void vTimerCallback(TimerHandle_t xTimer)
{
    printf("*");
}

void vFailTimerCallback(TimerHandle_t xTimer)
{
    //assert(false == true);
    assertDump(false == true);
    //assertBreak(false == true);
}

void setup()
{
    esp_log_level_set("*", ESP_LOG_VERBOSE);
    dbg_include_FreeRTOS_parameters();
    setvbuf(stdout, NULL, _IONBF, 0);

    printf("Starting...\r\n");

    xQueue = xQueueCreate(10, sizeof(unsigned long));
    vQueueAddToRegistry(xQueue, "QueueOne");

    unsigned long ulVar = 10UL;
    if (xQueueSend(xQueue,
                   (void *)&ulVar,
                   (TickType_t)10) != pdPASS)
    {
        printf("failed to send item to queue");
    }

    xSemaphore = xSemaphoreCreateCounting(10, 0);

    xTimer = xTimerCreate(
        "TimerOne",
        pdMS_TO_TICKS(5000),
        pdTRUE,
        (void *)0,
        vTimerCallback);

    xTimerCrash = xTimerCreate(
        "CrashTimer",
        pdMS_TO_TICKS(10000),
        pdTRUE,
        (void *)0,
        vFailTimerCallback);

    if (xTimerStart(xTimer, 0) != pdPASS)
    {
        printf("failed to start timer");
    }

    if (xTimerStart(xTimerCrash, 0) != pdPASS)
    {
        printf("failed to start crash timer");
    }
}

void loop()
{
    test();
}

extern "C"
{
    void app_main()
    {
        setup();
        volatile int i = 0;
        while (true)
        {
            loop();
            i++;
        }
    }
}