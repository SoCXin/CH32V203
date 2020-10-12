////////////////////////////////////////////////////////////////////////////////
/// @file     reg_usb.h
/// @author   AE TEAM
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE SERIES OF
///           MM32 FIRMWARE LIBRARY.
////////////////////////////////////////////////////////////////////////////////
/// @attention
///
/// THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
/// CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
/// TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
/// CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
/// HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
/// CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
///
/// <H2><CENTER>&COPY; COPYRIGHT MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion

#ifndef __REG_USB_H
#define __REG_USB_H

// Files includes

#include <stdint.h>
#include <stdbool.h>
#include "types.h"




#if defined ( __CC_ARM )
#pragma anon_unions
#endif










////////////////////////////////////////////////////////////////////////////////
/// @brief USB Base Address Definition
////////////////////////////////////////////////////////////////////////////////
#define USB_BASE                    (APB1PERIPH_BASE + 0x5C00)              ///< Base Address: 0x40005C00




////////////////////////////////////////////////////////////////////////////////
/// @brief USB Register Structure Definition
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    __IO u32 TOP;                                                               ///< Top Register                                   offset: 0x00
    __IO u32 INT_STATE;                                                         ///< Interrupt State Register                       offset: 0x04
    __IO u32 EP_INT_STATE;                                                      ///< Endpoint Interrupt State Register              offset: 0x08
    __IO u32 EP0_INT_STATE;                                                     ///< EP0 Interrupt State Register                   offset: 0x0C
    __IO u32 INT_EN;                                                            ///< Interrupt Enable Register                      offset: 0x10
    __IO u32 EP_INT_EN;                                                         ///< Endpoint Interrupt Enable Register             offset: 0x14
    __IO u32 EP0_INT_EN;                                                        ///< EP0 Interrupt Enable Register                  offset: 0x18
    __IO u32 RESERVED0;                                                         ///< Reserved                                       offset: 0x1C
    __IO u32 EP1_INT_STATE;                                                     ///< EP1 Interrupt State Register                   offset: 0x20
    __IO u32 EP2_INT_STATE;                                                     ///< EP2 Interrupt State Register                   offset: 0x24
    __IO u32 EP3_INT_STATE;                                                     ///< EP3 Interrupt State Register                   offset: 0x28
    __IO u32 EP4_INT_STATE;                                                     ///< EP4 Interrupt State Register                   offset: 0x2C
    __IO u32 RESERVED1;                                                         ///< Reserved                                       offset: 0x30
    __IO u32 RESERVED2;                                                         ///< Reserved                                       offset: 0x34
    __IO u32 RESERVED3;                                                         ///< Reserved                                       offset: 0x38
    __IO u32 RESERVED4;                                                         ///< Reserved                                       offset: 0x3C
    __IO u32 EP1_INT_EN;                                                        ///< EP0 Interrupt Enable Register                  offset: 0x40
    __IO u32 EP2_INT_EN;                                                        ///< EP0 Interrupt Enable Register                  offset: 0x44
    __IO u32 EP3_INT_EN;                                                        ///< EP0 Interrupt Enable Register                  offset: 0x48
    __IO u32 EP4_INT_EN;                                                        ///< EP0 Interrupt Enable Register                  offset: 0x4C
    __IO u32 RESERVED5;                                                         ///< Reserved                                       offset: 0x50
    __IO u32 RESERVED6;                                                         ///< Reserved                                       offset: 0x54
    __IO u32 RESERVED7;                                                         ///< Reserved                                       offset: 0x58
    __IO u32 RESERVED8;                                                         ///< Reserved                                       offset: 0x5C
    __IO u32 ADDR;                                                              ///< USB Address Register                           offset: 0x60
    __IO u32 EP_EN;                                                             ///< Endpoint Enable Register                       offset: 0x64
    __IO u32 EP_DMA_DIR;                                                        ///< Endpoint DMA Direction Register                offset: 0x68
    __IO u32 EP_TYPE;                                                           ///< Endpoint Type Register                         offset: 0x6C
    __IO u32 EP_INDEX1_2;                                                       ///< EP Index 1_2 register                          offset: 0x70
    __IO u32 EP_INDEX3_4;                                                       ///< EP Index 3_4 register                          offset: 0x74
    __IO u32 TOG_CTRL1_4;                                                       ///< Toggle Control Register                        offset: 0x78

    __IO u32 TOG_STST1_4;                                                       ///< Toggle State Register                          offset: 0x7C

    __IO u32 SETUP0;                                                            ///< SETUP Packet Byte 0 Register                   offset: 0x80
    __IO u32 SETUP1;                                                            ///< SETUP Packet Byte 1 Register                   offset: 0x84
    __IO u32 SETUP2;                                                            ///< SETUP Packet Byte 2 Register                   offset: 0x88
    __IO u32 SETUP3;                                                            ///< SETUP Packet Byte 3 Register                   offset: 0x8C
    __IO u32 SETUP4;                                                            ///< SETUP Packet Byte 4 Register                   offset: 0x90
    __IO u32 SETUP5;                                                            ///< SETUP Packet Byte 5 Register                   offset: 0x94
    __IO u32 SETUP6;                                                            ///< SETUP Packet Byte 6 Register                   offset: 0x98
    __IO u32 SETUP7;                                                            ///< SETUP Packet Byte 7 Register                   offset: 0x9C
    __IO u32 PAKET_SIZEL;                                                       ///< DMA Max Packet Size 0 Register                 offset: 0xA0
    __IO u32 PAKET_SIZEH;                                                       ///< DMA Max Packet Size 1 Register                 offset: 0xA4
    __IO u32 RESERVED9;                                                         ///< Reserved                                       offset: 0xA8
    __IO u32 RESERVED10;                                                        ///< Reserved                                       offset: 0xAC

    __IO u32 RESERVED11;                                                        ///< Reserved                                       offset: 0xB0
    __IO u32 RESERVED12;                                                        ///< Reserved                                       offset: 0xB4
    __IO u32 RESERVED13;                                                        ///< Reserved                                       offset: 0xB8
    __IO u32 RESERVED14;                                                        ///< Reserved                                       offset: 0xBC
    __IO u32 RESERVED15;                                                        ///< Reserved                                       offset: 0xC0
    __IO u32 RESERVED16;                                                        ///< Reserved                                       offset: 0xC4
    __IO u32 RESERVED17;                                                        ///< Reserved                                       offset: 0xC8
    __IO u32 RESERVED18;                                                        ///< Reserved                                       offset: 0xCC
    __IO u32 RESERVED19;                                                        ///< Reserved                                       offset: 0xD0
    __IO u32 RESERVED20;                                                        ///< Reserved                                       offset: 0xD4
    __IO u32 RESERVED21;                                                        ///< Reserved                                       offset: 0xD8
    __IO u32 RESERVED22;                                                        ///< Reserved                                       offset: 0xDC

    __IO u32 RESERVED23;                                                        ///< Reserved                                       offset: 0xE0
    __IO u32 RESERVED24;                                                        ///< Reserved                                       offset: 0xE4
    __IO u32 RESERVED25;                                                        ///< Reserved                                       offset: 0xE8
    __IO u32 RESERVED26;                                                        ///< Reserved                                       offset: 0xEC

    __IO u32 RESERVED27;                                                        ///< Reserved                                       offset: 0xF0
    __IO u32 RESERVED28;                                                        ///< Reserved                                       offset: 0xF4
    __IO u32 RESERVED29;                                                        ///< Reserved                                       offset: 0xF8
    __IO u32 RESERVED30;                                                        ///< Reserved                                       offset: 0xFC

    __IO u32 EP0_AVIL;                                                          ///< EP0 Available Data Register                    offset: 0x100
    __IO u32 EP1_AVIL;                                                          ///< EP1 Available Data Register                    offset: 0x104
    __IO u32 EP2_AVIL;                                                          ///< EP2 Available Data Register                    offset: 0x108
    __IO u32 EP3_AVIL;                                                          ///< EP3 Available Data Register                    offset: 0x10C
    __IO u32 EP4_AVIL;                                                          ///< EP4 Available Data Register                    offset: 0x110

    __IO u32 RESERVED31;                                                        ///< Reserved                                       offset: 0x114
    __IO u32 RESERVED32;                                                        ///< Reserved                                       offset: 0x118
    __IO u32 RESERVED33;                                                        ///< Reserved                                       offset: 0x11C
    __IO u32 DMA1_NUML;                                                         ///< EP1 DMA number 0-7                             offset: 0x120

    __IO u32 DMA1_NUMH;                                                         ///< EP1 DMA number 8-15                            offset: 0x124
    __IO u32 RESERVED34;                                                        ///< Reserved                                       offset: 0x128
    __IO u32 RESERVED35;                                                        ///< Reserved                                       offset: 0x12C
    __IO u32 DMA2_NUML;                                                         ///< EP2 DMA number 0-7                             offset: 0x130
    __IO u32 DMA2_NUMH;                                                         ///< EP2 DMA number 8-15                            offset: 0x134
    __IO u32 RESERVED36;                                                        ///< Reserved                                       offset: 0x138
    __IO u32 RESERVED37;                                                        ///< Reserved                                       offset: 0x13C
    __IO u32 EP0_CTRL;                                                          ///< EP0 Control Register                           offset: 0x140
    __IO u32 EP1_CTRL;                                                          ///< EP1 Control Register                           offset: 0x144
    __IO u32 EP2_CTRL;                                                          ///< EP2 Control Register                           offset: 0x148
    __IO u32 EP3_CTRL;                                                          ///< EP3 Control Register                           offset: 0x14C
    __IO u32 EP4_CTRL;                                                          ///< EP4 Control Register                           offset: 0x150

    __IO u32 RESERVED38;                                                        ///< Reserved                                       offset: 0x154
    __IO u32 RESERVED39;                                                        ///< Reserved                                       offset: 0x158
    __IO u32 RESERVED40;                                                        ///< Reserved                                       offset: 0x15C

    __IO u32 EP0_FIFO;                                                          ///< EP0 FIFO Register                              offset: 0x160
    __IO u32 EP1_FIFO;                                                          ///< EP1 FIFO Register                              offset: 0x164
    __IO u32 EP2_FIFO;                                                          ///< EP2 FIFO Register                              offset: 0x168
    __IO u32 EP3_FIFO;                                                          ///< EP3 FIFO Register                              offset: 0x16C
    __IO u32 EP4_FIFO;                                                          ///< EP4 FIFO Register                              offset: 0x170

    __IO u32 RESERVED41;                                                        ///< Reserved                                       offset: 0x174
    __IO u32 RESERVED42;                                                        ///< Reserved                                       offset: 0x178
    __IO u32 RESERVED43;                                                        ///< Reserved                                       offset: 0x17C

    __IO u32 EP_MEM;                                                            ///< EP Data Memory Register                        offset: 0x180
    __IO u32 EP_DMA;                                                            ///< EP DMA Register                                offset: 0x184
    __IO u32 EP_HALT;                                                           ///< EP Halt Register                               offset: 0x188
    __IO u32 RESERVED44;                                                        ///< Reserved                                       offset: 0x18C
    __IO u32 RESERVED45;                                                        ///< Reserved                                       offset: 0x190
    __IO u32 RESERVED46;                                                        ///< Reserved                                       offset: 0x194
    __IO u32 RESERVED47;                                                        ///< Reserved                                       offset: 0x198
    __IO u32 RESERVED48;                                                        ///< Reserved                                       offset: 0x19C
    __IO u32 RESERVED49;                                                        ///< Reserved                                       offset: 0x1A0
    __IO u32 RESERVED50;                                                        ///< Reserved                                       offset: 0x1A4
    __IO u32 RESERVED51;                                                        ///< Reserved                                       offset: 0x1A8
    __IO u32 RESERVED52;                                                        ///< Reserved                                       offset: 0x1AC
    __IO u32 RESERVED53;                                                        ///< Reserved                                       offset: 0x1B0
    __IO u32 RESERVED54;                                                        ///< Reserved                                       offset: 0x1B4
    __IO u32 RESERVED55;                                                        ///< Reserved                                       offset: 0x1B8
    __IO u32 RESERVED56;                                                        ///< Reserved                                       offset: 0x1BC
    __IO u32 POWER;                                                             ///< Power Register                                 offset: 0x1C0
    __IO u32 AHB_DMA;                                                           ///< USB AHB DMA Register                           offset: 0x1C4
} USB_TypeDef;


