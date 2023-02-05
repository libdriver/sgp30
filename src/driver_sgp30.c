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
 * @file      driver_sgp30.c
 * @brief     driver sgp30 source file
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

#include "driver_sgp30.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Sensirion SGP30"        /**< chip name */
#define MANUFACTURER_NAME         "Sensirion"              /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        1.62f                    /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        1.98f                    /**< chip max supply voltage */
#define MAX_CURRENT               48.8f                    /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                   /**< chip min operating temperature */
#define TEMPERATURE_MAX           85.0f                    /**< chip max operating temperature */
#define DRIVER_VERSION            2000                     /**< driver version */

/**
 * @brief chip address definition
 */
#define SGP30_ADDRESS             (0x58 << 1)              /**< chip iic address */

/**
 * @brief chip command definition
 */
#define SGP30_COMMAND_IAQ_INIT                           0x2003U        /**< iaq init command */
#define SGP30_COMMAND_MEASURE_IAQ                        0x2008U        /**< measure iaq command */
#define SGP30_COMMAND_GET_IAQ_BASELINE                   0x2015U        /**< get iaq baseline command */
#define SGP30_COMMAND_SET_IAQ_BASELINE                   0x201EU        /**< set iaq baseline command */
#define SGP30_COMMAND_SET_ABSOLUTE_HUMIDITY              0x2061U        /**< set absolute humidity command */
#define SGP30_COMMAND_MEASURE_TEST                       0x2032U        /**< measure test command */
#define SGP30_COMMAND_GET_FEATURE_SET                    0x202FU        /**< get feature set command */
#define SGP30_COMMAND_MEASURE_RAW                        0x2050U        /**< measure raw command */
#define SGP30_COMMAND_GET_TVOC_INCEPTIVE_BASELINE        0x20B3U        /**< get tvoc inceptive baseline command */
#define SGP30_COMMAND_SET_TVOC_BASELINE                  0x2077U        /**< set tvoc baseline command */
#define SGP30_COMMAND_SOFT_RESET                         0x0006U        /**< soft reset command */
#define SGP30_COMMAND_GET_SERIAL_ID                      0x3682U        /**< get serial id command */

/**
 * @brief crc8 definition
 */
#define SGP30_CRC8_POLYNOMIAL                            0x31
#define SGP30_CRC8_INIT                                  0xFF

/**
 * @brief      read bytes
 * @param[in]  *handle points to an sgp30 handle structure
 * @param[in]  addr is the iic device address
 * @param[in]  reg is the iic register address
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @param[in]  delay_ms is the delay time in ms
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_sgp30_iic_read(sgp30_handle_t *handle, uint8_t addr, uint16_t reg, uint8_t *data, uint16_t len, uint16_t delay_ms)
{
    uint8_t buf[2];
    
    memset(buf, 0, sizeof(uint8_t) * 2);                            /* clear the buffer */
    buf[0] = (uint8_t)((reg >> 8) & 0xFF);                          /* set reg MSB */
    buf[1] = (uint8_t)(reg & 0xFF);                                 /* set reg LSB */
    if (handle->iic_write_cmd(addr, (uint8_t *)buf, 2) != 0)        /* write command */
    {   
        return 1;                                                   /* return error */
    }
    handle->delay_ms(delay_ms);                                     /* delay ms */
    if (handle->iic_read_cmd(addr, data, len) != 0)                 /* read data */
    {
        return 1;                                                   /* write command */
    }
    else
    {
        return 0;                                                   /* success return 0 */
    }
}

/**
 * @brief     write bytes
 * @param[in] *handle points to an sgp30 handle structure
 * @param[in] addr is the iic device address
 * @param[in] reg is the iic register address
 * @param[in] *data points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_sgp30_iic_write(sgp30_handle_t *handle, uint8_t addr, uint16_t reg, uint8_t *data, uint16_t len)
{
    uint8_t buf[16];
    uint16_t i;
        
    if ((len + 2) > 16)                                                   /* check length */
    {
        return 1;                                                         /* return error */
    }
    memset(buf, 0, sizeof(uint8_t) * 16);                                 /* clear the buffer */
    buf[0] = (uint8_t)((reg >> 8) & 0xFF);                                /* set MSB of reg */
    buf[1] = (uint8_t)(reg & 0xFF);                                       /* set LSB of reg */
    for (i = 0; i < len; i++)
    {
        buf[2 + i] = data[i];                                             /* copy write data */
    }
    
    if (handle->iic_write_cmd(addr, (uint8_t *)buf, len + 2) != 0)        /* write iic command */
    {
        return 1;                                                         /* write command */
    }
    else
    {
        return 0;                                                         /* success return 0 */
    }
}

