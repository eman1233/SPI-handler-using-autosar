#include "Spi_MemMap.h"
#include "Spi.h"
#include "SchM_Spi.h"
#include "Det.h"
#include "Dem.h"
#include "Std_Types.h"
#define NULL ((void*)0)
#define SPI_CHANNEL_FLAG_TX_DEFAULT_U8  0x01u
#define SPI_JOB_PRIORITY_LEVELS_COUNT    (4)
#define SPI_MAX_HWUNIT 32u
#define Max_Seq 4u
#define MAX_JOB 32u
typedef struct{
	Spi_JobType aScheduledJobsListHead[SPI_JOB_PRIORITY_LEVELS_COUNT];
	Spi_JobType aScheduledJobsListTail[SPI_JOB_PRIORITY_LEVELS_COUNT];
	uint8_t MaxScheduledPriority;
	Spi_StatusType status_type;
}Spi_HWUnitQueue;
typedef struct{
	Spi_SeqResultType Result;
	Spi_JobType *pcJobIndexList[MAX_JOB];
}Spi_SequenceStateType;
typedef struct{
	Spi_HWUnitType Status;
}Spi_HWStateType;
typedef struct{
	Spi_HWUnitType Res;
}Spi_JobStateType;
static Spi_SequenceStateType*Spi_aSpiSequence;
static Spi_SequenceStateType *Spi_aSpiSequenceState[Max_Seq];
static Spi_HWStateType *Spi_aSpiHWState[SPI_MAX_HWUNIT];
static Spi_JobStateType *Spi_aSpiJobState[MAX_JOB];
static Spi_HWUnitQueue *Spi_aSpiHWQueue[SPI_MAX_HWUNIT];
/*******************************************************************************/
//Services            :[SWS_Spi_00177]
//Name                :Spi_WriteIB
//Service ID          :0x02
//Sync/Async          :Synchronous
//Reentrancy          :Reentrant
//Parameters (in)     :Channel
//                    :DataBufferPtr
//Parameters (inout)  :None
//Parameters (out)    :None
//Return value        :Std_ReturnType
//Description         :Service for writing one or more data to an IB SPI Handler/Driver Channel specified
//                    by parameter
/********************************************************************************/
//The function Spi_WriteIB shall be pre-compile time configurable
//by the parameter SpiChannelBuffersAllowed. This function is only 
//relevant for Channels with IB 
Std_ReturnType Spi_WriteIB(Spi_ChannelType Channel,const Spi_DataBufferType* DataBufferPtr){
	static Std_ReturnType status;
	static Spi_SequenceType Sequence;
	static Spi_JobType Job;
	static  uint16_t Length;
	static Spi_DataBufferType *pDataBufferDes;
	static Spi_DataBufferType *pDataBufferSrc;
	pDataBufferDes=Spi_Config->pcBufferDescriptor->pBufferTX;
	pDataBufferSrc=DataBufferPtr;
	for(uint16_t index=0;index<Spi_Config->SpiMaxLength;index++){
		*pDataBufferDes=*pDataBufferSrc;
		pDataBufferSrc++;
		pDataBufferDes++;
		Channeltype->SpiDefaultData|=(uint8_t)(Channeltype->SpiDefaultData)&(~SPI_CHANNEL_FLAG_TX_DEFAULT_U8);
	}
		//[SWS_Spi_00023] ? If the given parameter “DataBufferPtr” is null, the function
  //Spi_WriteIB shall assume that the data to be transmitted is not relevant and the
  //default transmit value of the given channel shall be used instead.
	if(NULL==DataBufferPtr){
		Channeltype->SpiDefaultData|=(uint8_t)SPI_CHANNEL_FLAG_TX_DEFAULT_U8;
	}
	#if (SpiDevErrorDetect==STD_ON)
		//[SWS_Spi_00031] ? The API parameter Channel shall have a value within
 //the defined channels in the initialization data structure, and the correct type of channel (IB
 //or EB) has to be used with services. Related error value: SPI_E_PARAM_CHANNEL.
 //Otherwise, the service is not done and the return value shall be E_NOT_OK.
	if(Channel>Spi_Config->SpiMaxChannel){
		Det_ReportError(SpiID, instanceID, Spi_WriteIB_API, SPI_E_PARAM_CHANNEL);
		status= E_NOT_OK;
	}
	//[SWS_Spi_00032] ? The API parameters Sequence and Job shall have values
  //within the specified range of values. Related errors values: SPI_E_PARAM_SEQ or
  //SPI_E_PARAM_JOB.
		else if(Sequence>Spi_Config->SpiMaxSequence){
		 Det_ReportError(SpiID, instanceID, Spi_WriteIB_API, SPI_E_PARAM_SEQ);
		 status= E_NOT_OK;
	}
	 else if(Job>Spi_Config->SpiMaxJob){
		Det_ReportError(SpiID, instanceID, Spi_WriteIB_API, SPI_E_PARAM_JOB);
		status= E_NOT_OK;
	}
	//[SWS_Spi_00254] ? If the Sequence and Job related service is not
//done and, depending on services, either the return value shall be E_NOT_OK or a failed result
 //(SPI_JOB_FAILED or SPI_SEQ_FAILED)
	else if(NULL==Spi_Config){
		Det_ReportError(SpiID, instanceID, Spi_WriteIB_API, SPI_JOB_FAILED);
		Det_ReportError(SpiID, instanceID, Spi_WriteIB_API, SPI_SEQ_FAILED);
		Det_ReportError(SpiID, instanceID, Spi_WriteIB_API, SPI_E_UNINIT);
		status= E_NOT_OK;
	}
	//[SWS_Spi_00060] ? The API parameter Length of data shall have a value within
  //the specified buffer maximum value. Related error value:
  //SPI_E_PARAM_LENGTH.
	else if(Length>Spi_Config->SpiMaxLength){
		Det_ReportError(SpiID, instanceID, Spi_WriteIB_API, SPI_E_PARAM_LENGTH);
		status= E_NOT_OK;
	}
	//[SWS_Spi_00024] ? The function Spi_WriteIB shall take over the given parameters,
	//and save the pointed data to the internal buffer defined with the function
   //Spi_Init.
	if(SpiChannelBuffersAllowed==IB)
		status=E_OK;
	#endif
		
	return status;
}

