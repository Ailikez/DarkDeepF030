/*
    Callback handlers.
    It is probably the easiest to store all callback functions in one file.
*/

/* Includes (or declare functions with 'extern' if it's prefered) */
#include "stm32f0xx_hal.h"
#include "stm32f0xx.h"
#include "stm32f0xx_it.h"

#ifdef __cplusplus
extern "C" {
#endif
/* Example callback:
    void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
        if (htim->Instance == stm::delayTimer.Instance)
            onTimPeriodElapsed();
    }
*/
/******************************************************************************/
/*            Cortex-M0 Processor Interruption and Exception Handlers         */
/******************************************************************************/

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
    /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

    /* USER CODE END NonMaskableInt_IRQn 0 */
    /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

    /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
    /* USER CODE BEGIN HardFault_IRQn 0 */

    /* USER CODE END HardFault_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_HardFault_IRQn 0 */
        /* USER CODE END W1_HardFault_IRQn 0 */
    }
    /* USER CODE BEGIN HardFault_IRQn 1 */

    /* USER CODE END HardFault_IRQn 1 */
}

/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void)
{
    /* USER CODE BEGIN SVC_IRQn 0 */

    /* USER CODE END SVC_IRQn 0 */
    /* USER CODE BEGIN SVC_IRQn 1 */

    /* USER CODE END SVC_IRQn 1 */
}

/**
* @brief This function handles Pendable request for system service.
*/
void PendSV_Handler(void)
{
    /* USER CODE BEGIN PendSV_IRQn 0 */

    /* USER CODE END PendSV_IRQn 0 */
    /* USER CODE BEGIN PendSV_IRQn 1 */

    /* USER CODE END PendSV_IRQn 1 */
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
    /* USER CODE BEGIN SysTick_IRQn 0 */

    /* USER CODE END SysTick_IRQn 0 */
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
    /* USER CODE BEGIN SysTick_IRQn 1 */

    /* USER CODE END SysTick_IRQn 1 */
}

/*
    Note about Priority in ARM Cortex-M

    High priority means low priority number, low priority means high number.
    In call to NVIC_SetPriority(IRQn, priority) write priority "just as is" (don't shift the value left).

    There are two parts of priority: preemption priority and subpriority,
    available priority bits are divided between these two.
    To do so use function NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_X).

    Preemption priority allows for preempting interrupts, which means that
    interrupt with higher preempt priority can interrupt the one with lower.
    Subpriority is used only for choosing which of pending interrupts to process next.

    Default division in HAL_Init is NVIC_PRIORITYGROUP_4, so 4 bits for preemption, 0 bits for subpriority.
    Default SysTick priority in HAL_Init is TICK_INT_PRIORITY from *_hal_conf.h,
    that is set for preemption priority (default value 0x0f - lowest priprity).
*/
#ifdef __cplusplus
}
#endif
