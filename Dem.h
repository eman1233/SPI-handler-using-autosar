#ifndef __DEM_H__
#define __DEM_H__
#include <stdint.h>
/********************************************************************************/
//Services        :SWS_Dem_00925
//Name            :Dem_EventIdType
//Derived from    :uint16
//Description     :Identification of an event by assigned EventId. The EventId is
//                   assigned by the Dem.
//Range           :1..65535
/********************************************************************************/
typedef uint16_t Dem_EventIdType;

/********************************************************************************/
//Services        :SWS_Dem_00926
//Name            :Dem_EventStatusType
//Derived from    :uint8
//Description     :This type contains all monitor test result values, which can be
//                 reported via Dem_SetEventStatus()..
/********************************************************************************/
typedef uint8_t Dem_EventStatusType;
//Monitor reports qualified test result passed
#define DEM_EVENT_STATUS_PASSED                 ((Dem_EventStatusType)0x00)
//Monitor reports qualified test result failed
#define DEM_EVENT_STATUS_FAILED                 ((Dem_EventStatusType)0x01)
//Monitor reports non-qualified test result pre-passed (debounced Dem-internally).
#define DEM_EVENT_STATUS_PREPASSED              ((Dem_EventStatusType)0x02)
//Monitor reports non-qualified test result pre-failed (debounced Dem-internally).
#define DEM_EVENT_STATUS_PREFAILED              ((Dem_EventStatusType)0x03)
//Monitor triggers the storage of ExtendedDataRecords and FreezeFrames ON_FDC_THRESHOLD
#define DEM_EVENT_STATUS_FDC_THRESHOLD_REACHED  ((Dem_EventStatusType)0x04)

#endif