////////////////////////////////////////////////////////////////////////////////
/// @brief USBD type pointer Definition
////////////////////////////////////////////////////////////////////////////////
#define USB                         ((USB_TypeDef*) USB_BASE )



////////////////////////////////////////////////////////////////////////////////
/// @brief USB_TOP Register Bit Definition
////////////////////////////////////////////////////////////////////////////////

#define USB_TOP_SPEED_Pos               (0)
#define USB_TOP_SPEED                   (0x01U << USB_TOP_SPEED_Pos)            ///< USB Speed
#define USB_TOP_CONNECT_Pos             (1)
#define USB_TOP_CONNECT                 (0x01U << USB_TOP_CONNECT_Pos)          ///< USB connection
#define USB_TOP_RESET_Pos               (3)
#define USB_TOP_RESET                   (0x01U << USB_TOP_RESET_Pos)            ///< Reset EP and FIFO in USB controller
#define USB_TOP_SUSPEND_Pos             (4)
#define USB_TOP_SUSPEND                 (0x01U << USB_TOP_SUSPEND_Pos)          ///< USB suspend state
#define USB_TOP_STATE_Pos               (5)
#define USB_TOP_STATE                   (0x03U << USB_TOP_STATE_Pos)            ///< Current USB DP/DM line state
#define USB_TOP_ACTIVE_Pos              (7)
#define USB_TOP_ACTIVE                  (0x01U << USB_TOP_ACTIVE_Pos)           ///< USB bus is active

