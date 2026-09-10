/** 
  ******************************************************************************
  * @file    hardware_u5_peripheral_spi.cpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    June 4, 2026
  * @brief   SPI Peripheral Driver for STM32U5 series.
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2026 Gremsy. All rights reserved.
  *
  ******************************************************************************
 */

#include "Hardware/Stm32U5/Inc/hardware_u5_peripheral_spi.hpp"
#include "Hardware/Stm32U5/Inc/hardware_u5_peripheral_time.hpp"

#ifdef STM32U5

#ifdef HAL_SPI_MODULE_ENABLED

#if !defined(USE_HAL_SPI_REGISTER_CALLBACKS) || (USE_HAL_SPI_REGISTER_CALLBACKS != 1U)
#error "hardware_u5_peripheral_spi requires USE_HAL_SPI_REGISTER_CALLBACKS = 1U in stm32u5xx_hal_conf.h"
#endif
static HW::Peripheral::Spi*    __gp_hardware_peripheral_spi[SPI_COUNT];

HW::Peripheral::Spi::Spi(SpiIdEnum _id)
{
    this->id = _id;
    this->baudratePrescaler = BAUDRATE_PRESCALER_NONE;
    this->dataSize = DATA_SIZE_NONE;
    this->polarity = POLARITY_NONE;
    this->phase = PHASE_NONE;
    this->__handle = nullptr;
}

HW::Peripheral::Spi::Spi()
{
    this->id = SPI_NONE;
    this->baudratePrescaler = BAUDRATE_PRESCALER_NONE;
    this->dataSize = DATA_SIZE_NONE;
    this->polarity = POLARITY_NONE;
    this->phase = PHASE_NONE;
    this->__handle = nullptr;
}

HW::Peripheral::Spi::Spi(SpiIdEnum _id, SpiModeEnum _spiMode, BaudratePrescalerEnum _preScale)
{
    this->id = _id;
    this->baudratePrescaler = _preScale;
    this->dataSize = DATA_SIZE_8BIT;
    this->__handle = nullptr;
    switch (_spiMode) {
        case SPI_MODE_0:
            this->polarity = POLARITY_LOW;
            this->phase = PHASE_1EDGE;
            break;
        case SPI_MODE_3:
            this->polarity = POLARITY_HIGH;
            this->phase = PHASE_2EDGE;
            break;
        case SPI_CUS1:
            this->polarity = POLARITY_HIGH;
            this->phase = PHASE_1EDGE;
            break;
        case SPI_CUS2:
            this->polarity = POLARITY_LOW;
            this->phase = PHASE_2EDGE;
            break;
        default:
            break;
    }
}

HW::Peripheral::Spi::Spi(SpiIdEnum _id, SpiModeEnum _spiMode, BaudratePrescalerEnum _preScale, DataSizeEnum _dataSize)
{
    this->id = _id;
    this->baudratePrescaler = _preScale;
    this->dataSize = _dataSize;
    this->__handle = nullptr;
    switch (_spiMode) {
        case SPI_MODE_0:
            this->polarity = POLARITY_LOW;
            this->phase = PHASE_1EDGE;
            break;
        case SPI_MODE_3:
            this->polarity = POLARITY_HIGH;
            this->phase = PHASE_2EDGE;
            break;
        case SPI_CUS1:
            this->polarity = POLARITY_HIGH;
            this->phase = PHASE_1EDGE;
            break;
        case SPI_CUS2:
            this->polarity = POLARITY_LOW;
            this->phase = PHASE_2EDGE;
            break;
        default:
            break;
    }
}

static HW::Peripheral::Spi* hardware_u5_peripheral_spi_get_pinstance(struct __SPI_HandleTypeDef *hspi)
{
    for(int i = 0; i < SPI_COUNT; i++)
    {
        if(__gp_hardware_peripheral_spi[i] != 0)
        {
            if(__gp_hardware_peripheral_spi[i]->__handle == hspi) return __gp_hardware_peripheral_spi[i];
        }
    }
    return 0;
}

static void hardware_u5_peripheral_spi_TxCpltCallback(struct __SPI_HandleTypeDef *hspi)
{
    HW::Peripheral::Spi* pspi = hardware_u5_peripheral_spi_get_pinstance(hspi);
    if(pspi)
    {
        pspi->__ITTxCpltProcess();
    }
}

