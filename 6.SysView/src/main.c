#include <stdint.h>
#include <nvs_flash.h>
#include <esp_log.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h> 
#include <esp_system.h>
#include <esp_task_wdt.h>


void init_run_time_counter()
{
}

unsigned long get_run_time_counter_value()
{
    return esp_timer_get_time();
}

volatile int iterations_counter[3] = {0};

void low_load()
{
    while (1)
    {
        uint32_t load = 0;
        for (int i = 0; i < 100000; i++)
        {
            load += 1;
            if ((i % 1000) == 0){
                esp_task_wdt_reset();
                taskYIELD();
            }
        }
        iterations_counter[0]++;
        vTaskDelay(1);
    }
}

void medium_load()
{
    while (1)
    {
        uint32_t load = 0;
        for (int i = 0; i < 100000; i++)
        {
            load += 1;
            if ((i % 1000) == 0){
                esp_task_wdt_reset();
                taskYIELD();
            }
        }
        iterations_counter[1]++;
        vTaskDelay(1);
    }
}

void high_load()
{
    while (1)
    {
        uint32_t load = 0;
        for (int i = 0; i < 100000; i++)
        {
            load += 1;
            if ((i % 1000) == 0){
                esp_task_wdt_reset();
                taskYIELD();
            }
        }
        iterations_counter[2]++;
        vTaskDelay(1);
    }
}


void display_stats()
{
    

    while (1)
    {
        char* pcWriteBuffer;
        char WriteBuffer[4096]={0};
        pcWriteBuffer = &WriteBuffer[0];

        printf("Printing Stats:\r\n");
        vTaskDelay(500 / portTICK_PERIOD_MS);
        
        vTaskGetRunTimeStats(pcWriteBuffer);
        printf(pcWriteBuffer);
        printf("iterations low: %d medium: %d high: %d\r\n", iterations_counter[0], iterations_counter[1], iterations_counter[2]);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    printf("BEGIN~~~~~~~~~~~~\r\n"); 

    printf("Starting low_load\r\n");
    xTaskCreate(&low_load, "low_load1", 4 * 1024, NULL, 4, NULL);
    xTaskCreate(&low_load, "low_load2", 4 * 1024, NULL, 4, NULL);

    printf("Starting medium_load\r\n");
    xTaskCreate(&medium_load, "medium_load1", 4 * 1024, NULL, 5, NULL);
    xTaskCreate(&medium_load, "medium_load2", 4 * 1024, NULL, 5, NULL);

    printf("Starting high_load\r\n");
    xTaskCreate(&high_load, "high_load1",4 * 1024, NULL, 8, NULL);
    xTaskCreate(&high_load, "high_load2",4 * 1024, NULL, 8, NULL); 

    printf("Starting display_stats\r\n");
    xTaskCreate(&display_stats, "display_stats", 8 * 1024, NULL, 5, NULL);
}