////////////////////////////////////////////////////////////////////////////////
/// @brief USB_INT_STATE Register Bit Definition
////////////////////////////////////////////////////////////////////////////////
#define USB_INT_STATE_RSTF_Pos          (0)
#define USB_INT_STATE_RSTF              (0x01U << USB_INT_STATE_RSTF_Pos)       ///< BUS reset received
#define USB_INT_STATE_SUSPENDF_Pos      (1)
#define USB_INT_STATE_SUSPENDF          (0x01U << USB_INT_STATE_SUSPENDF_Pos)   ///< BUS suspend received
#define USB_INT_STATE_RESUMF_Pos        (2)
#define USB_INT_STATE_RESUMF            (0x01U << USB_INT_STATE_RESUMF_Pos)     ///< BUS resume received
#define USB_INT_STATE_SOFF_Pos          (3)
#define USB_INT_STATE_SOFF              (0x01U << USB_INT_STATE_SOFF_Pos)       ///< BUS SOF received
#define USB_INT_STATE_EPINTF_Pos        (4)
#define USB_INT_STATE_EPINTF            (0x01U << USB_INT_STATE_EPINTF_Pos)     ///< EP interrupt received

////////////////////////////////////////////////////////////////////////////////
/// @brief EP_INT_STATE Register Bit Definition
////////////////////////////////////////////////////////////////////////////////
#define EP_INT_STATE_EP0F_Pos           (0)
#define EP_INT_STATE_EP0F               (0x01U << EP_INT_STATE_EP0F_Pos)        ///< EP0 interrupt received
#define EP_INT_STATE_EP1F_Pos           (1)
#define EP_INT_STATE_EP1F               (0x01U << EP_INT_STATE_EP1F_Pos)        ///< EP1 interrupt received
#define EP_INT_STATE_EP2F_Pos           (2)
#define EP_INT_STATE_EP2F               (0x01U << EP_INT_STATE_EP2F_Pos)        ///< EP2 interrupt received
#define EP_INT_STATE_EP3F_Pos           (3)
#define EP_INT_STATE_EP3F               (0x01U << EP_INT_STATE_EP3F_Pos)        ///< EP3 interrupt received
#define EP_INT_STATE_EP4F_Pos           (4)
#define EP_INT_STATE_EP4F               (0x01U << EP_INT_STATE_EP4F_Pos)        ///< EP4 interrupt received

