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
 * @file      driver_sgp30_read_test.c
 * @brief     driver sgp30 read test source file
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

#include "driver_sgp30_read_test.h"

static sgp30_handle_t gs_handle;        /**< sgp30 handle */

/**
 * @brief     read test
 * @param[in] times is the test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t sgp30_read_test(uint32_t times)
{
    uint8_t res;
    uint32_t i;
    uint16_t co2_eq_ppm;
    uint16_t tvoc_ppb;
    sgp30_info_t info;
    
    /* link functions */
    DRIVER_SGP30_LINK_INIT(&gs_handle, sgp30_handle_t);
    DRIVER_SGP30_LINK_IIC_INIT(&gs_handle, sgp30_interface_iic_init);
    DRIVER_SGP30_LINK_IIC_DEINIT(&gs_handle, sgp30_interface_iic_deinit);
    DRIVER_SGP30_LINK_IIC_WRITE_COMMAND(&gs_handle, sgp30_interface_iic_write_cmd);
    DRIVER_SGP30_LINK_IIC_READ_COMMAND(&gs_handle, sgp30_interface_iic_read_cmd);
    DRIVER_SGP30_LINK_DELAY_MS(&gs_handle, sgp30_interface_delay_ms);
    DRIVER_SGP30_LINK_DEBUG_PRINT(&gs_handle, sgp30_interface_debug_print);
    
    /* sgp30 info */
    res = sgp30_info(&info);
    if (res != 0)
    {
        sgp30_interface_debug_print("sgp30: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip information */
        sgp30_interface_debug_print("sgp30: chip is %s.\n", info.chip_name);
        sgp30_interface_debug_print("sgp30: manufacturer is %s.\n", info.manufacturer_name);
        sgp30_interface_debug_print("sgp30: interface is %s.\n", info.interface);
        sgp30_interface_debug_print("sgp30: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        sgp30_interface_debug_print("sgp30: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        sgp30_interface_debug_print("sgp30: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        sgp30_interface_debug_print("sgp30: max current is %0.2fmA.\n", info.max_current_ma);
        sgp30_interface_debug_print("sgp30: max temperature is %0.1fC.\n", info.temperature_max);
        sgp30_interface_debug_print("sgp30: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start read test */
    sgp30_interface_debug_print("sgp30: start read test.\n");
    
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
        sgp30_interface_debug_print("sgp30: soft failed.\n");
        (void)sgp30_deinit(&gs_handle);
        
        return 1;
    }
    
    /* delay 100 ms */
    sgp30_interface_delay_ms(100);
    
    /* iaq init */
    res = sgp30_iaq_init(&gs_handle);
    if (res != 0)
    {
        sgp30_interface_debug_print("sgp30: iaq init failed.\n");
        (void)sgp30_deinit(&gs_handle);
        
        return 1;
    }
    sgp30_interface_debug_print("sgp30: raw read.\n");
    for (i = 0; i < times; i++)
    {
        /* delay 1000 ms */
        sgp30_interface_delay_ms(1000);
        res = sgp30_get_measure_raw(&gs_handle, (uint16_t *)&co2_eq_ppm, (uint16_t *)&tvoc_ppb);
        if (res != 0)
        {
            sgp30_interface_debug_print("sgp30: read failed.\n");
            (void)sgp30_deinit(&gs_handle);
            
            return 1;
        }
        sgp30_interface_debug_print("sgp30: raw co2 eq is %d.\n", co2_eq_ppm);
        sgp30_interface_debug_print("sgp30: raw tvoc is %d.\n", tvoc_ppb);
    }
    sgp30_interface_debug_print("sgp30: normal read.\n");
    for (i = 0; i < times; i++)
    {
        /* delay 1000 ms */
        sgp30_interface_delay_ms(1000);
        res = sgp30_read(&gs_handle, (uint16_t *)&co2_eq_ppm, (uint16_t *)&tvoc_ppb);
        if (res != 0)
        {
            sgp30_interface_debug_print("sgp30: read failed.\n");
            (void)sgp30_deinit(&gs_handle);
            
            return 1;
        }
        sgp30_interface_debug_print("sgp30: co2 eq is %d ppm.\n", co2_eq_ppm);
        sgp30_interface_debug_print("sgp30: tvoc is %d ppb.\n", tvoc_ppb);
    }
    
    /* finish read test */
    sgp30_interface_debug_print("sgp30: finish read test.\n");
    (void)sgp30_deinit(&gs_handle);
    
    return 0;
}
