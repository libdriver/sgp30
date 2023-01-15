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
 * @file      driver_sgp30.h
 * @brief     driver sgp30 header file
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

#ifndef DRIVER_SGP30_H
#define DRIVER_SGP30_H

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup sgp30_driver sgp30 driver function
 * @brief    sgp30 driver modules
 * @{
 */

/**
 * @addtogroup sgp30_base_driver
 * @{
 */

/**
 * @brief sgp30 handle structure definition
 */
typedef struct sgp30_handle_s
{
    uint8_t (*iic_init)(void);                                                 /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                               /**< point to an iic_deinit function address */
    uint8_t (*iic_write_cmd)(uint8_t addr, uint8_t *buf, uint16_t len);        /**< point to an iic_write_cmd function address */
    uint8_t (*iic_read_cmd)(uint8_t addr, uint8_t *buf, uint16_t len);         /**< point to an iic_read_cmd function address */
    void (*delay_ms)(uint32_t ms);                                             /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                           /**< point to a debug_print function address */
    uint8_t inited;                                                            /**< inited flag */
} sgp30_handle_t;

/**
 * @brief sgp30 information structure definition
 */
typedef struct sgp30_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} sgp30_info_t;

/**
 * @}
 */

/**
 * @defgroup sgp30_link_driver sgp30 link driver function
 * @brief    sgp30 link driver modules
 * @ingroup  sgp30_driver
 * @{
 */

/**
 * @brief     initialize sgp30_handle_t structure
 * @param[in] HANDLE points to an sgp30 handle structure
 * @param[in] STRUCTURE is sgp30_handle_t
 * @note      none
 */
#define DRIVER_SGP30_LINK_INIT(HANDLE, STRUCTURE)            memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link iic_init function
 * @param[in] HANDLE points to an sgp30 handle structure
 * @param[in] FUC points to an iic_init function address
 * @note      none
 */
#define DRIVER_SGP30_LINK_IIC_INIT(HANDLE, FUC)             (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE points to an sgp30 handle structure
 * @param[in] FUC points to an iic_deinit function address
 * @note      none
 */
#define DRIVER_SGP30_LINK_IIC_DEINIT(HANDLE, FUC)           (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_write_cmd function
 * @param[in] HANDLE points to an sgp30 handle structure
 * @param[in] FUC points to an iic_write_cmd function address
 * @note      none
 */
#define DRIVER_SGP30_LINK_IIC_WRITE_COMMAND(HANDLE, FUC)    (HANDLE)->iic_write_cmd = FUC

/**
 * @brief     link iic_read_cmd function
 * @param[in] HANDLE points to an sgp30 handle structure
 * @param[in] FUC points to an iic_read_cmd function address
 * @note      none
 */
#define DRIVER_SGP30_LINK_IIC_READ_COMMAND(HANDLE, FUC)     (HANDLE)->iic_read_cmd = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to an sgp30 handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_SGP30_LINK_DELAY_MS(HANDLE, FUC)             (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to an sgp30 handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_SGP30_LINK_DEBUG_PRINT(HANDLE, FUC)          (HANDLE)->debug_print = FUC

/**
 * @}
 */

/**
 * @defgroup sgp30_base_driver sgp30 base driver function
 * @brief    sgp30 base driver modules
 * @ingroup  sgp30_driver
 * @{
 */

/**
 * @brief      get chip information
 * @param[out] *info points to an sgp30 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t sgp30_info(sgp30_info_t *info);

/**
 * @brief     initialize the chip
 * @param[in] *handle points to an sgp30 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 * @note      none
 */
