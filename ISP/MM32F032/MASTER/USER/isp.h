#ifndef __ISP_H
#define __ISP_H

#include "stdint.h"
#include "module.h"

#define ISP_START_FRAME            0x7F

#define ISP_ACK                    0x79
#define ISP_NACK                   0x1F

#define ISP_GET                    0x00
#define ISP_GET_VERSION            0x01
#define ISP_GET_ID                 0x02

#define ISP_READ_MEMORY            0x11
#define ISP_GO                     0x21
#define ISP_WRITE_MEMORY           0x31
#define ISP_ERASE_MEMORY           0x43
#define ISP_ERASE_MEMORY_EX        0x44
#define ISP_WRITE_PROTECT          0x63
#define ISP_WRITE_UNPROTECT        0x73
#define ISP_READOUT_PROTECT        0x82
#define ISP_READOUT_UNPROTECT      0x92

//ISP direction
typedef enum
{
    PC_TO_MCU   = 'P',
    MCU_TO_PC   = 'S'
} Direction_t;

#define UART_REC_MAX_LEN            (300)        //定义最大接收字节数
#define UART_REC_MIN_LEN            (5)         //定义最小接收字节数
#define UART_SEND_MAX_LEN           (269)       //定义最大发送字节数
#define UART_SEND_MIN_LEN           (5)         //定义最小发送字节数

#define START_CONFIG                (0x00000000)
#define SEND_CONFIG                 (0x00000001)
#define CONFIG_COMPLETE             (0x00000002)
#define LOAD_CONFIG_LENGTH          (17)
#define LOAD_CONFIG_DATA_LENGTH     (256)
#define LOAD_FLASH_DATA_LENGTH      (256)

#define ONE_STEP      1
#define TWO_STEP      2
#define THREE_STEP    3
#define FOUR_STEP     4

//ISP command
typedef enum
{
    HANDSHAKE_PROTOCOL  = 0x00,
    GET_ISP_VERSION     = 0x20,
    LOAD_CONFIG         = 0x02,
    //LOAD_CONFIG加载后支持
    APP_DOWLOAD         = 0x01,
    CONFIGURED_BAUDRATE = 0x03,
    WRITE_REGISTER      = 0x05,
    READ_REGISTER       = 0x06,
    WRITE_MEMORY        = 0x07,
    WRITE_FLASH         = 0x08,
    PROGRAM_JUMP        = 0x09,
    WRITE_OPTIONBYTES   = 0x0A,
    GET_CHECKSUN        = 0x0F,
    INIT_MUC            = 0x5A,
    COMPATIBLE_SYNCBAUD_PROTOCOL   = 0x30,
    COMPATIBLE_ISP_GET = COMPATIBLE_SYNCBAUD_PROTOCOL + 1 + ISP_GET,
    COMPATIBLE_ISP_GET_VERSION = COMPATIBLE_SYNCBAUD_PROTOCOL + 1 + ISP_GET_VERSION,
    COMPATIBLE_ISP_GET_ID = COMPATIBLE_SYNCBAUD_PROTOCOL + 1 + ISP_GET_ID,
    COMPATIBLE_ISP_READ_MEMORY = COMPATIBLE_SYNCBAUD_PROTOCOL + 1 + ISP_READ_MEMORY,
    COMPATIBLE_ISP_GO = COMPATIBLE_SYNCBAUD_PROTOCOL + 1 + ISP_GO,
    COMPATIBLE_ISP_WRITE_MEMORY = COMPATIBLE_SYNCBAUD_PROTOCOL + 1 + ISP_WRITE_MEMORY,
    COMPATIBLE_ISP_ERASE_MEMORY = COMPATIBLE_SYNCBAUD_PROTOCOL + 1 + ISP_ERASE_MEMORY,
    COMPATIBLE_ISP_ERASE_MEMORY_EX = COMPATIBLE_SYNCBAUD_PROTOCOL + 1 + ISP_ERASE_MEMORY_EX,
    COMPATIBLE_ISP_WRITE_PROTECT = COMPATIBLE_SYNCBAUD_PROTOCOL + 1 + ISP_WRITE_PROTECT,
    COMPATIBLE_ISP_WRITE_UNPROTECT = COMPATIBLE_SYNCBAUD_PROTOCOL + 1 + ISP_WRITE_UNPROTECT,
    COMPATIBLE_ISP_READOUT_PROTECT = COMPATIBLE_SYNCBAUD_PROTOCOL + 1 + ISP_READOUT_PROTECT,
    COMPATIBLE_ISP_READOUT_UNPROTECT = COMPATIBLE_SYNCBAUD_PROTOCOL + 1 + ISP_READOUT_UNPROTECT
} Command_t;

typedef struct
{
    Direction_t tDirection;
    uint16_t hwFrameLengh;
    Command_t tCommand;
    uint32_t wConfig;
    uint32_t wAddress;
    uint32_t wDataLength;
    const uint8_t *pData;
    uint8_t framebuf[300];
    uint8_t chChecksum;
} FrameDcb_t;

fsm_rt_t Received_SRAM_Command(void);
bool Send_SRAM_Frame(FrameDcb_t *pFrameDcb);
fsm_rt_t Handshake(FrameDcb_t *pFrameDcb);
fsm_rt_t LoadRamCode(FrameDcb_t *pFrameDcb);
fsm_rt_t SyncBaud(FrameDcb_t *pFrameDcb);
fsm_rt_t IspGet(FrameDcb_t *pFrameDcb);
fsm_rt_t IspGetVersion(FrameDcb_t *pFrameDcb);
fsm_rt_t IspGetID(FrameDcb_t *pFrameDcb);
fsm_rt_t IspReadMemory(FrameDcb_t *pFrameDcb);
fsm_rt_t IspGo(FrameDcb_t *pFrameDcb);
fsm_rt_t IspWriteMemroy(FrameDcb_t *pFrameDcb);
fsm_rt_t IspEraseMempry(FrameDcb_t *pFrameDcb);
fsm_rt_t IspEraseMempryEX(FrameDcb_t *pFrameDcb);
fsm_rt_t IspWriteProtect(FrameDcb_t *pFrameDcb);
fsm_rt_t IspWriteUNProtect(FrameDcb_t *pFrameDcb);
fsm_rt_t IspReadProtect(FrameDcb_t *pFrameDcb);
fsm_rt_t IspReadUNProtect(FrameDcb_t *pFrameDcb);

#endif