static void hardware_u5_peripheral_spi_TxRxCpltCallback(struct __SPI_HandleTypeDef *hspi)
{
    HW::Peripheral::Spi* pspi = hardware_u5_peripheral_spi_get_pinstance(hspi);
    if(pspi)
    {
        pspi->__ITTxRxCpltProcess();
    }
}

void hardware_u5_peripheral_spi_ErrorCallback(struct __SPI_HandleTypeDef *hspi)
{
    HW::Peripheral::Spi* pspi = hardware_u5_peripheral_spi_get_pinstance(hspi);
    if(pspi)
    {
        pspi->__ErrorProcess();
    }
}

static void hardware_u5_peripheral_spi_task(void* arg)
{
    HW::Peripheral::Spi* pspi = (HW::Peripheral::Spi*)arg;
    pspi->__TaskProcess();
}

void HW::Peripheral::Spi::__ErrorProcess(void)
{
    if(funcErrorCallback)
    {
        ptrFuncErrorCallbackArg->code = __handle->ErrorCode;
        funcErrorCallback(ptrFuncErrorCallbackArg);
    }

    // Clear các cờ lỗi tương ứng theo ErrorCode
    if (__handle->ErrorCode & HAL_SPI_ERROR_OVR)
    {
        __HAL_SPI_CLEAR_OVRFLAG(__handle);
    }
    if (__handle->ErrorCode & HAL_SPI_ERROR_MODF)
    {
        __HAL_SPI_CLEAR_MODFFLAG(__handle);
    }
    if (__handle->ErrorCode & HAL_SPI_ERROR_FRE)
    {
        __HAL_SPI_CLEAR_FREFLAG(__handle);
    }
    if (__handle->ErrorCode & HAL_SPI_ERROR_CRC)
    {
        __HAL_SPI_CLEAR_CRCERRFLAG(__handle);
    }

    // Nếu lỗi liên quan DMA hoặc Abort, phải abort transfer trước khi restart
    // để đưa state machine SPI về HAL_SPI_STATE_READY (tránh HAL_BUSY khi gọi lại)
    if (__handle->State != HAL_SPI_STATE_READY)
    {
        HAL_SPI_Abort_IT(__handle);
    }
    if (__handle->ErrorCode & HAL_SPI_ERROR_DMA) {
        HAL_DMA_Abort(__handle->hdmatx);
        HAL_DMA_Abort(__handle->hdmarx);
    }

    if (__handle->ErrorCode & HAL_SPI_ERROR_TIMEOUT) {
        HAL_SPI_Abort(__handle);
    }
    if (__handle->ErrorCode & ~HAL_SPI_ERROR_MODF) {
        HAL_SPI_DeInit(__handle);
        HAL_SPI_Init(__handle);
    }

}

void HW::Peripheral::Spi::__ITTxCpltProcess(void)
{
    txEvent.Set(0x01);
}

void HW::Peripheral::Spi::__ITTxRxCpltProcess(void)
{
    task.Set(0x01);
}

void HW::Peripheral::Spi::__TaskProcess(void)
{
    ptrElementContinous = &elementContinous;

    while(1)
    {
        if(ptrElementContinous->ptrData)
        {
            SpiElementWriteType* pspiele = (SpiElementWriteType*)ptrElementContinous->ptrData;
            HW::Peripheral::Time* ptime  = (HW::Peripheral::Time*)pspiele->__timePrivate;

            if(pspiele->func)
            {
                pspiele->funcArg.time = *ptime>>TIME_LOOP;
                FuncPeripheralSpiWriteCallbackType callback = (FuncPeripheralSpiWriteCallbackType)pspiele->func;
                callback(&pspiele->funcArg);
            }

            if(ptrElementContinous->ptrNext->ptrData)
            {
                ptrElementContinous = ptrElementContinous->ptrNext;
            }
            else
            {
                ptrElementContinous = &elementContinous;
            }

            task.Sleep(1);
        }
        else
        {
            task.Sleep(1000);
        }
    }
}

#endif

StatusEnum HW::Peripheral::Spi::SendGet(uint8_t* txbuff, uint8_t* rxbuff, int len)
{
    
#ifdef HAL_SPI_MODULE_ENABLED
    if(isOpen)
    {
        std::lock_guard<RTOS::StaticMutex> lock(_spi_mutex);
        return (StatusEnum)HAL_SPI_TransmitReceive(__handle, txbuff, rxbuff, len, len * 1000);
    }
#endif
    return ERROR_NOT_EXIST;
}