////////////////////////////////////////////////////////////////////////////////
/// @brief EPn_INT_STATE Register Bit Definition
////////////////////////////////////////////////////////////////////////////////
#define EPn_INT_STATE_SETUP_Pos         (0)
#define EPn_INT_STATE_SETUP             (0x01U << EPn_INT_STATE_SETUP_Pos)      ///< SETUP packet received
#define EPn_INT_STATE_END_Pos           (1)
#define EPn_INT_STATE_END               (0x01U << EPn_INT_STATE_END_Pos)        ///< Status stage finished
#define EPn_INT_STATE_INNACK_Pos        (2)
#define EPn_INT_STATE_INNACK            (0x01U << EPn_INT_STATE_INNACK_Pos)     ///< IN-NACK received
#define EPn_INT_STATE_INACK_Pos         (3)
#define EPn_INT_STATE_INACK             (0x01U << EPn_INT_STATE_INACK_Pos)      ///< IN-ACK received
#define EPn_INT_STATE_INSTALL_Pos       (4)
#define EPn_INT_STATE_INSTALL           (0x01U << EPn_INT_STATE_INSTALL_Pos)    ///< IN-STALL received
#define EPn_INT_STATE_OUTNACK_Pos       (5)
#define EPn_INT_STATE_OUTNACK           (0x01U << EPn_INT_STATE_OUTNACK_Pos)    ///< OUT-NACK received
#define EPn_INT_STATE_OUTACK_Pos        (6)
#define EPn_INT_STATE_OUTACK            (0x01U << EPn_INT_STATE_OUTACK_Pos)     ///< OUT-ACK received
#define EPn_INT_STATE_OUTSTALL_Pos      (7)
#define EPn_INT_STATE_OUTSTALL          (0x01U << EPn_INT_STATE_OUTSTALL_Pos)   ///< OUT-STALL received

