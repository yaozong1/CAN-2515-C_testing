#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "nrf_sdm.h"
#include "app_error.h"
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"
#include "app_timer.h"
//#include "bsp_btn_ble.h"
#include "fds.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


/************************************************
/
/ CAN MCP2515 Include Files
/
*************************************************/

#include "inttypes.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "app_util_platform.h"
#include "mcp_can.h"
#include "SEGGER_RTT.h"
#include "nrf_drv_spi.h"
#include "nrfx_spim.h"









/**@brief Function for handling events from the BSP module.
 *
 * @param[in]   event   Event generated by button press.
 */ 
void bsp_event_handler(bsp_event_t event)
{
    ret_code_t err_code;

    switch (event)
    {
        case BSP_EVENT_SLEEP:
        {
            NRF_LOG_DEBUG("BSP Event: SLEEP");



            break;
        }
        case BSP_EVENT_WAKEUP:
        {
            NRF_LOG_DEBUG("BSP Event: WAKEUP");

            break;
        }
        case BSP_EVENT_RESET:
        {
            NRF_LOG_DEBUG("BSP Event: RESET");

            break;
        }
        case BSP_EVENT_SYSOFF:
        {
            NRF_LOG_DEBUG("BSP Event: SYSOFF");

            break;
        }
        case BSP_EVENT_DFU:
        {
            NRF_LOG_DEBUG("BSP Event: DFU");

            break;
        }
        case BSP_EVENT_ADVERTISING_START:
        {
            NRF_LOG_DEBUG("BSP Event: ADVERTISING START");

            break;
        }
        case BSP_EVENT_ADVERTISING_STOP:
        {
            NRF_LOG_DEBUG("BSP Event: ADVERTISING STOP");

            break;
        }
        case BSP_EVENT_DISCONNECT:
        {
            NRF_LOG_DEBUG("BSP Event: DISCONNECT");

            break;
        }
        case BSP_EVENT_WHITELIST_OFF:
        {
            NRF_LOG_DEBUG("BSP Event: WHITELIST OFF");

            break;
        }
        case BSP_EVENT_BOND:
        {
            NRF_LOG_DEBUG("BSP Event: BOND");

            break;
        }
        case BSP_EVENT_CLEAR_BONDING_DATA:
        {
            NRF_LOG_DEBUG("BSP Event: CLEAR BONDING DATA");

            break;
        }
        default:
            break;
    }
}


/**@brief Function for initializing the nrf log module.
 */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();

    NRF_LOG_DEBUG("Logging Initialization COMPLETED.");
}


/**@brief Function for initializing the nrf log module.
 */
static void mem_init(void)
{
    NRF_LOG_DEBUG("Memory Manager Initialization...");

    nrf_mem_init();

    NRF_LOG_DEBUG("Memory Manager Initialization COMPLETED.");
} 


/**@brief Function for initializing the clock.
 */
//static void clock_init(void)
//{
//    NRF_LOG_DEBUG("Clock Initialization...");

//    ret_code_t err_code;

//    //Initialize clock module
//    err_code = nrf_drv_clock_init();
//    APP_ERROR_CHECK(err_code);

//    err_code = nrf_drv_clock_lfclk_request(NULL);
//    APP_ERROR_CHECK(err_code);

//    NRF_LOG_DEBUG("Clock Initialization COMPLETED.");
//}





/**@brief Function for initializing buttons and leds.
 */ 
static void buttons_leds_init(void)
{
    NRF_LOG_DEBUG("LED & Buttons Initialization...");

    ret_code_t err_code;
    bsp_event_t startup_event;

    err_code = bsp_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS, bsp_event_handler);
    //APP_ERROR_CHECK(err_code);

    //err_code = bsp_btn_ble_init(NULL, &startup_event);
    //APP_ERROR_CHECK(err_code);

    NRF_LOG_DEBUG("LED & Buttons Initialization COMPLETED.");
}






/**@brief Function for initializing MCP2515 CAN Bus.
 */ 
static void can_init(void)
{
    NRF_LOG_DEBUG("CAN Initialization...");

    mcp_spi_init();

    NRF_LOG_DEBUG("MCP2515 SPI Initialized");

    START_INIT:
	
    if(mcp_can_begin(CAN_250KBPS, MCP_16MHz) == CAN_OK)
    {
	NRF_LOG_DEBUG("CAN BUS Initialization ok");
    }
    else
    {
	NRF_LOG_DEBUG("CAN BUS Initialization failed");
        NRF_LOG_DEBUG("Init CAN BUS again");
        nrf_delay_ms(1000);
        goto START_INIT;
    }

    NRF_LOG_DEBUG("CAN Initialization COMPLETED.");
}



/**@brief Function for application main entry.
 */
int main(void)
{



 //    printf("enable CAN power\r\n");


    //Enable CAN POWER
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0,0));
    nrf_delay_ms(10);
    nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(0,0),1);
    nrf_delay_ms(10);
 //   printf("enable CAN power\r\n");
  //  buttons_leds_init();//important
    bsp_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS, bsp_event_handler);
    // Initialize.
    log_init();
    nrf_mem_init();
   // mem_init();
    mcp_spi_init();
    //clock_init();
    //timers_init();

    //power_management_init();
   
    //can_init();
    mcp_can_begin(CAN_250KBPS, MCP_16MHz);
    
    // Start execution.
    NRF_LOG_DEBUG("CAN started.");
    
 /*
    uint8_t tmp_buffer0[4] = {0x11, 0x22, 0x33, 0x44};
    uint8_t tmp_buffer1[3] = {0x55, 0x66, 0x77};
    uint8_t tmp_buffer2[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};

    uint8_t tx_buf_index = 0;

    if (tx_buf_index == 0)
        {
            mcp2515_write_canMsg(MCP_TXB0SIDH, 0x300, 0, 0, sizeof(tmp_buffer0), (uint8_t*)&tmp_buffer0);
            tx_buf_index = 1;
            NRF_LOG_INFO("BUF0");
            nrf_delay_ms(5);
        }
        else if (tx_buf_index == 1)
        {
            mcp2515_write_canMsg(MCP_TXB1SIDH, 0x301, 0, 0, sizeof(tmp_buffer1), (uint8_t*)&tmp_buffer1);
            tx_buf_index = 2;
            NRF_LOG_INFO("BUF1");
            nrf_delay_ms(5);
        }
        else if (tx_buf_index == 2)
        {
            mcp2515_write_canMsg(MCP_TXB2SIDH, 0x302, 0, 0, sizeof(tmp_buffer2), (uint8_t*)&tmp_buffer2);
            tx_buf_index = 0;
            NRF_LOG_INFO("BUF2");
            nrf_delay_ms(5);
        }
*/

    // Enter main loop.
    for (;;)
    {
    //for send can
                uint32_t can_idd = 0x0005;
                uint8_t ext_send;
                uint8_t buff[8] = {1,2,3,4,5,6,7,8};                
                uint8_t lens = 0x08;

                mcp_can_send_msg(can_idd, ext_send, lens, buff);
                nrf_delay_ms(400);


    }
}