StatusEnum HW::Peripheral::Spi::Send(uint8_t* txbuff, int len)
{
     
#ifdef HAL_SPI_MODULE_ENABLED
    if(isOpen)
    {
        std::lock_guard<RTOS::StaticMutex> lock(_spi_mutex);
        return (StatusEnum)HAL_SPI_Transmit(__handle, txbuff, len, len * 1000);
    }
#endif
    return ERROR_NOT_EXIST;
}

StatusEnum HW::Peripheral::Spi::Send(std::span<const uint8_t> data)
{
     
#ifdef HAL_SPI_MODULE_ENABLED
    if (!isOpen)
    {
        return BUSY;
    }

    if (data.empty())
    {
        return OK;
    }
    std::lock_guard<RTOS::StaticMutex> lock(_spi_mutex);
    StatusEnum ret;

    uint8_t* buff = const_cast<uint8_t*>(data.data());
    uint16_t len  = static_cast<uint16_t>(data.size());

    if (__handle->hdmatx)
    {
        ret = static_cast<StatusEnum>(
            HAL_SPI_Transmit_DMA(__handle, buff, len)
        );
    }
    else
    {
        ret = static_cast<StatusEnum>(
        		HAL_SPI_Transmit(__handle, buff, len, len * 1000)
        );
    }

    if (ret == OK)
    {
        if (txEvent.Wait(0x01, 1000) == 0x01)
        {
            ret = OK;
        }
        else
        {
            ret = ERROR_HAL;
        }
    }

    return ret;
#else
    return OK;
#endif
}

StatusEnum HW::Peripheral::Spi::Get(uint8_t* rxbuff, int len)
{
     std::lock_guard<RTOS::StaticMutex> lock(_spi_mutex);
#ifdef HAL_SPI_MODULE_ENABLED
    if(isOpen)
    {
        return (StatusEnum)HAL_SPI_Receive(__handle, rxbuff, len, len * 1000);
    }
#endif
    return ERROR_NOT_EXIST;
}

StatusEnum HW::Peripheral::Spi::Read(uint8_t* txbuff, uint8_t* rxbuff, int len)
{
     
#ifdef HAL_SPI_MODULE_ENABLED
    if(isOpen)
    {
        std::lock_guard<RTOS::StaticMutex> lock(_spi_mutex);
        StatusEnum ret;
        uint32_t eventRet = 0;

        // if (__handle->hdmatx == NULL || __handle->hdmarx == NULL)
        // {
        //     ret = (StatusEnum)HAL_SPI_TransmitReceive(__handle, txbuff, rxbuff, len, 100);
        //     return ret;
        // }

        ret = (StatusEnum)HAL_SPI_TransmitReceive_DMA(__handle, txbuff, rxbuff, len);
        if(ret != OK)
        {
            HAL_SPI_Abort(__handle);
            return ret;
        }

        eventRet = task.Wait(0x01, 2);
        if(eventRet == 0x01)
        {
            return OK;
        }
        HAL_SPI_Abort(__handle);
        return ERROR_TIMEOUT;
    }
#endif
    return ERROR_NOT_EXIST;
}

StatusEnum HW::Peripheral::Spi::Add(Type::String name, FuncPeripheralSpiWriteCallbackType func, object sender)
{
    if(isOpen)
    {
        StatusEnum ret = OK;
        SpiElementWriteType* ptrElement;

        nameAdd = name;

        ptrElement = (SpiElementWriteType*)g_calloc(sizeof(SpiElementWriteType));
        ptrElement->__timePrivate = (uint32_t)(uintptr_t)g_calloc(sizeof(HW::Peripheral::Time));

        ptrElement->funcArg.sender = (uint32_t)sender;
        ptrElement->func = (uint32_t)func;
        ptrElement->name = nameAdd;

        ret = elementContinous.Append(ptrElement);

        if(ret == ERROR_EXIST)
        {
            g_free((void*)(uintptr_t)ptrElement->__timePrivate);
            g_free(ptrElement);
        }
        return ret;
    }
    return ERROR_NOT_EXIST;
}

StatusEnum HW::Peripheral::Spi::Abort()
{
#ifdef HAL_SPI_MODULE_ENABLED
    if(isOpen)
    {
        StatusEnum ret = (StatusEnum)HAL_SPI_Abort(__handle);
        __handle->ErrorCode = HAL_SPI_ERROR_NONE;
        return ret;
    }
#endif
    return ERROR_EXIST;
}