/*******************************************************************************/
//Services            :[SWS_Spi_00182]
//Name                :Spi_GetJobResult
//Service ID          :0x07
//Sync/Async          :Synchronous
//Reentrancy          :Reentrant
//Parameters (in)     :Job
//Parameters (inout)  :None
//Parameters (out)    :None
//Return value        :Spi_JobResultType
//Description         :This service returns the last transmission result of the specified Job
/********************************************************************************/
Spi_JobResultType Spi_GetJobResult(Spi_JobType Job){
	static Spi_ChannelType Channel;
	static  uint16_t Length;
	static Spi_JobResultType status; 
	static Spi_SequenceType Sequence;
	//[SWS_Spi_00026] ? The function Spi_GetJobResult shall return
  //the last transmission result of the specified Job.
	status=Spi_aSpiJobState[Job]->Res;
	//[SWS_Spi_00038] ? The SPI Handler/Driver’s environment shall call the function
  //Spi_GetJobResult to inquire whether the Job transmission has succeeded
 //(SPI_JOB_OK) or failed (SPI_JOB_FAILED)
	if(NULL==(Spi_Config))
		status=SPI_JOB_FAILED;
	else
		status=SPI_JOB_OK;
	#if (SpiDevErrorDetect==STD_ON)
		//[SWS_Spi_00031] ? The API parameter Channel shall have a value within
 //the defined channels in the initialization data structure, and the correct type of channel (IB
 //or EB) has to be used with services. Related error value: SPI_E_PARAM_CHANNEL.
 //Otherwise, the service is not done and the return value shall be E_NOT_OK.
	if(Channel>Spi_Config->SpiMaxChannel){
		Det_ReportError(SpiID, instanceID, Spi_GetJobResult_API, SPI_E_PARAM_CHANNEL);
		
	}
	//[SWS_Spi_00032] ? The API parameters Sequence and Job shall have values
  //within the specified range of values. Related errors values: SPI_E_PARAM_SEQ or
  //SPI_E_PARAM_JOB.
		else if(Sequence>Spi_Config->SpiMaxSequence){
		 Det_ReportError(SpiID, instanceID, Spi_GetJobResult_API, SPI_E_PARAM_SEQ);
		 
	}
	 else if(Job>Spi_Config->SpiMaxJob){
		Det_ReportError(SpiID, instanceID, Spi_GetJobResult_API, SPI_E_PARAM_JOB);
		
	}
	//[SWS_Spi_00254] ? If the Sequence and Job related service is not
//done and, depending on services, either the return value shall be E_NOT_OK or a failed result
 //(SPI_JOB_FAILED or SPI_SEQ_FAILED)
	else if(NULL==Spi_Config){
		Det_ReportError(SpiID, instanceID, Spi_GetJobResult_API, SPI_JOB_FAILED);
		Det_ReportError(SpiID, instanceID, Spi_GetJobResult_API, SPI_SEQ_FAILED);
		Det_ReportError(SpiID, instanceID, Spi_GetJobResult_API, SPI_E_UNINIT);
	
	}
	//[SWS_Spi_00060] ? The API parameter Length of data shall have a value within
  //the specified buffer maximum value. Related error value:
  //SPI_E_PARAM_LENGTH.
	else if(Length>Spi_Config->SpiMaxLength){
		Det_ReportError(SpiID, instanceID, Spi_GetJobResult_API, SPI_E_PARAM_LENGTH);
		
	}
	#endif
	return status;
}

