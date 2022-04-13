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
 * @file      driver_sgp30_advance.h
 * @brief     driver sgp30 advance header file
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

#ifndef DRIVER_SGP30_ADVANCE_H
#define DRIVER_SGP30_ADVANCE_H

#include "driver_sgp30_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup sgp30_example_driver
 * @{
 */

/**
 * @brief  advance example init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t sgp30_advance_init(void);

/**
 * @brief  advance example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t sgp30_advance_deinit(void);

/**
 * @brief      advance example read
 * @param[out] *co2_eq_ppm points to a co2 buffer
 * @param[out] *tvoc_ppb points to a tvoc buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t sgp30_advance_read(uint16_t *co2_eq_ppm, uint16_t *tvoc_ppb);

/**
 * @brief      advance example get the serial id
 * @param[out] *id points to a serial id buffer
 * @return     status code
 *             - 0 success
 *             - 1 get serial id failed
 * @note       none
 */
uint8_t sgp30_advance_get_serial_id(uint16_t id[3]);

/**
 * @brief      advance example get the chip feature
 * @param[out] *product_type points to a product type buffer
 * @param[out] *product_version points to a product version buffer
 * @return     status code
 *             - 0 success
 *             - 1 get feature failed
 * @note       none
 */
uint8_t sgp30_advance_get_feature(uint8_t *product_type, uint8_t *product_version);

/**
 * @brief     advance example set the chip iaq baseline
 * @param[in] tvoc is the current tvoc
 * @param[in] co2_eq is the current co2 eq
 * @return    status code
 *            - 0 success
 *            - 1 set iaq baseline failed
 * @note      none
 */
uint8_t sgp30_advance_set_iaq_baseline(uint16_t tvoc, uint16_t co2_eq);

/**
 * @brief      advance example get the chip iaq baseline
 * @param[out] *tvoc points to a tvoc buffer
 * @param[out] *co2_eq points to a co2 buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iaq baseline failed
 * @note       none
 */
uint8_t sgp30_advance_get_iaq_baseline(uint16_t *tvoc, uint16_t *co2_eq);

/**
 * @brief     advance example set the absolute humidity
 * @param[in] temp is the current temperature 
 * @param[in] rh is the current relative humidity
 * @return    status code
 *            - 0 success
 *            - 1 set absolute humidity failed
 * @note      none
 */
uint8_t sgp30_advance_set_absolute_humidity(float temp, float rh);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
