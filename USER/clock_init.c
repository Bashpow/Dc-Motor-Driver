#include "stm32f0xx.h"

#define LL_FLASH_LATENCY_1 FLASH_ACR_LATENCY			 /*!< FLASH One Latency cycle */
#define LL_RCC_SYSCLK_DIV_1 RCC_CFGR_HPRE_DIV1			 /*!< SYSCLK not divided */
#define LL_RCC_APB1_DIV_1 RCC_CFGR_PPRE_DIV1			 /*!< HCLK not divided */
#define LL_RCC_SYS_CLKSOURCE_PLL RCC_CFGR_SW_PLL		 /*!< PLL selection as system clock */
#define LL_RCC_SYS_CLKSOURCE_STATUS_PLL RCC_CFGR_SWS_PLL /*!< PLL used as system clock */

#define SET_BIT(REG, BIT) ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT) ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT) ((REG) & (BIT))
#define CLEAR_REG(REG) ((REG) = (0x0))
#define WRITE_REG(REG, VAL) ((REG) = (VAL))
#define READ_REG(REG) ((REG))
#define MODIFY_REG(REG, CLEARMASK, SETMASK) WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#define FLASH_ACR_LATENCY_Pos (0U)
#define FLASH_ACR_LATENCY_Msk (0x1UL << FLASH_ACR_LATENCY_Pos) /*!< 0x00000001 */
#define LL_FLASH_LATENCY_1 FLASH_ACR_LATENCY				   /*!< FLASH One Latency cycle */

void SystemClock_Config(void)
{
	/* Configure Flash prefetch */
	MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, LL_FLASH_LATENCY_1);

	while (((uint32_t)(READ_BIT(FLASH->ACR, FLASH_ACR_LATENCY))) != LL_FLASH_LATENCY_1)
	{
	}

	SET_BIT(RCC->CR, RCC_CR_HSION);

	/* Wait till HSI is ready */
	while ((READ_BIT(RCC->CR, RCC_CR_HSIRDY) == (RCC_CR_HSIRDY)) != 1)
	{
	}

	MODIFY_REG(RCC->CR, RCC_CR_HSITRIM, (16) << (3U));

	SET_BIT(RCC->CR2, RCC_CR2_HSI14ON);

	/* Wait till HSI14 is ready */
	while (((READ_BIT(RCC->CR2, RCC_CR2_HSI14RDY) == (RCC_CR2_HSI14RDY))) != 1)
	{
	}

	MODIFY_REG(RCC->CR2, RCC_CR2_HSI14TRIM, (16) << ((uint32_t)3U));
	MODIFY_REG(RCC->CFGR, RCC_CFGR_PLLSRC | RCC_CFGR_PLLMUL, (0x00008000U) | (0x00100000U));
	MODIFY_REG(RCC->CFGR2, (0xFUL << (0U)), (0x00000000U));

	SET_BIT(RCC->CR, RCC_CR_PLLON);

	/* Wait till PLL is ready */
	while ((READ_BIT(RCC->CR, RCC_CR_PLLRDY) == (RCC_CR_PLLRDY)) != 1)
	{
	}

	MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, LL_RCC_SYSCLK_DIV_1);
	MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE, LL_RCC_APB1_DIV_1);
	MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, LL_RCC_SYS_CLKSOURCE_PLL);
	while (((uint32_t)(READ_BIT(RCC->CFGR, RCC_CFGR_SWS))) != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
	{
		/* code */
	}

	// SystemCoreClock = 48000000;

	CLEAR_BIT(RCC->CR2, RCC_CR2_HSI14DIS);
}