/*******************************************************************************/
//Services            :[SWS_Spi_00182]
//Name                :Spi_GetJobResult
//Service ID          :0x07
//Sync/Async          :Synchronous
//Reentrancy          :Reentrant
//Parameters (in)     :Job
//Parameters (inout)  :None
//Parameters (out)    :None
//Return value        :Spi_JobResultType
//Description         :This service returns the last transmission result of the specified Job
/********************************************************************************/
Std_ReturnType Spi_AsyncTransmit(Spi_SequenceType Sequence){
	static Std_ReturnType status;
	static Spi_HWUnitType HWUnit;
	static Spi_JobType Jobindex;
	static Spi_JobType NumJobsInSequence;
	static Spi_JobType pcJobCount;
	//[SWS_Spi_00020] ? The function Spi_AsyncTransmit shall take over the given
  //parameter, initiate a transmission, set the SPI Handler/Driver status to SPI_BUSY,
	//set the sequence result to SPI_SEQ_PENDING and return.
	Spi_aSpiSequenceState[Sequence]->Result=SPI_SEQ_PENDING;
	Spi_aSpiHWQueue[HWUnit]->status_type=SPI_BUSY;
	//[SWS_Spi_00194] ? When the function Spi_AsyncTransmit is called, shall take over
  //the given parameter and set the Job status to SPI_JOB_QUEUED,
	//which can be obtained by calling the API service Spi_GetJobResult
	//Spi_aSpiHWState[Job]->Status=SPI_JOB_QUEUED;
	for(Jobindex=0u;Jobindex< NumJobsInSequence;Jobindex++){
		pcJobCount=(*Spi_aSpiSequence->pcJobIndexList)[Jobindex];
		Spi_aSpiJobState[pcJobCount]->Res=SPI_JOB_QUEUED;
	}
	return status;
}