////////////////////////////////////////////////////////////////////////////////
/// @brief USB_INT_EN Register Bit Definition
////////////////////////////////////////////////////////////////////////////////
#define USB_INT_EN_RSTIE_Pos            (0)
#define USB_INT_EN_RSTIE                (0x01U << USB_INT_EN_RSTIE_Pos)         ///< BUS reset interrupt enable
#define USB_INT_EN_SUSPENDIE_Pos        (1)
#define USB_INT_EN_SUSPENDIE            (0x01U << USB_INT_EN_SUSPENDIE_Pos)     ///< BUS suspend interrupt enable
#define USB_INT_EN_RESUMIE_Pos          (2)
#define USB_INT_EN_RESUMIE              (0x01U << USB_INT_EN_RESUMIE_Pos)       ///< BUS resume interrupt enable
#define USB_INT_EN_SOFIE_Pos            (3)
#define USB_INT_EN_SOFIE                (0x01U << USB_INT_EN_SOFIE_Pos)         ///< SOF interrupt enable
#define USB_INT_EN_EPIE_Pos             (4)
#define USB_INT_EN_EPIE                 (0x01U << USB_INT_EN_EPIE_Pos)          ///< EP interrupt enable
#define USB_INT_EN_INTMASK_Pos          (7)
#define USB_INT_EN_INTMASK              (0x01U << USB_INT_EN_INTMASK_Pos)       ///< interrupt mask
#define USB_INT_EN_EPINTIE              USB_INT_EN_EPIE

