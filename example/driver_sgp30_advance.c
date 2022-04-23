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
 * @file      driver_sgp30_advance.c
 * @brief     driver sgp30 advance source file
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

#include "driver_sgp30_advance.h"

static sgp30_handle_t gs_handle;        /**< sgp30 handle */

/**
 * @brief  advance example init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t sgp30_advance_init(void)
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
    
    /* wait 100 ms */
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
 * @brief  advance example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t sgp30_advance_deinit(void)
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
 * @brief      advance example read
 * @param[out] *co2_eq_ppm points to a co2 buffer
 * @param[out] *tvoc_ppb points to a tvoc buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t sgp30_advance_read(uint16_t *co2_eq_ppm, uint16_t *tvoc_ppb)
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

/**
 * @brief      advance example get the serial id
 * @param[out] *id points to a serial id buffer
 * @return     status code
 *             - 0 success
 *             - 1 get serial id failed
 * @note       none
 */
uint8_t sgp30_advance_get_serial_id(uint16_t id[3])
{
    /* get serial id */
    if (sgp30_get_serial_id(&gs_handle, id) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      advance example get the chip feature
 * @param[out] *product_type points to a product type buffer
 * @param[out] *product_version points to a product version buffer
 * @return     status code
 *             - 0 success
 *             - 1 get feature failed
 * @note       none
 */
uint8_t sgp30_advance_get_feature(uint8_t *product_type, uint8_t *product_version)
{
    /* get feature set */
    if (sgp30_get_feature_set(&gs_handle, product_type, product_version) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     advance example set the chip iaq baseline
 * @param[in] tvoc is the current tvoc
 * @param[in] co2_eq is the current co2 eq
 * @return    status code
 *            - 0 success
 *            - 1 set iaq baseline failed
 * @note      none
 */
uint8_t sgp30_advance_set_iaq_baseline(uint16_t tvoc, uint16_t co2_eq)
{
    /* set iaq baseline */
    if (sgp30_set_iaq_baseline(&gs_handle, tvoc, co2_eq) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      advance example get the chip iaq baseline
 * @param[out] *tvoc points to a tvoc buffer
 * @param[out] *co2_eq points to a co2 buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iaq baseline failed
 * @note       none
 */
uint8_t sgp30_advance_get_iaq_baseline(uint16_t *tvoc, uint16_t *co2_eq)
{
    /* get iaq baseline */
    if (sgp30_get_iaq_baseline(&gs_handle, tvoc, co2_eq) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     advance example set the absolute humidity
 * @param[in] temp is the current temperature 
 * @param[in] rh is the current relative humidity
 * @return    status code
 *            - 0 success
 *            - 1 set absolute humidity failed
 * @note      none
 */
uint8_t sgp30_advance_set_absolute_humidity(float temp, float rh)
{
    uint8_t res;
    uint16_t reg;
    
    /* humidity convert to register */
    res = sgp30_absolute_humidity_convert_to_register(&gs_handle, temp, rh, (uint16_t *)&reg);
    if (res != 0)
    {
        return 1;
    }
    
    /* set absolute humidity */
    res = sgp30_set_absolute_humidity(&gs_handle, reg);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}