/*******************************************************************************/
//Services            :[SWS_Spi_00183]
//Name                :Spi_GetSequenceResult
//Service ID          :0x08
//Sync/Async          :Synchronous
//Reentrancy          :Reentrant
//Parameters (in)     :Sequence
//Parameters (inout)  :None
//Parameters (out)    :None
//Return value        :Spi_SeqResultType
//Description         :This service returns the last transmission result of the specified Sequance
/********************************************************************************/
Spi_SeqResultType Spi_GetSequenceResult(Spi_SequenceType Sequence){
	static Spi_ChannelType Channel;
	static Spi_JobType Job;
	static  uint16_t Length;
	static Spi_SeqResultType status; 
  //[SWS_Spi_00324] ? The function Spi_GetSequenceResult shall return the last
  //transmission result of the specified Sequence
	status=Spi_aSpiSequenceState[Sequence]->Result;
  //[SWS_Spi_00042] ? The SPI Handler/Driver’s environment shall call the function
  //Spi_GetSequenceResult to inquire whether the full Sequence transmission has
  //succeeded (SPI_SEQ_OK) or failed (SPI_SEQ_FAILED).
	if(NULL==(Spi_Config))
		status=SPI_SEQ_FAILED;
	else
		status=SPI_SEQ_OK;
	#if (SpiDevErrorDetect==STD_ON)
		//[SWS_Spi_00031] ? The API parameter Channel shall have a value within
 //the defined channels in the initialization data structure, and the correct type of channel (IB
 //or EB) has to be used with services. Related error value: SPI_E_PARAM_CHANNEL.
 //Otherwise, the service is not done and the return value shall be E_NOT_OK.
	if(Channel>Spi_Config->SpiMaxChannel){
		Det_ReportError(SpiID, instanceID, Spi_GetSequenceResult_API, SPI_E_PARAM_CHANNEL);
		
	}
	//[SWS_Spi_00032] ? The API parameters Sequence and Job shall have values
  //within the specified range of values. Related errors values: SPI_E_PARAM_SEQ or
  //SPI_E_PARAM_JOB.
		else if(Sequence>Spi_Config->SpiMaxSequence){
		 Det_ReportError(SpiID, instanceID, Spi_GetSequenceResult_API, SPI_E_PARAM_SEQ);
		 
	}
	 else if(Job>Spi_Config->SpiMaxJob){
		Det_ReportError(SpiID, instanceID, Spi_GetSequenceResult_API, SPI_E_PARAM_JOB);
		
	}
	//[SWS_Spi_00254] ? If the Sequence and Job related service is not
//done and, depending on services, either the return value shall be E_NOT_OK or a failed result
 //(SPI_JOB_FAILED or SPI_SEQ_FAILED)
	else if(NULL==Spi_Config){
		Det_ReportError(SpiID, instanceID, Spi_GetSequenceResult_API, SPI_JOB_FAILED);
		Det_ReportError(SpiID, instanceID, Spi_GetSequenceResult_API, SPI_SEQ_FAILED);
		Det_ReportError(SpiID, instanceID, Spi_GetSequenceResult_API, SPI_E_UNINIT);
	
	}
	//[SWS_Spi_00060] ? The API parameter Length of data shall have a value within
  //the specified buffer maximum value. Related error value:
  //SPI_E_PARAM_LENGTH.
	else if(Length>Spi_Config->SpiMaxLength){
		Det_ReportError(SpiID, instanceID, Spi_GetSequenceResult_API, SPI_E_PARAM_LENGTH);
		
	}
	#endif
	return status;
}

/*******************************************************************************/
//Services            :[SWS_Spi_00186]
//Name                :Spi_GetHWUnitStatus
//Service ID          :0x0b
//Sync/Async          :Synchronous
//Reentrancy          :Reentrant
//Parameters (in)     :HWUnit
//Parameters (inout)  :None
//Parameters (out)    :None
//Return value        :Spi_StatusType
//Description         :This service returns the status of the specified SPI Hardware microcontroller peripheral
/********************************************************************************/
//[SWS_Spi_00142] ? The function Spi_GetHWUnitStatus is pre-compile time
//configurable On / Off by the configuration parameter SpiHwStatusApi.?
#if(SpiHwStatusApi==STD_ON)
Spi_StatusType Spi_GetHWUnitStatus(Spi_HWUnitType HWUnit){
	static Spi_ChannelType Channel;
	static Spi_StatusType status;
	static Spi_SequenceType Sequence;
	static Spi_JobType Job;
	static  uint16_t Length;
  //[SWS_Spi_00332] ? The function Spi_GetHWUnitStatus service returns the status
  //of the specified SPI Hardware microcontroller peripheral.
	status=Spi_aSpiHWState[HWUnit]->Status;
	#if (SpiDevErrorDetect==STD_ON)
		//[SWS_Spi_00031] ? The API parameter Channel shall have a value within
 //the defined channels in the initialization data structure, and the correct type of channel (IB
 //or EB) has to be used with services. Related error value: SPI_E_PARAM_CHANNEL.
 //Otherwise, the service is not done and the return value shall be E_NOT_OK.
	if(Channel>Spi_Config->SpiMaxChannel){
		Det_ReportError(SpiID, instanceID, Spi_GetHWUnitStatus_API, SPI_E_PARAM_CHANNEL);
		
	}
	//[SWS_Spi_00032] ? The API parameters Sequence and Job shall have values
  //within the specified range of values. Related errors values: SPI_E_PARAM_SEQ or
  //SPI_E_PARAM_JOB.
		else if(Sequence>Spi_Config->SpiMaxSequence){
		 Det_ReportError(SpiID, instanceID, Spi_GetHWUnitStatus_API, SPI_E_PARAM_SEQ);
		 
	}
	 else if(Job>Spi_Config->SpiMaxJob){
		Det_ReportError(SpiID, instanceID, Spi_GetHWUnitStatus_API, SPI_E_PARAM_JOB);
		
	}
	//[SWS_Spi_00254] ? If the Sequence and Job related service is not
//done and, depending on services, either the return value shall be E_NOT_OK or a failed result
 //(SPI_JOB_FAILED or SPI_SEQ_FAILED)
	else if(NULL==Spi_Config){
		Det_ReportError(SpiID, instanceID, Spi_GetHWUnitStatus_API, SPI_JOB_FAILED);
		Det_ReportError(SpiID, instanceID, Spi_GetHWUnitStatus_API, SPI_SEQ_FAILED);
		Det_ReportError(SpiID, instanceID, Spi_GetHWUnitStatus_API, SPI_E_UNINIT);
	
	}
	//[SWS_Spi_00060] ? The API parameter Length of data shall have a value within
  //the specified buffer maximum value. Related error value:
  //SPI_E_PARAM_LENGTH.
	else if(Length>Spi_Config->SpiMaxLength){
		Det_ReportError(SpiID, instanceID, Spi_GetHWUnitStatus_API, SPI_E_PARAM_LENGTH);
		
	}
	#endif
	return status;
}
#endif

