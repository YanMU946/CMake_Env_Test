#ifndef __WS2812_HPP__
#define __WS2812_HPP__
#include "spi.h"

namespace ihiter
{
constexpr uint8_t WS2812_CODE0 = 0x80;
constexpr uint8_t WS2812_CODE1 = 0xF0;
constexpr uint8_t WS2812_SENDNUM = 24;

enum class WS : uint32_t
{
    BLANK = 0x000000,
    WHITE = 0x111111,
    RED = 0x110000,
    GREEN = 0x001100,
    BLUE = 0x000011,
    YELLOW = 0x111100,
    CYAN = 0x001111,
    MAGENTA = 0x110011,
    ORANGE = 0x110800,
    PINK = 0x110109,
    PURPLE = 0x09020F,
    BROWN = 0x090403
};

class WS2812
{
public:
    WS2812(SPI_HandleTypeDef* _hspi) : hspi(_hspi) { }

    void set(WS _rgb)
    {
        rgb = static_cast<uint32_t>(_rgb);
        r = (uint8_t)(rgb >> 16);
        g = (uint8_t)(rgb >> 8);
        b = (uint8_t)(rgb >> 0);
        for (uint8_t i = 0; i < 8; i++)
        {
            data[i] = (((g >> (7 - i)) & 0x01) ? WS2812_CODE1 : WS2812_CODE0) >> 1;
            data[i + 8] = (((r >> (7 - i)) & 0x01) ? WS2812_CODE1 : WS2812_CODE0) >> 1;
            data[i + 16] = (((b >> (7 - i)) & 0x01) ? WS2812_CODE1 : WS2812_CODE0) >> 1;
        }
        HAL_SPI_Transmit_DMA(hspi, data, WS2812_SENDNUM);
    }

private:
    uint32_t rgb = 0x000000;
    uint8_t r = 0x00;
    uint8_t g = 0x00;
    uint8_t b = 0x00;

    SPI_HandleTypeDef* hspi;
    uint8_t data[WS2812_SENDNUM];
};

}  // namespace ihiter
#endif /* end ws2812 */