/**
 * @brief     generate the crc
 * @param[in] *data points to a data buffer
 * @param[in] count is the data length
 * @return    crc
 * @note      none
 */
static uint8_t a_sgp30_generate_crc(uint8_t* data, uint8_t count) 
{
    uint8_t current_byte;
    uint8_t crc = SGP30_CRC8_INIT;
    uint8_t crc_bit;

    for (current_byte = 0; current_byte < count; ++current_byte)        /* calculate crc */
    {
        crc ^= (data[current_byte]);                                    /* xor data */
        for (crc_bit = 8; crc_bit > 0; --crc_bit)                       /* 8 bit */
        {
            if ((crc & 0x80) != 0)                                      /* if 7th bit is 1 */
            {
                crc = (crc << 1) ^ SGP30_CRC8_POLYNOMIAL;               /* xor */
            }
            else
            {
                crc = crc << 1;                                         /* left shift 1 */
            }
        }
    }
    
    return crc;                                                         /* return crc */
}

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
uint8_t sgp30_set_tvoc_baseline(sgp30_handle_t *handle, uint16_t tvoc_baseline)
{
    uint8_t res;
    uint8_t buf[3];
    
    if (handle == NULL)                                                                                          /* check handle */
    {
        return 2;                                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                                     /* check handle initialization */
    {
        return 3;                                                                                                /* return error */
    }

    memset(buf, 0, sizeof(uint8_t) * 3);                                                                         /* clear the buffer */
    buf[0] = (tvoc_baseline >> 8) & 0xFF;                                                                        /* get high part */
    buf[1] = (tvoc_baseline >> 0) & 0xFF;                                                                        /* get low part */
    buf[2] = a_sgp30_generate_crc((uint8_t *)buf, 2);                                                            /* generate crc */
    res = a_sgp30_iic_write(handle, SGP30_ADDRESS, SGP30_COMMAND_SET_TVOC_BASELINE, (uint8_t *)buf, 3);          /* write set tvoc baseline command */
    if (res != 0)                                                                                                /* check result */
    {
        handle->debug_print("sgp30: write tvoc baseline failed.\n");                                             /* write tvoc baseline failed */
       
        return 1;                                                                                                /* return error */
    }
    handle->delay_ms(10);                                                                                        /* wait 10 ms */
    
    return 0;                                                                                                    /* success return 0 */
}

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
uint8_t sgp30_get_tvoc_inceptive_baseline(sgp30_handle_t *handle, uint16_t *tvoc_baseline)
{
    uint8_t res;
    uint8_t buf[3];
    
    if (handle == NULL)                                                                                                   /* check handle */
    {
        return 2;                                                                                                         /* return error */
    }
    if (handle->inited != 1)                                                                                              /* check handle initialization */
    {
        return 3;                                                                                                         /* return error */
    }

    memset(buf, 0, sizeof(uint8_t) * 3);                                                                                  /* clear the buffer */
    res = a_sgp30_iic_read(handle, SGP30_ADDRESS, SGP30_COMMAND_GET_TVOC_INCEPTIVE_BASELINE, (uint8_t *)buf, 3, 10);      /* read tvoc inceptive baseline */
    if (res != 0)                                                                                                         /* check result */
    {
        handle->debug_print("sgp30: read tvoc baseline failed.\n");                                                       /* read tvoc baseline failed */
       
        return 1;                                                                                                         /* return error */
    }
    if (buf[2] != a_sgp30_generate_crc((uint8_t *)buf, 2))                                                                /* check crc */
    {
        handle->debug_print("sgp30: crc check error.\n");                                                                 /* crc check error */
       
        return 1;                                                                                                         /* return error */
    }
    *tvoc_baseline = (uint16_t)(((uint16_t)buf[0]) << 8 | buf[1]);                                                        /* get data */
    
    return 0;                                                                                                             /* success return 0 */
}

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
uint8_t sgp30_iaq_init(sgp30_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }

    res = a_sgp30_iic_write(handle, SGP30_ADDRESS, SGP30_COMMAND_IAQ_INIT, NULL, 0);          /* write iaq init command */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("sgp30: write iaq init failed.\n");                               /* write iaq init failed */
       
        return 1;                                                                             /* return error */
    }
    handle->delay_ms(10);                                                                     /* wait 10 ms */
    
    return 0;                                                                                 /* success return 0 */
}

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
uint8_t sgp30_soft_reset(sgp30_handle_t *handle)
{
    uint8_t res;
    uint8_t reg;
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }

    reg = 0x06;                                                          /* soft reset command */
    res = handle->iic_write_cmd(0x00, (uint8_t *)&reg, 1);               /* write reset config */
    if (res != 0)                                                        /* check result */
    {
        handle->debug_print("sgp30: write soft reset failed.\n");        /* write soft reset failed */
       
        return 1;                                                        /* return error */
    }
    
    return 0;                                                            /* success return 0 */
}

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
uint8_t sgp30_get_serial_id(sgp30_handle_t *handle, uint16_t id[3])
{
    uint8_t res;
    uint8_t buf[9];
    
    if (handle == NULL)                                                                                     /* check handle */
    {
        return 2;                                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                                /* check handle initialization */
    {
        return 3;                                                                                           /* return error */
    }

    memset(buf, 0, sizeof(uint8_t) * 9);                                                                    /* clear the buffer */
    res = a_sgp30_iic_read(handle, SGP30_ADDRESS, SGP30_COMMAND_GET_SERIAL_ID, (uint8_t *)buf, 9, 10);      /* read config */
    if (res != 0)                                                                                           /* check result */
    {
        handle->debug_print("sgp30: read serial id failed.\n");                                             /* read serial id failed */
       
        return 1;                                                                                           /* return error */
    }
    if (buf[2] != a_sgp30_generate_crc((uint8_t *)&buf[0], 2))                                              /* check 1st crc */
    {
        handle->debug_print("sgp30: crc 1 check failed.\n");                                                /* crc 1 check failed */
       
        return 1;                                                                                           /* return error */
    }
    if (buf[5] != a_sgp30_generate_crc((uint8_t *)&buf[3], 2))                                              /* check 2nd crc */
    {
        handle->debug_print("sgp30: crc 2 check failed.\n");                                                /* crc 2 check failed */
       
        return 1;                                                                                           /* return error */
    }    
    if (buf[8] != a_sgp30_generate_crc((uint8_t *)&buf[6], 2))                                              /* check 3rd crc */
    {
        handle->debug_print("sgp30: crc 3 check failed.\n");                                                /* crc 3 check failed */
       
        return 1;                                                                                           /* return error */
    }
    id[0] = (uint16_t)((((uint16_t)buf[0]) << 8) | buf[1]);                                                 /* set id 0 */
    id[1] = (uint16_t)((((uint16_t)buf[3]) << 8) | buf[4]);                                                 /* set id 1 */
    id[2] = (uint16_t)((((uint16_t)buf[6]) << 8) | buf[7]);                                                 /* set id 2 */
    
    return 0;                                                                                               /* success return 0 */
}

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
uint8_t sgp30_measure_iaq(sgp30_handle_t *handle, uint16_t *co2_eq_ppm, uint16_t *tvoc_ppb)
{
    uint8_t res;
    uint8_t buf[6];
    
    if (handle == NULL)                                                                                   /* check handle */
    {
        return 2;                                                                                         /* return error */
    }
    if (handle->inited != 1)                                                                              /* check handle initialization */
    {
        return 3;                                                                                         /* return error */
    }

    memset(buf, 0, sizeof(uint8_t) * 6);                                                                  /* clear the buffer */
    res = a_sgp30_iic_read(handle, SGP30_ADDRESS, SGP30_COMMAND_MEASURE_IAQ, (uint8_t *)buf, 6, 12);      /* read measure iaq */
    if (res != 0)                                                                                         /* check result */
    {
        handle->debug_print("sgp30: read measure iaq failed.\n");                                         /* read measure iaq failed */
       
        return 1;                                                                                         /* return error */
    }
    if (buf[2] != a_sgp30_generate_crc((uint8_t *)buf, 2))                                                /* calculate crc */
    {
        handle->debug_print("sgp30: co2 eq crc check error.\n");                                          /* co2 eq crc check error */
       
        return 1;                                                                                         /* return error */
    }
    if (buf[5] != a_sgp30_generate_crc((uint8_t *)&buf[3], 2))                                            /* calculate crc */
    {
        handle->debug_print("sgp30: tvoc crc check error.\n");                                            /* tvoc crc check error */
       
        return 1;                                                                                         /* return error */
    }
    *co2_eq_ppm = (uint16_t)(((uint16_t)buf[0]) << 8 | buf[1]);                                           /* get co2 eq ppm */
    *tvoc_ppb = (uint16_t)(((uint16_t)buf[3]) << 8 | buf[4]);                                             /* get tvoc ppb */
    
    return 0;                                                                                             /* success return 0 */
}

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
uint8_t sgp30_get_iaq_baseline(sgp30_handle_t *handle, uint16_t *tvoc, uint16_t *co2_eq)
{
    uint8_t res;
    uint8_t buf[6];
    
    if (handle == NULL)                                                                                        /* check handle */
    {
        return 2;                                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                                   /* check handle initialization */
    {
        return 3;                                                                                              /* return error */
    }

    memset(buf, 0, sizeof(uint8_t) * 6);                                                                       /* clear the buffer */
    res = a_sgp30_iic_read(handle, SGP30_ADDRESS, SGP30_COMMAND_GET_IAQ_BASELINE, (uint8_t *)buf, 6, 10);      /* read iaq baseline */
    if (res != 0)                                                                                              /* check result */
    {
        handle->debug_print("sgp30: read measure iaq failed.\n");                                              /* read measure iaq failed */
       
        return 1;                                                                                              /* return error */
    }
    if (buf[2] != a_sgp30_generate_crc((uint8_t *)buf, 2))                                                     /* check co2 crc */
    {
        handle->debug_print("sgp30: co2 eq crc check error.\n");                                               /* co2 eq crc check error */
       
        return 1;                                                                                              /* return error */
    }
    if (buf[5] != a_sgp30_generate_crc((uint8_t *)&buf[3], 2))                                                 /* check tvoc crc */
    {
        handle->debug_print("sgp30: tvoc crc check error.\n");                                                 /* tvoc crc check error */
        
        return 1;                                                                                              /* return error */
    }
    *co2_eq = (uint16_t)(((uint16_t)buf[0]) << 8 | buf[1]);                                                    /* get co2 eq */
    *tvoc = (uint16_t)(((uint16_t)buf[3]) << 8 | buf[4]);                                                      /* get tvoc */
    
    return 0;                                                                                                  /* success return 0 */
}

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
uint8_t sgp30_set_iaq_baseline(sgp30_handle_t *handle, uint16_t tvoc, uint16_t co2_eq)
{
    uint8_t res;
    uint8_t buf[6];
    
    if (handle == NULL)                                                                                         /* check handle */
    {
        return 2;                                                                                               /* return error */
    }
    if (handle->inited != 1)                                                                                    /* check handle initialization */
    {
        return 3;                                                                                               /* return error */
    }
    
    memset(buf, 0, sizeof(uint8_t) * 6);                                                                        /* clear the buffer */
    buf[0] = (tvoc >> 8) & 0xFF;                                                                                /* set tvoc high part */
    buf[1] = tvoc & 0xFF;                                                                                       /* set tvoc low part */
    buf[2] = a_sgp30_generate_crc((uint8_t *)buf, 2);                                                           /* generate tvoc crc */
    buf[3] = (co2_eq >> 8) & 0xFF;                                                                              /* set co2 eq high part */
    buf[4] = co2_eq & 0xFF;                                                                                     /* set co2 eq low part */
    buf[5] = a_sgp30_generate_crc((uint8_t *)&buf[3], 2);                                                       /* generate co2_eq crc */
    res = a_sgp30_iic_write(handle, SGP30_ADDRESS, SGP30_COMMAND_SET_IAQ_BASELINE, (uint8_t *)buf, 6);          /* write iaq baseline command */
    if (res != 0)                                                                                               /* check result */
    {
        handle->debug_print("sgp30: write iaq baseline failed.\n");                                             /* write iaq baseline failed */
       
        return 1;                                                                                               /* return error */
    }
    handle->delay_ms(10);                                                                                       /* delay 10 ms */
    
    return 0;                                                                                                   /* success return 0 */
}

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
uint8_t sgp30_set_absolute_humidity(sgp30_handle_t *handle, uint16_t humidity)
{
    uint8_t res;
    uint8_t buf[3];
    
    if (handle == NULL)                                                                                              /* check handle */
    {
        return 2;                                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                                         /* check handle initialization */
    {
        return 3;                                                                                                    /* return error */
    }

    memset(buf, 0, sizeof(uint8_t) * 3);                                                                             /* clear the buffer */
    buf[0] = (humidity >> 8) & 0xFF;                                                                                 /* set humidity high part */
    buf[1] = (humidity >> 0) & 0xFF;                                                                                 /* set humidity low part */
    buf[2] = a_sgp30_generate_crc((uint8_t *)buf, 2);                                                                /* generate crc */
    res = a_sgp30_iic_write(handle, SGP30_ADDRESS, SGP30_COMMAND_SET_ABSOLUTE_HUMIDITY, (uint8_t *)buf, 3);          /* write set absolute humidity command */
    if (res != 0)                                                                                                    /* check result */
    {
        handle->debug_print("sgp30: write absolute humidity failed.\n");                                             /* write absolute humidity failed */
       
        return 1;                                                                                                    /* return error */
    }
    handle->delay_ms(10);                                                                                            /* delay 10 ms */
    
    return 0;                                                                                                        /* success return 0 */
}

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
uint8_t sgp30_absolute_humidity_convert_to_register(sgp30_handle_t *handle, float temp, float rh, uint16_t *reg)
{
    float absolute_humidity;
    float intpart, fractpart;
    
    if (handle == NULL)                                                                                                 /* check handle */
    {
        return 2;                                                                                                       /* return error */
    }
    if (handle->inited != 1)                                                                                            /* check handle initialization */
    {
        return 3;                                                                                                       /* return error */
    }
    
    absolute_humidity = (rh / 100.0f * 6.112f * powf(17.62f * temp, 243.12f + temp)) / (273.15f + temp) * 216.7f;       /* get absolute humidity */
    fractpart = modff(absolute_humidity, (float *)&intpart);                                                            /* get intpart and fractpart */
    *reg = (uint16_t)(intpart) << 8 | (uint8_t)(fractpart * 256);                                                       /* convert to register */
    
    return 0;                                                                                                           /* success return 0 */
}

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
uint8_t sgp30_measure_test(sgp30_handle_t *handle, uint16_t *result)
{
    uint8_t res;
    uint8_t buf[3];
    
    if (handle == NULL)                                                                                    /* check handle */
    {
        return 2;                                                                                          /* return error */
    }
    if (handle->inited != 1)                                                                               /* check handle initialization */
    {
        return 3;                                                                                          /* return error */
    }

    memset(buf, 0, sizeof(uint8_t) * 3);                                                                   /* clear the buffer */
    res = a_sgp30_iic_read(handle, SGP30_ADDRESS, SGP30_COMMAND_MEASURE_TEST, (uint8_t *)buf, 3, 220);     /* read measure test */
    if (res != 0)                                                                                          /* check result */
    {
        handle->debug_print("sgp30: read measure test failed.\n");                                         /* read measure test failed */
       
        return 1;                                                                                          /* return error */
    }
    if (buf[2] != a_sgp30_generate_crc((uint8_t *)buf, 2))                                                 /* check crc */
    {
        handle->debug_print("sgp30: measure test check error.\n");                                         /* measure test check error */
       
        return 1;                                                                                          /* return error */
    }
    *result = (uint16_t)(((uint16_t)buf[0]) << 8 | buf[1]);                                                /* combine data */
    
    return 0;                                                                                              /* success return 0 */
}

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
uint8_t sgp30_get_feature_set(sgp30_handle_t *handle, uint8_t *product_type, uint8_t *product_version)
{
    uint8_t res;
    uint8_t buf[3];
    
    if (handle == NULL)                                                                                       /* check handle */
    {
        return 2;                                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                                  /* check handle initialization */
    {
        return 3;                                                                                             /* return error */
    }

    memset(buf, 0, sizeof(uint8_t) * 3);                                                                      /* clear the buffer */
    res = a_sgp30_iic_read(handle, SGP30_ADDRESS, SGP30_COMMAND_GET_FEATURE_SET, (uint8_t *)buf, 3, 10);      /* read get feature set */
    if (res != 0)                                                                                             /* check result */
    {
        handle->debug_print("sgp30: get feature set failed.\n");                                              /* get feature set failed */
       
        return 1;                                                                                             /* return error */
    }
    if (buf[2] != a_sgp30_generate_crc((uint8_t *)buf, 2))                                                    /* check crc */
    {
        handle->debug_print("sgp30: feature set check error.\n");                                             /* crc check error */
       
        return 1;                                                                                             /* return error */
    }
    *product_type = (buf[0] >> 4) & 0xF;                                                                      /* get product type */
    *product_version = buf[1];                                                                                /* get product version */
    
    return 0;                                                                                                 /* success return 0 */
}

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
uint8_t sgp30_get_measure_raw(sgp30_handle_t *handle, uint16_t *tvoc, uint16_t *co2_eq)
{
    uint8_t res;
    uint8_t buf[6];
    
    if (handle == NULL)                                                                                     /* check handle */
    {
        return 2;                                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                                /* check handle initialization */
    {
        return 3;                                                                                           /* return error */
    }

    memset(buf, 0, sizeof(uint8_t) * 6);                                                                    /* clear the buffer */
    res = a_sgp30_iic_read(handle, SGP30_ADDRESS, SGP30_COMMAND_MEASURE_RAW, (uint8_t *)buf, 6, 25);        /* read measure raw */
    if (res != 0)                                                                                           /* check result */
    {
        handle->debug_print("sgp30: read measure raw failed.\n");                                           /* read measure failed */
       
        return 1;                                                                                           /* return error */
    }
    if (buf[2] != a_sgp30_generate_crc((uint8_t *)buf, 2))                                                  /* check 1st crc */
    {
        handle->debug_print("sgp30: co2 eq crc check error.\n");                                            /* co2 eq crc check error */
       
        return 1;                                                                                           /* return error */
    }
    if (buf[5] != a_sgp30_generate_crc((uint8_t *)&buf[3], 2))                                              /* check 2nd crc */
    {
        handle->debug_print("sgp30: tvoc crc check error.\n");                                              /* tvoc crc check error */
       
        return 1;                                                                                           /* return error */
    }
    *tvoc = (uint16_t)(((uint16_t)buf[0]) << 8 | buf[1]);                                                   /* get tvoc data */
    *co2_eq = (uint16_t)(((uint16_t)buf[3]) << 8 | buf[4]);                                                 /* get co2 eq data */
    
    return 0;                                                                                               /* success return 0 */
}

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
uint8_t sgp30_init(sgp30_handle_t *handle)
{ 
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->debug_print == NULL)                                         /* check debug_print */
    {
        return 3;                                                            /* return error */
    }
    if (handle->iic_init == NULL)                                            /* check iic_init */
    {
        handle->debug_print("sgp30: iic_init is null.\n");                   /* iic_init is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->iic_deinit == NULL)                                          /* check iic_deinit */
    {
        handle->debug_print("sgp30: iic_deinit is null.\n");                 /* iic_deinit is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->iic_write_cmd == NULL)                                       /* check iic_write_cmd */
    {
        handle->debug_print("sgp30: iic_write_cmd is null.\n");              /* iic_write_cmd is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->iic_read_cmd == NULL)                                        /* check iic_read_cmd */
    {
        handle->debug_print("sgp30: iic_read_cmd is null.\n");               /* iic_read_cmd is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->delay_ms == NULL)                                            /* check delay_ms */
    {
        handle->debug_print("sgp30: delay_ms is null.\n");                   /* delay_ms is null */
    
        return 3;                                                            /* return error */
    }
    
    if (handle->iic_init() != 0)                                             /* iic init */
    {
        handle->debug_print("sgp30: iic init failed.\n");                    /* iic init failed */
    
        return 3;                                                            /* return error */
    }
    handle->inited = 1;                                                      /* flag finish initialization */
  
    return 0;                                                                /* success return 0 */
}

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
uint8_t sgp30_deinit(sgp30_handle_t *handle)
{
    if (handle == NULL)                                           /* check handle */
    {
        return 2;                                                 /* return error */
    }
    if (handle->inited != 1)                                      /* check handle initialization */
    {
        return 3;                                                 /* return error */
    }    
    
    if (sgp30_soft_reset(handle) != 0)                            /* reset chip */
    {
        handle->debug_print("sgp30: soft reset failed.\n");       /* soft reset failed */
    
        return 4;                                                 /* return error */
    }
    if (handle->iic_deinit() != 0)                                /* iic deinit */
    {
        handle->debug_print("sgp30: iic close failed.\n");        /* iic close failed */
    
        return 3;                                                 /* return error */
    }
    handle->inited = 0;                                           /* flag close initialization */
  
    return 0;                                                     /* success return 0 */
}

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
uint8_t sgp30_read(sgp30_handle_t *handle, uint16_t *co2_eq_ppm, uint16_t *tvoc_ppb)
{
    uint8_t res;
    uint8_t buf[6];
    
    if (handle == NULL)                                                                                   /* check handle */
    {
        return 2;                                                                                         /* return error */
    }
    if (handle->inited != 1)                                                                              /* check handle initialization */
    {
        return 3;                                                                                         /* return error */
    }

    memset(buf, 0, sizeof(uint8_t) * 6);                                                                  /* clear the buffer */
    res = a_sgp30_iic_read(handle, SGP30_ADDRESS, SGP30_COMMAND_MEASURE_IAQ, (uint8_t *)buf, 6, 12);      /* read measure iaq */
    if (res != 0)                                                                                         /* check result */
    {
        handle->debug_print("sgp30: read measure iaq failed.\n");                                         /* read measure iaq failed */
       
        return 1;                                                                                         /* return error */
    }
    if (buf[2] != a_sgp30_generate_crc((uint8_t *)buf, 2))                                                /* check 1st crc */
    {
        handle->debug_print("sgp30: co2 eq crc check error.\n");                                          /* co2 eq crc check error */
       
        return 1;                                                                                         /* return error */
    }
    if (buf[5] != a_sgp30_generate_crc((uint8_t *)&buf[3], 2))                                            /* check 2nd crc */
    {
        handle->debug_print("sgp30: tvoc crc check error.\n");                                            /* tvoc crc check error */
       
        return 1;                                                                                         /* return error */
    }
    *co2_eq_ppm = (uint16_t)(((uint16_t)buf[0]) << 8 | buf[1]);                                           /* get co2 eq ppm data */
    *tvoc_ppb = (uint16_t)(((uint16_t)buf[3]) << 8 | buf[4]);                                             /* get tvoc ppb data */
    
    return 0;                                                                                             /* success return 0 */
}

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
uint8_t sgp30_set_reg(sgp30_handle_t *handle, uint16_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }
  
    return a_sgp30_iic_write(handle, SGP30_ADDRESS, reg, buf, len);          /* write data */
}

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
uint8_t sgp30_get_reg(sgp30_handle_t *handle, uint16_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                                     /* check handle */
    {
        return 2;                                                           /* return error */
    }
    if (handle->inited != 1)                                                /* check handle initialization */
    {
        return 3;                                                           /* return error */
    }
  
    return a_sgp30_iic_read(handle, SGP30_ADDRESS, reg, buf, len, 20);      /* read data */
}

/**
 * @brief      get chip information
 * @param[out] *info points to an sgp30 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t sgp30_info(sgp30_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(sgp30_info_t));                          /* initialize sgp30 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "IIC", 8);                             /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