/*******************************************************************************/
//Services            :[SWS_Spi_00187]
//Name                :Spi_Cancel
//Service ID          :0x0c
//Sync/Async          :Asynchronous
//Reentrancy          :Reentrant
//Parameters (in)     :Sequence
//Parameters (inout)  :None
//Parameters (out)    :None
//Return value        :None
//Description         :Service cancels the specified on-going sequence transmission
/********************************************************************************/
//[SWS_Spi_00334] ? The function Spi_Cancel service cancels the specified ongoing sequence transmission.
#if(SpiCancelApi==STD_ON)
void Spi_Cancel(Spi_SequenceType Sequence){
	//[SWS_Spi_00144] ? The function Spi_Cancel shall cancel the specified on-going
 //sequence transmission without cancelling any Job transmission and set the sequence result to SPI_SEQ_CANCELLED.
 Spi_aSpiSequenceState[Sequence]->Result=SPI_SEQ_CANCELED;
}
#endif
/*******************************************************************************/
//Services            :[SWS_Spi_00188]
//Name                :Spi_SetAsyncMode
//Service ID          :0x0d
//Sync/Async          :Synchronous
//Reentrancy          :Non Reentrant
//Parameters (in)     :Mode
//Parameters (inout)  :None
//Parameters (out)    :None
//Return value        :Std_ReturnType
//Description         :Service to set the asynchronous mechanism mode for SPI busses handled asynchronously.
/********************************************************************************/
//[SWS_Spi_00154] ? The function Spi_SetAsyncMode is pre-compile time 
//selectable by the configuration parameter SpiLevelDelivered. This function is only
//relevant for LEVEL 2.
#if(SpiLevelDelivered==LEVEL2)
Std_ReturnType Spi_SetAsyncMode(Spi_AsyncModeType Mode){
	static Std_ReturnType status;
	static Spi_StatusType StatusType;
	//[SWS_Spi_00152] ? The function Spi_SetAsyncMode according to the given parameter 
	//shall set the asynchronous mechanism mode for SPI channels configured to
  //behave asynchronously.
	for(Spi_HWUnitType HWunit=0u;HWunit<(Spi_HWUnitType)SPI_MAX_HWUNIT;HWunit++){
			HWunit=Mode;
	}
  //[SWS_Spi_00171] ? If the function Spi_SetAsyncMode is called while the SPI
  //Handler/Driver status is SPI_BUSY and an asynchronous transmission is in progress,
  //the SPI Handler/Driver shall not change the AsyncModeType and keep the mode
 //type as it is. The function shall return the value E_NOT_OK.	
	if(StatusType==SPI_BUSY){
		status=E_NOT_OK;
	}
	//[SWS_Spi_00172] ? If Spi_SetAsyncMode is called while a synchronous transmission is in progress,
	//the SPI Handler/Driver shall set the AsyncModeType according to parameter 'Mode',
	//even if the SPI Handler/Driver status is SPI_BUSY. The
  //function shall return the value E_OK.
	if(Mode==SPI_INTERRUPT_MODE){
		status=E_OK;
	}
	return status;
}
#endif
