#include "cmsis_os2.h"
#include "system_monitor.h"

#include "ws2812.hpp"
#include "buzzer.hpp"
#include "bsp_fdcan.hpp"

using namespace ihiter;

__attribute__((section(".BDMA_SRAM"))) WS2812 ws2812(&hspi6);
BUZZER buzzer(&htim12, TIM_CHANNEL_2);
FDCAN fdcan1(&hfdcan1), fdcan2(&hfdcan2), fdcan3(&hfdcan3);

extern "C" void led_ctrl(void* argument)
{
    ws2812.set(WS::RED);

    buzzer.stop();
    buzzer.set_note(NOTE::N_C);
    buzzer.start();
    osDelay(600);
    buzzer.stop();
    buzzer.set_note(NOTE::N_G);
    buzzer.start();
    osDelay(600);
    buzzer.stop();
    buzzer.set_note(NOTE::N_5C);
    buzzer.start();
    osDelay(600);
    buzzer.stop();

    ws2812.set(WS::GREEN);

    fdcan1.config();
    fdcan2.config();
    fdcan3.config();
    osDelay(1000);

    /* Infinite loop */
    for (;;)
    {
        ws2812.set(WS::BLUE);
        osDelay(500);
        ws2812.set(WS::YELLOW);
        osDelay(500);
        ws2812.set(WS::PINK);
        osDelay(500);
        system_monitor.Led_Ctrl_cnt++;
    }
}
