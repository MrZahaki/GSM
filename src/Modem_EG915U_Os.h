
/* -----------------------------------------------------------------------------
 * Copyright (c) 2019-2022 Hussein zahaki. All rights reserved.
 * 
 * Based on the ARM::CMSIS driver softwares.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 * $Date:        5. April 2022
 *
 * Project:      GSM
 * -------------------------------------------------------------------------- */
#ifndef MOD_EG915U_OS_H__
#define MOD_EG915U_OS_H__

#include "cmsis_os2.h"
#include "cmsis_compiler.h"
#include "RTE_Components.h"

#include "Modem_EG915U_Config.h"

/* Convert priority number to CMSIS-RTOS2 priority value */
#define CMSIS_RTOS2_PRIORITY(n)      \
 ((n == 0) ? osPriorityLow         : \
  (n == 1) ? osPriorityBelowNormal : \
  (n == 2) ? osPriorityNormal      : \
  (n == 3) ? osPriorityAboveNormal : \
  (n == 4) ? osPriorityHigh        : \
  (n == 5) ? osPriorityRealtime    : osPriorityError)

/* Modem parser thread priority and stack size */
#define MOD_THREAD_PRIORITY          CMSIS_RTOS2_PRIORITY(MOD_EG915U_THREAD_PRIORITY)
#define MOD_THREAD_STACK_SIZE        MOD_EG915U_THREAD_STACK_SIZE

/* Socket buffer memory pool: (size of one block * total number of block) */
#define SOCKET_BUFFER_BLOCK_SIZE      MOD_EG915U_SOCKET_BLOCK_SIZE
#define SOCKET_BUFFER_BLOCK_COUNT     MOD_EG915U_SOCKET_BLOCK_COUNT

/* Serial parser buffer memory pool: (size of one block * total number of block) */
#define PARSER_BUFFER_BLOCK_SIZE      MOD_EG915U_PARSER_BLOCK_SIZE
#define PARSER_BUFFER_BLOCK_COUNT     MOD_EG915U_PARSER_BLOCK_COUNT


#if defined(RTE_CMSIS_RTOS2_RTX5)
  #include "rtx_os.h"
  
  /* Thread sizes */
  #define OS_THREAD_CB_SIZE                       osRtxThreadCbSize

  /* Memory Pool sizes */
  #define OS_MEMPOOL_CB_SIZE                      osRtxMemoryPoolCbSize
  #define OS_MEMPOOL_MEM_SIZE(bl_count, bl_size)  osRtxMemoryPoolMemSize(bl_count, bl_size)

  /* Mutex control block size */
  #define OS_MUTEX_CB_SIZE                        osRtxMutexCbSize

  /* Event Flags control block size */
  #define OS_EVENTFLAGS_CB_SIZE                   osRtxEventFlagsCbSize

#elif defined(RTE_CMSIS_RTOS2_FreeRTOS)
  #include "FreeRTOS.h"

  #define OS_THREAD_CB_SIZE                       (sizeof(StaticTask_t))

  /* Define block space required to create block of required size */
  #define MPOOL_BLOCK_SPACE(bl_count, bl_size)  (((((bl_size) + (4 - 1)) / 4) * 4)*(bl_count))

  /* Memory Pool sizes */
  #define OS_MEMPOOL_CB_SIZE                      ((8 * 4) + sizeof(StaticSemaphore_t))
  #define OS_MEMPOOL_MEM_SIZE(bl_count, bl_size)  MPOOL_BLOCK_SPACE(bl_count, bl_size)

  /* Mutex control block size */
  #define OS_MUTEX_CB_SIZE                        (sizeof(StaticSemaphore_t))

  /* Event Flags control block size */
  #define OS_EVENTFLAGS_CB_SIZE                   (sizeof(StaticEventGroup_t))
#endif

/* Thread for pooling and parsing engine execution */
extern const osThreadAttr_t     Modem_Thread_Attr;

/* Event flags for signaling events */
extern const osEventFlagsAttr_t Modem_EventFlags_Attr;

/* Mutex for socket access protection */
extern const osMutexAttr_t      Socket_Mutex_Attr;

/* Memory pool for socket data storage */
extern const osMemoryPoolAttr_t Socket_MemPool_Attr;

/* Memory pool for serial parser */
extern const osMemoryPoolAttr_t AT_Parser_MemPool_Attr;

/* Memory access mutex */
extern const osMutexAttr_t      BufList_Mutex_Attr;

#endif /* MOD_EG915U_OS_H__ */
