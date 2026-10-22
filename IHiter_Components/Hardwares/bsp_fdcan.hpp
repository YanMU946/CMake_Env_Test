#ifndef __FDCAN_HPP__
#define __FDCAN_HPP__
#include "fdcan.h"

namespace ihiter
{

constexpr uint8_t FDCAN_DataLenth = 8;

class FDCAN
{
public:
    FDCAN(FDCAN_HandleTypeDef* _hfdcan) : hfdcan(_hfdcan) { }

    void config();

    void send(uint16_t id, uint8_t* data, uint32_t len = FDCAN_DataLenth);

    void rec() { HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rx_header, rx_buf); }

    uint32_t rx_id() { return rx_header.Identifier; }

    uint8_t* rx_data() { return rx_buf; }

private:
    FDCAN_HandleTypeDef* hfdcan;
    FDCAN_TxHeaderTypeDef tx_header;
    FDCAN_RxHeaderTypeDef rx_header;

    uint8_t rx_buf[FDCAN_DataLenth];
};

}  // namespace ihiter
#endif /* end fdcan */
