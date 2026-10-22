#include "bsp_fdcan.hpp"

namespace ihiter
{

void FDCAN::config()
{
    FDCAN_FilterTypeDef FDCAN_FilterConfigStructure;

    FDCAN_FilterConfigStructure.IdType = FDCAN_STANDARD_ID;
    FDCAN_FilterConfigStructure.FilterIndex = 0;
    FDCAN_FilterConfigStructure.FilterType = FDCAN_FILTER_MASK;
    FDCAN_FilterConfigStructure.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    FDCAN_FilterConfigStructure.FilterID1 = 0x00000000;
    FDCAN_FilterConfigStructure.FilterID2 = 0x00000000;

    // config filter
    HAL_FDCAN_ConfigFilter(hfdcan, &FDCAN_FilterConfigStructure);
    HAL_FDCAN_ConfigGlobalFilter(hfdcan, FDCAN_REJECT, FDCAN_REJECT, FDCAN_REJECT_REMOTE, FDCAN_REJECT_REMOTE);
    HAL_FDCAN_ConfigFifoWatermark(hfdcan, FDCAN_CFG_RX_FIFO0, 1);
    // start FDCAN, activate interrupts
    HAL_FDCAN_Start(hfdcan);
    HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE | FDCAN_IT_BUS_OFF, 0);
}

void FDCAN::send(uint16_t id, uint8_t* data, uint32_t len)
{
    tx_header.Identifier = id;
    tx_header.IdType = FDCAN_STANDARD_ID;
    tx_header.TxFrameType = FDCAN_DATA_FRAME;
    tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;

    tx_header.BitRateSwitch = FDCAN_BRS_ON;
    tx_header.FDFormat = FDCAN_FD_CAN;
    tx_header.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    tx_header.MessageMarker = 0;

    HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &tx_header, data);

    if (len <= 8)
        tx_header.DataLength = len;
    else
        switch (len)
        {
            case 12: tx_header.DataLength = FDCAN_DLC_BYTES_12; break;
            case 16: tx_header.DataLength = FDCAN_DLC_BYTES_16; break;
            case 20: tx_header.DataLength = FDCAN_DLC_BYTES_20; break;
            case 24: tx_header.DataLength = FDCAN_DLC_BYTES_24; break;
            case 32: tx_header.DataLength = FDCAN_DLC_BYTES_32; break;
            case 48: tx_header.DataLength = FDCAN_DLC_BYTES_48; break;
            case 64: tx_header.DataLength = FDCAN_DLC_BYTES_64; break;
            default: tx_header.DataLength = FDCAN_DLC_BYTES_8; break;
        }
}

}  // namespace ihiter