////////////////////////////////////////////////////////////////////////////////
/// @brief EP_INT_EN Register Bit Definition
////////////////////////////////////////////////////////////////////////////////
#define EP_INT_EN_EP0IE_Pos             (0)
#define EP_INT_EN_EP0IE                 (0x01U << EP_INT_EN_EP0IE_Pos)          ///< EP0 interrupt enable
#define EP_INT_EN_EP1IE_Pos             (1)
#define EP_INT_EN_EP1IE                 (0x01U << EP_INT_EN_EP1IE_Pos)          ///< EP1 interrupt enable
#define EP_INT_EN_EP2IE_Pos             (2)
#define EP_INT_EN_EP2IE                 (0x01U << EP_INT_EN_EP2IE_Pos)          ///< EP2 interrupt enable
#define EP_INT_EN_EP3IE_Pos             (3)
#define EP_INT_EN_EP3IE                 (0x01U << EP_INT_EN_EP3IE_Pos)          ///< EP3 interrupt enable
#define EP_INT_EN_EP4IE_Pos             (4)
#define EP_INT_EN_EP4IE                 (0x01U << EP_INT_EN_EP4IE_Pos)          ///< EP4 interrupt enable

////////////////////////////////////////////////////////////////////////////////
/// @brief EPn_INT_EN Register Bit Definition
////////////////////////////////////////////////////////////////////////////////
#define EPn_INT_EN_SETUPIE_Pos          (0)
#define EPn_INT_EN_SETUPIE              (0x01U << EPn_INT_EN_SETUPIE_Pos)       ///< SETUP packet interrupt enable
#define EPn_INT_EN_ENDIE_Pos            (1)
#define EPn_INT_EN_ENDIE                (0x01U << EPn_INT_EN_ENDIE_Pos)         ///< Status stage finished interrupt enable
#define EPn_INT_EN_INNACKIE_Pos         (2)
#define EPn_INT_EN_INNACKIE             (0x01U << EPn_INT_EN_INNACKIE_Pos)      ///< IN-NACK interrupt enable
#define EPn_INT_EN_INACKIE_Pos          (3)
#define EPn_INT_EN_INACKIE              (0x01U << EPn_INT_EN_INACKIE_Pos)       ///< IN-ACK interrupt enable
#define EPn_INT_EN_INSTALLIE_Pos        (4)
#define EPn_INT_EN_INSTALLIE            (0x01U << EPn_INT_EN_INSTALLIE_Pos)     ///< IN-STALL interrupt enable
#define EPn_INT_EN_OUTNACKIE_Pos        (5)
#define EPn_INT_EN_OUTNACKIE            (0x01U << EPn_INT_EN_OUTNACKIE_Pos)     ///< OUT-NACK interrupt enable
#define EPn_INT_EN_OUTACKIE_Pos         (6)
#define EPn_INT_EN_OUTACKIE             (0x01U << EPn_INT_EN_OUTACKIE_Pos)      ///< OUT-ACK interrupt enable
#define EPn_INT_EN_OUTSTALLIE_Pos       (7)
#define EPn_INT_EN_OUTSTALLIE           (0x01U << EPn_INT_EN_OUTSTALLIE_Pos)    ///< OUT-STALL interrupt enable

////////////////////////////////////////////////////////////////////////////////
/// @brief USB_ADDR Register Bit Definition
////////////////////////////////////////////////////////////////////////////////
#define USB_ADDR_ADDR_Pos               (0)
#define USB_ADDR_ADDR                   (0x7FU << USB_ADDR_ADDR_Pos)            ///< USB address

