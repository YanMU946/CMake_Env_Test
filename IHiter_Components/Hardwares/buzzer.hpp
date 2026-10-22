#ifndef __BUZZER_HPP__
#define __BUZZER_HPP__
#include "tim.h"

namespace ihiter
{

constexpr float APB2_FREQ = 2.4e8;
constexpr float QUAVER_SPEED = 62.5f;  //62.5ms
constexpr float DEFAULT_VOL = 0.2f;    //default volume of buzzer

enum class NOTE : uint32_t
{
    N_NULL = 1,

    N_3A = 110,
    N_3B = 123,   // 123.471
    N_3sD = 156,  // 155.563
    N_3E = 165,   // 164.814
    N_3F = 175,   // 174.614
    N_3sF = 185,  // 184.997
    N_3G = 196,   // 195.998

    N_A = 220,
    N_sA = 233,  // 233.082
    N_B = 247,   // 246.942
    N_C = 262,   // 261.626
    N_sC = 277,  // 277.183
    N_D = 294,   // 293.665
    N_sD = 311,  // 311.127
    N_E = 330,   // 329.629
    N_F = 349,   // 349.228
    N_sF = 370,  // 369.994
    N_G = 392,   // 391.995
    N_sG = 415,  // 415.305

    N_5A = 440,
    N_5B = 494,  // 493.883
    N_5C = 523,  // 523.251
    N_5D = 587,  // 587.33
    N_5sG = 831  // 830.609
};

class BUZZER
{
public:
    BUZZER(TIM_HandleTypeDef* _htim, uint32_t _channel, float _apb_freq = APB2_FREQ)
        : htim(_htim),
          channel(_channel),
          apb_freq(_apb_freq)
    {
        __HAL_TIM_SET_PRESCALER(_htim, apb_freq / 1e4 - 1);
    }

    void set_note(NOTE _note, float _vol = DEFAULT_VOL)
    {
        if (_note == NOTE::N_NULL)
        {
            __HAL_TIM_SetCompare(htim, channel, 0);
            return;
        }
        uint32_t arr_buzzer = htim->Instance->PSC / static_cast<uint32_t>(_note);
        __HAL_TIM_SetAutoreload(htim, arr_buzzer - 1);
        __HAL_TIM_SetCompare(htim, channel, arr_buzzer * _vol - 1);
        __HAL_TIM_SET_COUNTER(htim, 0);
    }

    void start() { HAL_TIM_PWM_Start(htim, channel); }

    void stop() { HAL_TIM_PWM_Stop(htim, channel); }

private:
    TIM_HandleTypeDef* htim;
    uint32_t channel;
    const float apb_freq;
};

}  // namespace ihiter
#endif /* end buzzer */
