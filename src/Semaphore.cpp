
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "freertos/semphr.h"

SemaphoreHandle_t xSemaphore = xSemaphoreCreateMutex();

void takeSemaphore()
{
    while (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdFALSE)
    {
    }
}

void giveSemaphore()
{
    xSemaphoreGive(xSemaphore);
}