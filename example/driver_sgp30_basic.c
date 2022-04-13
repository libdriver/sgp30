/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_sgp30_basic.c
 * @brief     driver sgp30 basic source file
 * @version   2.0.0
 * @author    Shifeng Li
 * @date      2021-03-07
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/03/07  <td>2.0      <td>Shifeng Li  <td>format the code
 * <tr><td>2020/11/05  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */
 
#include "driver_sgp30_basic.h"

static sgp30_handle_t gs_handle;        /**< sgp30 handle */

/**
 * @brief  basic example init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t sgp30_basic_init(void)
{
    uint8_t res;
    
    /* link functions */
    DRIVER_SGP30_LINK_INIT(&gs_handle, sgp30_handle_t);
    DRIVER_SGP30_LINK_IIC_INIT(&gs_handle, sgp30_interface_iic_init);
    DRIVER_SGP30_LINK_IIC_DEINIT(&gs_handle, sgp30_interface_iic_deinit);
    DRIVER_SGP30_LINK_IIC_WRITE_COMMAND(&gs_handle, sgp30_interface_iic_write_cmd);
    DRIVER_SGP30_LINK_IIC_READ_COMMAND(&gs_handle, sgp30_interface_iic_read_cmd);
    DRIVER_SGP30_LINK_DELAY_MS(&gs_handle, sgp30_interface_delay_ms);
    DRIVER_SGP30_LINK_DEBUG_PRINT(&gs_handle, sgp30_interface_debug_print);
    
    /* sgp30 init */
    res = sgp30_init(&gs_handle);
    if (res != 0)
    {
        sgp30_interface_debug_print("sgp30: init failed.\n");
    
        return 1;
    }
    
    /* soft reset */
    res = sgp30_soft_reset(&gs_handle);
    if (res != 0)
    {
        sgp30_interface_debug_print("sgp30: soft reset failed.\n");
    
        return 1;
    }
    
    /* 100 ms */
    sgp30_interface_delay_ms(100);
    
    /* iaq init */
    res = sgp30_iaq_init(&gs_handle);
    if (res != 0)
    {
        sgp30_interface_debug_print("sgp30: iaq init failed.\n");
        (void)sgp30_deinit(&gs_handle);
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t sgp30_basic_deinit(void)
{
    /* close sgp30 */
    if (sgp30_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      basic example read
 * @param[out] *co2_eq_ppm points to a co2 buffer
 * @param[out] *tvoc_ppb points to a tvoc buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t sgp30_basic_read(uint16_t *co2_eq_ppm, uint16_t *tvoc_ppb)
{
    /* read data */
    if (sgp30_read(&gs_handle, co2_eq_ppm, tvoc_ppb) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