////////////////////////////////////////////////////////////////////////////////
/// @brief EP_EN Register Bit Definition
////////////////////////////////////////////////////////////////////////////////
#define EP_EN_EP0EN_Pos                 (0)
#define EP_EN_EP0EN                     (0x01U << EP_EN_EP0EN_Pos)              ///< Enable EP0
#define EP_EN_EP1EN_Pos                 (1)
#define EP_EN_EP1EN                     (0x01U << EP_EN_EP1EN_Pos)              ///< Enable EP1
#define EP_EN_EP2EN_Pos                 (2)
#define EP_EN_EP2EN                     (0x01U << EP_EN_EP2EN_Pos)              ///< Enable EP2
#define EP_EN_EP3EN_Pos                 (3)
#define EP_EN_EP3EN                     (0x01U << EP_EN_EP3EN_Pos)              ///< Enable EP3
#define EP_EN_EP4EN_Pos                 (4)
#define EP_EN_EP4EN                     (0x01U << EP_EN_EP4EN_Pos)              ///< Enable EP4

////////////////////////////////////////////////////////////////////////////////
/// @brief TOG_CTRL1_4 Register Bit Definition
////////////////////////////////////////////////////////////////////////////////
#define TOG_CTRL1_4_DTOG1_Pos           (0)
#define TOG_CTRL1_4_DTOG1               (0x01U << TOG_CTRL1_4_DTOG1_Pos)        ///< Set EP1 Toggle
#define TOG_CTRL1_4_DTOG1EN_Pos         (1)
#define TOG_CTRL1_4_DTOG1EN             (0x01U << TOG_CTRL1_4_DTOG1EN_Pos)      ///< Set EP1  Data Toggle Enable
#define TOG_CTRL1_4_DTOG2_Pos           (2)
#define TOG_CTRL1_4_DTOG2               (0x01U << TOG_CTRL1_4_DTOG2_Pos)        ///< Set EP2 Toggle
#define TOG_CTRL1_4_DTOG2EN_Pos         (3)
#define TOG_CTRL1_4_DTOG2EN             (0x01U << TOG_CTRL1_4_DTOG2EN_Pos)      ///< Set EP2  Data Toggle Enable
#define TOG_CTRL1_4_DTOG3_Pos           (4)
#define TOG_CTRL1_4_DTOG3               (0x01U << TOG_CTRL1_4_DTOG3_Pos)        ///< Set EP3 Toggle
#define TOG_CTRL1_4_DTOG3EN_Pos         (5)
#define TOG_CTRL1_4_DTOG3EN             (0x01U << TOG_CTRL1_4_DTOG3EN_Pos)      ///< Set EP3  Data Toggle Enable
#define TOG_CTRL1_4_DTOG4_Pos           (6)
#define TOG_CTRL1_4_DTOG4               (0x01U << TOG_CTRL1_4_DTOG4_Pos)        ///< Set EP4 Toggle
#define TOG_CTRL1_4_DTOG4EN_Pos         (7)
#define TOG_CTRL1_4_DTOG4EN             (0x01U << TOG_CTRL1_4_DTOG4EN_Pos)      ///< Set EP4  Data Toggle Enable

////////////////////////////////////////////////////////////////////////////////
/// @brief SETUPn Register Bit Definition
////////////////////////////////////////////////////////////////////////////////
#define SETUPn_Pos                      (0)
#define SETUPn                          (0xFFU << SETUPn_Pos)                   ///< Setup Data X

////////////////////////////////////////////////////////////////////////////////
/// @brief PACKET_SIZE0 Register Bit Definition
////////////////////////////////////////////////////////////////////////////////
#define PACKET_SIZE0_Pos                (0)
#define PACKET_SIZE0                    (0xFFU << PACKET_SIZE0_Pos)             ///< USB DMA Max Packet Size

////////////////////////////////////////////////////////////////////////////////
/// @brief PACKET_SIZE1 Register Bit Definition
////////////////////////////////////////////////////////////////////////////////
#define PACKET_SIZE1_Pos                (0)
#define PACKET_SIZE1                    (0xFFU << PACKET_SIZE1_Pos)             ///< USB DMA Max Packet Size