uint8_t sgp30_init(sgp30_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle points to an sgp30 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 soft reset failed
 * @note      none
 */
uint8_t sgp30_deinit(sgp30_handle_t *handle);

/**
 * @brief     initialize the chip iaq 
 * @param[in] *handle points to an sgp30 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iaq init failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sgp30_iaq_init(sgp30_handle_t *handle);

/**
 * @brief      read the iaq measure result
 * @param[in]  *handle points to an sgp30 handle structure
 * @param[out] *co2_eq_ppm points to a co2 buffer
 * @param[out] *tvoc_ppb points to a tvoc buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sgp30_read(sgp30_handle_t *handle, uint16_t *co2_eq_ppm, uint16_t *tvoc_ppb);

/**
 * @brief      get the iaq measure result
 * @param[in]  *handle points to an sgp30 handle structure
 * @param[out] *co2_eq_ppm points to a co2 buffer
 * @param[out] *tvoc_ppb points to a tvoc buffer
 * @return     status code
 *             - 0 success
 *             - 1 measure iaq failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sgp30_measure_iaq(sgp30_handle_t *handle, uint16_t *co2_eq_ppm, uint16_t *tvoc_ppb);

/**
 * @brief      get the iaq measure raw result
 * @param[in]  *handle points to an sgp30 handle structure
 * @param[out] *tvoc points to a tvoc buffer
 * @param[out] *co2_eq points to a co2 buffer
 * @return     status code
 *             - 0 success
 *             - 1 get measure raw failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sgp30_get_measure_raw(sgp30_handle_t *handle, uint16_t *tvoc, uint16_t *co2_eq);

/**
 * @brief     set the chip tvoc baseline
 * @param[in] *handle points to an sgp30 handle structure
 * @param[in] tvoc_baseline is the current tvoc baseline
 * @return    status code
 *            - 0 success
 *            - 1 set tvoc baseline failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sgp30_set_tvoc_baseline(sgp30_handle_t *handle, uint16_t tvoc_baseline);

/**
 * @brief      get the chip tvoc inceptive baseline
 * @param[in]  *handle points to an sgp30 handle structure
 * @param[out] *tvoc_baseline points to a current tvoc baseline buffer
 * @return     status code
 *             - 0 success
 *             - 1 get tvoc inceptive baseline failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sgp30_get_tvoc_inceptive_baseline(sgp30_handle_t *handle, uint16_t *tvoc_baseline);

/**
 * @brief     soft reset the chip
 * @param[in] *handle points to an sgp30 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 soft reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sgp30_soft_reset(sgp30_handle_t *handle);

/**
 * @brief      get the chip serial id
 * @param[in]  *handle points to an sgp30 handle structure
 * @param[out] *id points to an id buffer
 * @return     status code
 *             - 0 success
 *             - 1 get serial id failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sgp30_get_serial_id(sgp30_handle_t *handle, uint16_t id[3]);

/**
 * @brief     set the chip iaq baseline
 * @param[in] *handle points to an sgp30 handle structure
 * @param[in] tvoc is the current tvoc
 * @param[in] co2_eq is the current co2 eq
 * @return    status code
 *            - 0 success
 *            - 1 set iaq baseline failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sgp30_set_iaq_baseline(sgp30_handle_t *handle, uint16_t tvoc, uint16_t co2_eq);

/**
 * @brief      get the chip iaq baseline
 * @param[in]  *handle points to an sgp30 handle structure
 * @param[out] *tvoc points to a tvoc buffer
 * @param[out] *co2_eq points to a co2 buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iaq baseline failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sgp30_get_iaq_baseline(sgp30_handle_t *handle, uint16_t *tvoc, uint16_t *co2_eq);

/**
 * @brief     set the chip absolute_humidity
 * @param[in] *handle points to an sgp30 handle structure
 * @param[in] humidity is the current absolute_humidity
 * @return    status code
 *            - 0 success
 *            - 1 set absolute_humidity failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sgp30_set_absolute_humidity(sgp30_handle_t *handle, uint16_t humidity);

/**
 * @brief      convert the absolute humidity to the register data
 * @param[in]  *handle points to an sgp30 handle structure
 * @param[in]  temp is the current temperature 
 * @param[in]  rh is the current relative humidity
 * @param[out] *reg points to a converted register data
 * @return     status code
 *             - 0 success
 *             - 1 absolute humidity convert to register failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sgp30_absolute_humidity_convert_to_register(sgp30_handle_t *handle, float temp, float rh, uint16_t *reg);

/**
 * @brief      run the chip measure test
 * @param[in]  *handle points to an sgp30 handle structure
 * @param[out] *result points to a test result buffer
 * @return     status code
 *             - 0 success
 *             - 1 measure test failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sgp30_measure_test(sgp30_handle_t *handle, uint16_t *result);

/**
 * @brief      get the chip feature
 * @param[in]  *handle points to an sgp30 handle structure
 * @param[out] *product_type points to a product type buffer
 * @param[out] *product_version points to a product version buffer
 * @return     status code
 *             - 0 success
 *             - 1 get feature set failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sgp30_get_feature_set(sgp30_handle_t *handle, uint8_t *product_type, uint8_t *product_version);

/**
 * @brief      get the iaq measure raw result
 * @param[in]  *handle points to an sgp30 handle structure
 * @param[out] *tvoc points to a tvoc buffer
 * @param[out] *co2_eq points to a co2 buffer
 * @return     status code
 *             - 0 success
 *             - 1 get measure raw failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sgp30_get_measure_raw(sgp30_handle_t *handle, uint16_t *tvoc, uint16_t *co2_eq);

/**
 * @}
 */

/**
 * @defgroup sgp30_extern_driver sgp30 extern driver function
 * @brief    sgp30 extern driver modules
 * @ingroup  sgp30_driver
 * @{
 */

/**
 * @brief     set the chip register
 * @param[in] *handle points to an sgp30 handle structure
 * @param[in] reg is the iic register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sgp30_set_reg(sgp30_handle_t *handle, uint16_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      get the chip register
 * @param[in]  *handle points to an sgp30 handle structure
 * @param[in]  reg is the iic register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sgp30_get_reg(sgp30_handle_t *handle, uint16_t reg, uint8_t *buf, uint16_t len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
