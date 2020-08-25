#ifndef __STD_TYPES_H__
#define __STD_TYPES_H__
#include "Platform_Types.h"
#include "Compiler.h"
#include <stdint.h>
/********************************************************************************/
//Name            :Std_ReturnType
//Derived from    :uint8
//Description     :STD005: This type can be used as standard API return type which is shared
//                 between the RTE and the BSW modules. It shall be defined as follows:
//                 typedef uint8 Std_ReturnType;
//                 STD011:, The Std_ReturnType shall normally be used with value E_OK or
//                 E_NOT_OK. If those return values are not sufficient user specific values can be
//                 defined by using the 6 least specific bits.
//                 Layout of the Std_ReturnType shall be as stated in the RTE specification. Bit 7
//                 and Bit 8 are reserved and defined by the RTE specification.
/********************************************************************************/
typedef enum{
  E_OK=1,
  E_NOT_OK=0,
} Std_ReturnType;


/********************************************************************************/
//Name            :Std_ReturnType
//Derived from    :structure
//Description     :STD015 : This type shall be used to request the version of a
//                 BSW module using the <Module name>_GetVersionInfo() function.
/********************************************************************************/
typedef struct
{
uint16_t vendorID;
uint16_t moduleID;
uint8_t instanceID;
uint8_t sw_major_version;
uint8_t sw_minor_version;
uint8_t sw_patch_version;
} Std_VersionInfoType;
typedef uint8_t API;
#define Spi_Init_API                      ((API)0x00)
#define Spi_DeInit_API                    ((API)0x01)
#define Spi_WriteIB_API                   ((API)0x02)
#define Spi_AsyncTransmit_API             ((API)0x03)
#define Spi_ReadIB_API                    ((API)0x04)
#define Spi_SetupEB_API                   ((API)0x05)
#define Spi_GetStatus_API                 ((API)0x06)
#define Spi_GetJobResult_API              ((API)0x07)
#define Spi_GetSequenceResult_API         ((API)0x08)
#define Spi_GetVersionInfo_API            ((API)0x09)
#define Spi_SyncTransmit_API              ((API)0x0a)
#define Spi_GetHWUnitStatus_API           ((API)0x0b)
#define Spi_Cancel_API                    ((API)0x0c)
#define Spi_SetAsyncMode_API              ((API)0x0d)
#define SpiID                             (83)
#define instanceID                        (0U)
//Development Errors
#define SPI_E_PARAM_CHANNEL               (0x0A)
#define SPI_E_PARAM_JOB                   (0x0B)
#define SPI_E_PARAM_SEQ                   (0x0C)
#define SPI_E_PARAM_LENGTH                (0x0D)
#define SPI_E_PARAM_UNIT                  (0x0E)
#define SPI_E_PARAM_POINTER               (0x10)
#define SPI_E_UNINIT                      (0x1A)
#define SPI_E_ALREADY_INITIALIZED         (0x4A)
//Runtime Errors
#define SPI_E_SEQ_PENDING                 (0x2A)
#define SPI_E_SEQ_IN_PROCESS              (0x3A)
#endif