////////////////////////////////////////////////////////////////////////////////
/// @brief EPn_AVIL Register Bit Definition
////////////////////////////////////////////////////////////////////////////////
#define EPn_AVIL_EPXAVIL_Pos            (0)
#define EPn_AVIL_EPXAVIL                (0xFFU << EPn_AVIL_EPXAVIL_Pos)         ///< EPX FIFO available data number

////////////////////////////////////////////////////////////////////////////////
/// @brief EPn_CTRL Register Bit Definition
////////////////////////////////////////////////////////////////////////////////
#define EPn_CTRL_TRANCOUNT_Pos          (0)
#define EPn_CTRL_TRANCOUNT              (0x7FU << EPn_CTRL_TRANCOUNT_Pos)       ///< EPX transfer counter
#define EPn_CTRL_TRANEN_Pos             (7)
#define EPn_CTRL_TRANEN                 (0x01U << EPn_CTRL_TRANEN_Pos)          ///< EPX transfer enable

////////////////////////////////////////////////////////////////////////////////
/// @brief EPn_FIFO Register Bit Definition
////////////////////////////////////////////////////////////////////////////////
#define EPn_FIFO_EPn_FIFO_Pos           (0)
#define EPn_FIFO_EPn_FIFO               (0xFFU << EPn_FIFO_EPn_FIFO_Pos)        ///< EPX FIFO port

////////////////////////////////////////////////////////////////////////////////
/// @brief EP_DMA Register Bit Definition
////////////////////////////////////////////////////////////////////////////////
#define EP_DMA_DMA1EN_Pos               (0)
#define EP_DMA_DMA1EN                   (0x01U << EP_DMA_DMA1EN_Pos)            ///< EP1 DMA enable
#define EP_DMA_DMA2EN_Pos               (1)
#define EP_DMA_DMA2EN                   (0x01U << EP_DMA_DMA2EN_Pos)            ///< EP2 DMA enable

////////////////////////////////////////////////////////////////////////////////
/// @brief EP_HALT Register Bit Definition
////////////////////////////////////////////////////////////////////////////////
#define EP_HALT_HALT0_Pos               (0)
#define EP_HALT_HALT0                   (0x01U << EP_HALT_HALT0_Pos)            ///< EP0 halt
#define EP_HALT_HALT1_Pos               (1)
#define EP_HALT_HALT1                   (0x01U << EP_HALT_HALT1_Pos)            ///< EP1 halt
#define EP_HALT_HALT2_Pos               (2)
#define EP_HALT_HALT2                   (0x01U << EP_HALT_HALT2_Pos)            ///< EP2 halt
#define EP_HALT_HALT3_Pos               (3)
#define EP_HALT_HALT3                   (0x01U << EP_HALT_HALT3_Pos)            //< EP3 halt
#define EP_HALT_HALT4_Pos               (4)
#define EP_HALT_HALT4                   (0x01U << EP_HALT_HALT4_Pos)            ///< EP4 halt

////////////////////////////////////////////////////////////////////////////////
/// @brief USB_POWER Register Bit Definition
////////////////////////////////////////////////////////////////////////////////
#define USB_POWER_SUSPEN_Pos            (0)
#define USB_POWER_SUSPEN                (0x01U << USB_POWER_SUSPEN_Pos)         ///< BUS suspend enable bit
#define USB_POWER_SUSP_Pos              (1)
#define USB_POWER_SUSP                  (0x01U << USB_POWER_SUSP_Pos)           ///< suspend status
#define USB_POWER_WKUP_Pos              (3)
#define USB_POWER_WKUP                  (0x01U << USB_POWER_WKUP_Pos)           ///< Enable controller wake up from suspend state


////////////////////////////////////////////////////////////////////////////////
#include "mm32_reg_redefine_v1.h"
////////////////////////////////////////////////////////////////////////////////

/// @}

/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
