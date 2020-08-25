#ifndef __SPI_CFG_H__
#define __SPI_CFG_H__
#define STD_ON                            (1U)
#define STD_OFF                           (0U)
#define SSI0_ID                           (0U)
#define SSI1_ID                           (1U)
#define SSI2_ID                           (2U)
#define SSI3_ID                           (3U)
#define SPI_E_HARDWARE_ERROR              STD_ON
#define SpiCancelApi                      STD_ON
#define IB                                (0U)
#define EB                                (1U)
#define IB_EB                             (2U)
#define SpiChannelBuffersAllowed          IB
#define SpiDevErrorDetect                 STD_ON
#define SpiHwStatusApi                    STD_ON
#define SpiInterruptibleSeqAllowed        STD_ON
#define LEVEL0                            (0U)
#define LEVEL1                            (1U)
#define LEVEL2                            (2U)
#define SpiLevelDelivered                 LEVEL2
#define SpiMainFunctionPeriod             STD_OFF
#define SpiSupportConcurrentSyncTransmit  STD_ON
#define SpiVersionInfoApi                 STD_ON
#define SpiInterruptibleSequence          STD_OFF
#define SpiSeqEndNotification             STD_ON
#define Spi0SequenceId                    (0U)
#define Spi1SequenceId                    (1U)
#define Spi2SequenceId                    (2U)
#define Spi3SequenceId                    (3U)
#define Spi0ChannelId                     (0U)
#define Spi1ChannelId                     (1U)
#define Spi2ChannelId                     (2U)
#define Spi3ChannelId                     (3U)
#endif
