#ifndef __SPI_H__
#define __SPI_H__
#include "Spi_Cfg.h"
#include "Std_Types.h"
/********************************************************************************/
/********************************ENUM*******************************************/
/********************************************************************************/
//Services        :SWS_Spi_00373
//Name            :Spi_StatusType
//type            :Enumeration
//Description     :This type defines a range of specific status for SPI Handler/Driver
/********************************************************************************/
typedef enum{
	//The SPI Handler/Driver is not initialized or not usable
	SPI_UNINIT=0,
	//The SPI Handler/Driver is not currently transmitting any Job
	SPI_IDLE,
	// The SPI Handler/Driver is performing a SPI Job (transmit)
	SPI_BUSY,
}Spi_StatusType;

/********************************************************************************/
//Services        :SWS_Spi_00374
//Name            :Spi_JobResultType
//type            :Enumeration
//Description     :This type defines a range of specific Jobs status for SPI Handler/Driver
/********************************************************************************/
typedef enum{
	// The last transmission of the Job has failed
	SPI_JOB_FAILED=0,
	//The last transmission of the Job has been finished successfully
	SPI_JOB_OK ,
	// The SPI Handler/Driver is performing a SPI Job. The meaning of this status is equal to SPI_BUSY.
	SPI_JOB_PENDING,
	//An asynchronous transmit Job has been accepted, while
 //actual transmission for this Job has not started yet.
	SPI_JOB_QUEUED,
}Spi_JobResultType;
/********************************************************************************/
//Services        :SWS_Spi_00375
//Name            :Spi_SeqResultType
//type            :Enumeration
//Description     :This type defines a range of specific Sequences status for SPI Handler/Driver.
/********************************************************************************/
typedef enum{
	//The last transmission of the Sequence has failed.
 SPI_SEQ_FAILED=0,
//The last transmission of the Sequence has been finished successfully.
 SPI_SEQ_OK,
//The SPI Handler/Driver is performing a SPI Sequence. The
//meaning of this status is equal to SPI_BUSY.
 SPI_SEQ_PENDING,
//The last transmission of the Sequence has been canceled by user.
SPI_SEQ_CANCELED
}Spi_SeqResultType;
/********************************************************************************/
//Services        :SWS_Spi_00382
//Name            :Spi_AsyncModeType
//type            :Enumeration
//Description     :Specifies the asynchronous mechanism mode for SPI busses handled asynchronously in LEVEL 2.
/********************************************************************************/
typedef enum{
	//The asynchronous mechanism is ensured by polling, so
  //interrupts related to SPI busses handled asynchronously
 //are disabled.
	SPI_POLLING_MODE=0,
	//The asynchronous mechanism is ensured by interrupt, so
  //interrupts related to SPI busses handled asynchronously
  //are enabled.
	SPI_INTERRUPT_MODE
}Spi_AsyncModeType;
typedef enum{
	EB1=0,
	IB1=1
}SpiChannelType;
typedef enum{
	LSB=0,
	MSB
}SpiTransferStart;
typedef enum{
	ASYNCHRONOUS=0,
	SYNCHRONOUS
}SpiHwUnitSynchronous;
typedef enum{
	HIGH=1,
	LOW=0
}SpiCsPolarity;
typedef enum{
	CS_VIA_GPIO=0,
	CS_VIA_PERIPHERAL_ENGINE,
}SpiCsSelection;
typedef enum{
	LEADING=0,
	TRAILING
}SpiDataShiftEdge;
typedef enum{
	CSIB0=0,
	CSIB1,
	CSIB2,
	CSIB3,
}SpiHwUnit;
typedef enum{
	high=1,
	low=0
}SpiShiftClockIdleLevel;
/********************************************************************************/
//Services        :SWS_Spi_00376
//Name            :Spi_DataBufferType
//type            :uint8
//Description     :Type of application data buffer elements
/********************************************************************************/
typedef uint8_t  Spi_DataBufferType;

/********************************************************************************/
//Services        :SWS_Spi_00377
//Name            :Spi_NumberOfDataType
//type            :uint16
//Description     :Type for defining the number of data elements of the type Spi_DataBufferType to
//send and / or receive by Channel
/********************************************************************************/
typedef uint16_t Spi_NumberOfDataType;

/********************************************************************************/
//Services        :SWS_Spi_00378
//Name            :Spi_ChannelType
//type            :uint8
//Description     :Specifies the identification (ID) for a Channel
/********************************************************************************/
typedef uint8_t  Spi_ChannelType;
/********************************************************************************/
//Services        :SWS_Spi_00379
//Name            :Spi_JobType
//type            :uint16
//Description     :Specifies the identification (ID) for a Job.
/********************************************************************************/
typedef uint16_t Spi_JobType;