StatusEnum HW::Peripheral::Spi::Open(void)
{
#ifdef HAL_SPI_MODULE_ENABLED
    StatusEnum ret = OK;

    if(!IS_SPI_ID(id))
    {
        return ERROR_NOT_EXIST;
    }

    if(isOpen == true)
    {
        return ERROR_EXIST;
    }

    if(priority == PRIORITY_NONE)
    {
        priority = PRIORITY_LOW;
    }

    const SpiConfigType* spiconfig = &HARDWARE_SPI_CONFIG[id];
    __handle = spiconfig->handle;

    HAL_SPI_DeInit(__handle);

    if(dataSize != 0)           __handle->Init.DataSize           = HARDWARE_SPI_DATA_SIZE[dataSize];
    if(polarity != 0)           __handle->Init.CLKPolarity        = HARDWARE_SPI_POLARITY[polarity];
    if(phase != 0)              __handle->Init.CLKPhase           = HARDWARE_SPI_PHASE[phase];
    if(baudratePrescaler != 0)  __handle->Init.BaudRatePrescaler  = HARDWARE_SPI_BAUDRATE_PRESCALER[baudratePrescaler];

    ret = (StatusEnum)HAL_SPI_Init(__handle);
    if(ret != OK)
    {
        return ret;
    }

    __handle->TxCpltCallback   = hardware_u5_peripheral_spi_TxCpltCallback;
    __handle->TxRxCpltCallback = hardware_u5_peripheral_spi_TxRxCpltCallback;
    __handle->ErrorCallback    = hardware_u5_peripheral_spi_ErrorCallback;

    task.name       = spiconfig->name;
    task.option     = FLAGS_WAIT_ANY;
    task.priority   = priority;
    task.sender     = this;
    task.size       = 4096;
    task           += hardware_u5_peripheral_spi_task;
    task.Open();

    txEvent.name        = spiconfig->name;
    txEvent.name       += "_EVT";
    txEvent.option      = FLAGS_WAIT_ANY;
    ret = txEvent.Open();
    if(ret != OK)
    {
        return ret;
    }

    _bus_mutex.Open();
    __gp_hardware_peripheral_spi[id] = this;
    isOpen = true;

    return ret;
#endif
    return ERROR_NOT_EXIST;
}

StatusEnum HW::Peripheral::Spi::Close(void)
{
#ifdef HAL_SPI_MODULE_ENABLED
    StatusEnum ret = OK;

    if(!IS_SPI_ID(id))
    {
        return ERROR_NOT_EXIST;
    }

    if(isOpen == false)
    {
        return ERROR_EXIST;
    }

    __gp_hardware_peripheral_spi[id] = 0;
    isOpen = false;

    if (HAL_SPI_DeInit(__handle) != HAL_OK)
    {
        ret = ERROR_HAL;
    }

    Type::Node* current = elementContinous.ptrNext;
    while(current != nullptr)
    {
        Type::Node* nextNode = current->ptrNext;
        if(current->ptrData != nullptr)
        {
            SpiElementWriteType* pspiele = (SpiElementWriteType*)current->ptrData;
            if(pspiele->__timePrivate != 0)
            {
                g_free((void*)(uintptr_t)pspiele->__timePrivate);
            }
            g_free(pspiele);
            current->ptrData = nullptr;
        }
        g_free(current);
        current = nextNode;
    }

    if(elementContinous.ptrData != nullptr)
    {
        SpiElementWriteType* pspiele = (SpiElementWriteType*)elementContinous.ptrData;
        if(pspiele->__timePrivate != 0)
        {
            g_free((void*)(uintptr_t)pspiele->__timePrivate);
        }
        g_free(pspiele);
        elementContinous.ptrData = nullptr;
    }
    elementContinous.ptrNext = nullptr;

    if(ptrFuncErrorCallbackArg != nullptr)
    {
        g_free(ptrFuncErrorCallbackArg);
        ptrFuncErrorCallbackArg = nullptr;
    }

    __handle->TxCpltCallback    = nullptr;
    __handle->TxRxCpltCallback  = nullptr;
    __handle->ErrorCallback     = nullptr;

    task.Close();
    txEvent.Close();

    return ret;
#endif
    return ERROR_NOT_EXIST;
}

void HW::Peripheral::Spi::Lock(void)
{
    _bus_mutex.lock();
}

void HW::Peripheral::Spi::Unlock(void)
{
    _bus_mutex.unlock();
}

#endif /** STM32U5 */
