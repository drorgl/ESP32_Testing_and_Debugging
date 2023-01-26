#pragma once

#ifdef DEBUG

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
extern "C"
{
#include <soc/cpu.h>
}

uint8_t volatile dbgFreeRTOSConfig_vTaskSuspend = INCLUDE_vTaskSuspend;
uint8_t volatile dbgFreeRTOSConfig_vTaskDelete = INCLUDE_vTaskDelete;
uint8_t volatile dbgFreeRTOSConfig_GENERATE_RUN_TIME_STATS = configGENERATE_RUN_TIME_STATS;
uint8_t volatile dbgFreeRTOSConfig_QUEUE_REGISTRY_SIZE = configQUEUE_REGISTRY_SIZE;

#endif

//you must include all variables that should be included in the debug binary, otherwise the linker will remove them
void dbg_include_FreeRTOS_parameters()
{
#ifdef DEBUG
    dbgFreeRTOSConfig_vTaskSuspend = dbgFreeRTOSConfig_vTaskSuspend;
    dbgFreeRTOSConfig_vTaskDelete = dbgFreeRTOSConfig_vTaskDelete;
    dbgFreeRTOSConfig_GENERATE_RUN_TIME_STATS = dbgFreeRTOSConfig_GENERATE_RUN_TIME_STATS;
    dbgFreeRTOSConfig_QUEUE_REGISTRY_SIZE = dbgFreeRTOSConfig_QUEUE_REGISTRY_SIZE;
#endif
}

const char *DEBUG_TAG = "DEBUG";

typedef void (*null_callback)(void);
static volatile null_callback null_ref = NULL;

void crashDump(const char *file, int line, const char *func, const char *expr)
{
    ESP_LOGE(DEBUG_TAG, "Assert failed in %s, %s:%d (%s)", func, file, line, expr);
    null_ref();
}

#define assertDump(__e) ((__e) ? (void)0 : crashDump(__FILE__, __LINE__, __ASSERT_FUNC, #__e))

void assertBreak(const char *file, int line, const char *func, const char *expr)
{
    ESP_LOGE(DEBUG_TAG, "Assert failed in %s, %s:%d (%s)", func, file, line, expr);
    if (esp_cpu_in_ocd_debug_mode())
    {
        __asm__("break 0,0");
    }
}

#define assertBreak(__e) ((__e) ? (void)0 : assertBreak(__FILE__, __LINE__, __ASSERT_FUNC, #__e))

//invoke_abort()