/********************************************************************************/
//Services        :SWS_Spi_00380
//Name            :Spi_SequenceType
//type            :uint16
//Description     :Specifies the identification (ID) for a Job.
/********************************************************************************/
typedef uint16_t Spi_SequenceType;

/********************************************************************************/
//Services        :SWS_Spi_00381
//Name            :Spi_HWUnitType
//type            :uint8
//Description     :Specifies the identification (ID) for a SPI Hardware microcontroller peripheral (unit).
/********************************************************************************/
typedef uint8_t  Spi_HWUnitType;

/*******************************************************************************/
//Services        :SWS_Spi_00372
//Name            :Spi_ConfigType
//type            :Structure
//Description     :This type of the external data structure shall contain the initialization data for the
//                  SPI Handler/Driver.
//Range           :Implementation Specific
/********************************************************************************/
typedef struct{
	Spi_DataBufferType pBufferTX;
	Spi_DataBufferType pBufferRX;
}Spi_BufferDescriptorType;
typedef struct{
	uint8_t SpiMaxChannel;
	uint16_t SpiMaxJob;
	uint8_t SpiMaxSequence;
	uint8_t SpiMaxLength;
	Spi_BufferDescriptorType *pcBufferDescriptor;
}Spi_ConfigType;
extern Spi_ConfigType *Spi_Config;
typedef struct{
	//SPI Channel ID, used as parameter in SPI API functions
	uint8_t SpiChannelId;
	//Buffer usage with EB/IB channel
	SpiChannelType *ChannelType; 
	//This parameter is the width of a transmitted data unit.
	uint8_t SpiDataWidth;
	//The default data to be transmitted when (for internal buffer or external
 //buffer) the pointer passed to Spi_WriteIB (for internal buffer) or to
 //Spi_SetupEB (for external buffer) is NULL
	uint16_t SpiDefaultData;
	//This parameter contains the maximum size (number of data elements) of
 //data buffers in case of EB Channels and only.
	uint16_t SpiEbMaxLength;
	//This parameter contains the maximum number of data buffers in case of IB
  //Channels and only.
	uint16_t SpiIbNBuffers;
	//This parameter defines the first starting bit for transmission.
	SpiTransferStart *Start; 
}SpiChannel;
extern SpiChannel *Channeltype;
typedef struct{
	int SpiBaudrate;
	char SpiCsIdentifier;
	SpiCsPolarity CsPolarity;
	SpiCsSelection CsSelection;
	SpiDataShiftEdge ShiftEdge;
	int SpiEnableCs;
	SpiHwUnit Unit;
	SpiShiftClockIdleLevel ShiftClockIdleLevel;
	float SpiTimeClk2Cs;
}SpiExternalDevice;
typedef struct{
	SpiHwUnitSynchronous HwUnitSynchronous;
	int SpiJobEndNotification;
	int SpiJobId;
	int SpiJobPriority;
	SpiExternalDevice SpiDeviceAssignment;
}SpiJob;
extern SpiJob *Jobtype;
typedef struct{
	//SPI Sequence ID, used as parameter in SPI API functions.
	uint8_t SpiSequenceId;
	//A sequence references several jobs, which are executed during a communication sequence
	SpiJob SpiJobAssignment;
}SpiSequence;
extern SpiSequence *Sequencetype;
//Enable interrupts
void irq_Enable(void);
//Disable interrupts
void irq_Disable(void);

void Spi_Init(const Spi_ConfigType* ConfigPtr);
Std_ReturnType Spi_DeInit(void);
Std_ReturnType Spi_WriteIB(Spi_ChannelType Channel,const Spi_DataBufferType* DataBufferPtr);
Std_ReturnType Spi_AsyncTransmit(Spi_SequenceType Sequence);
Std_ReturnType Spi_ReadIB(Spi_ChannelType Channel,Spi_DataBufferType* DataBufferPointer);
Std_ReturnType Spi_SetupEB(Spi_ChannelType Channel,const Spi_DataBufferType* SrcDataBufferPtr,Spi_DataBufferType* DesDataBufferPtr,Spi_NumberOfDataType Length);
Spi_StatusType Spi_GetStatus(void);
Spi_JobResultType Spi_GetJobResult(Spi_JobType Job);
Spi_SeqResultType Spi_GetSequenceResult(Spi_SequenceType Sequence);
void Spi_GetVersionInfo(Std_VersionInfoType* versioninfo);
Std_ReturnType Spi_SyncTransmit(Spi_SequenceType Sequence);
Spi_StatusType Spi_GetHWUnitStatus(Spi_HWUnitType HWUnit);
void Spi_Cancel(Spi_SequenceType Sequence);
Std_ReturnType Spi_SetAsyncMode(Spi_AsyncModeType Mode);

#endif
