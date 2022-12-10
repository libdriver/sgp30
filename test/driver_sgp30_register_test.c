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
 * @file      driver_sgp30_register_test.c
 * @brief     driver sgp30 register test source file
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

#include "driver_sgp30_register_test.h"
#include <stdlib.h>

static sgp30_handle_t gs_handle;        /**< sgp30 handle */

/**
 * @brief  register test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t sgp30_register_test(void)
{
    uint8_t res;
    uint16_t id[3];
    uint8_t product_type;
    uint8_t product_version;
    uint16_t tvoc, tvoc_check;
    uint16_t co2_eq, co2_eq_check;
    uint16_t humidity;
    float temp;
    float rh;
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
    
    /* start register test */
    sgp30_interface_debug_print("sgp30: start register test.\n");
    
    /* sgp30 init */
    res = sgp30_init(&gs_handle);
    if (res != 0)
    {
        sgp30_interface_debug_print("sgp30: init failed.\n");
    
        return 1;
    }
    
    /* sgp30 soft reset test */
    sgp30_interface_debug_print("sgp30: sgp30_soft_reset test.\n");
    
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
    sgp30_interface_debug_print("sgp30: check soft reset %s.\n", res==0?"ok":"error");
    
    /* iaq init */
    res = sgp30_iaq_init(&gs_handle);
    if (res != 0)
    {
        sgp30_interface_debug_print("sgp30: iaq init failed.\n");
        (void)sgp30_deinit(&gs_handle);
        
        return 1;
    }
    
    /* sgp30 get serial id test */
    sgp30_interface_debug_print("sgp30: sgp30_get_serial_id test.\n");
    
    /* sgp30 get serial id */
    res = sgp30_get_serial_id(&gs_handle, (uint16_t *)id);
    if (res != 0)
    {
        sgp30_interface_debug_print("sgp30: get serial id failed.\n");
        (void)sgp30_deinit(&gs_handle);
        
        return 1;
    }
    sgp30_interface_debug_print("sgp30: check get serial id %s.\n", res==0?"ok":"error");
    sgp30_interface_debug_print("sgp30: serial id 0x%04X 0x%04X 0x%04X.\n", (uint16_t)(id[0]), (uint16_t)(id[1]), (uint16_t)(id[2]));
    
    /* sgp30_get_feature_set test */
    sgp30_interface_debug_print("sgp30: sgp30_get_feature_set test.\n");
    res = sgp30_get_feature_set(&gs_handle, (uint8_t *)&product_type, (uint8_t *)&product_version);
    if (res != 0)
    {
        sgp30_interface_debug_print("sgp30: get feature set failed.\n");
        (void)sgp30_deinit(&gs_handle);
        
        return 1;
    }
    sgp30_interface_debug_print("sgp30: check get feature set %s.\n", res==0?"ok":"error");
    sgp30_interface_debug_print("sgp30: product type is 0x%02X.\n", product_type);
    sgp30_interface_debug_print("sgp30: product version is 0x%02X.\n", product_version);
    
    /* sgp30_set_tvoc_baseline/sgp30_get_tvoc_inceptive_baseline test */
    sgp30_interface_debug_print("sgp30: sgp30_set_tvoc_baseline/sgp30_get_tvoc_inceptive_baseline test.\n");
    tvoc = (uint16_t)(rand() % 65535);
    res = sgp30_set_tvoc_baseline(&gs_handle, tvoc);
    if (res != 0)
    {
        sgp30_interface_debug_print("sgp30: set tvoc baseline failed.\n");
        (void)sgp30_deinit(&gs_handle);
        
        return 1;
    }
    sgp30_interface_debug_print("sgp30: set tvoc baseline 0x%04X.\n", (uint16_t)(tvoc));
    res = sgp30_get_tvoc_inceptive_baseline(&gs_handle, (uint16_t *)&tvoc_check);
    if (res != 0)
    {
        sgp30_interface_debug_print("sgp30: get tvoc inceptive baseline failed.\n");
        (void)sgp30_deinit(&gs_handle);
        
        return 1;
    }
    sgp30_interface_debug_print("sgp30: get tvoc inceptive baseline 0x%04X.\n", tvoc_check);
    
    /* sgp30_set_iaq_baseline/sgp30_get_iaq_baseline test */
    sgp30_interface_debug_print("sgp30: sgp30_set_iaq_baseline/sgp30_get_iaq_baseline test.\n");
    tvoc = (uint16_t)(rand() % 65535);
    co2_eq = (uint16_t)(rand() % 65535);
    res = sgp30_set_iaq_baseline(&gs_handle, tvoc, co2_eq);
    if (res != 0)
    {
        sgp30_interface_debug_print("sgp30: set iaq baseline failed.\n");
        (void)sgp30_deinit(&gs_handle);
        
        return 1;
    }
    sgp30_interface_debug_print("sgp30: set tvoc baseline 0x%04X.\n", (uint16_t)(tvoc));
    sgp30_interface_debug_print("sgp30: set co2 eq baseline 0x%04X.\n", (uint16_t)(co2_eq));
    res = sgp30_get_iaq_baseline(&gs_handle, (uint16_t *)&tvoc_check, (uint16_t *)&co2_eq_check);
    if (res != 0)
    {
        sgp30_interface_debug_print("sgp30: get iaq baseline failed.\n");
        (void)sgp30_deinit(&gs_handle);
        
        return 1;
    }
    sgp30_interface_debug_print("sgp30: check iaq baseline %s.\n", ((tvoc==tvoc_check)&&(co2_eq==co2_eq_check))?"ok":"error");
    
    /* sgp30_set_absolute_humidity test */
    sgp30_interface_debug_print("sgp30: sgp30_set_absolute_humidity test.\n");
    temp = (float)(rand() % 3000) / 100.0f;
    rh = (float)(rand() % 10000) / 100.0f;
    res = sgp30_absolute_humidity_convert_to_register(&gs_handle, temp, rh, (uint16_t *)&humidity);
    if (res != 0)
    {
        sgp30_interface_debug_print("sgp30: absolute humidity convert to register failed.\n");
        (void)sgp30_deinit(&gs_handle);
        
        return 1;
    }
    sgp30_interface_debug_print("sgp30: set temp %0.1fC.\n", temp);
    sgp30_interface_debug_print("sgp30: set rh %0.2f%%.\n", rh);
    sgp30_interface_debug_print("sgp30: set register value 0x%04X.\n", (uint16_t)humidity);
    res = sgp30_set_absolute_humidity(&gs_handle, humidity);
    if (res != 0)
    {
        sgp30_interface_debug_print("sgp30: set absolute humidity failed.\n");
        (void)sgp30_deinit(&gs_handle);
        
        return 1;
    }
    sgp30_interface_debug_print("sgp30: check absolute humidity %s.\n", (res==0)?"ok":"error");
    
    /* finish register test */
    sgp30_interface_debug_print("sgp30: finish register test.\n");
    (void)sgp30_deinit(&gs_handle);
    
    return 0;
}
