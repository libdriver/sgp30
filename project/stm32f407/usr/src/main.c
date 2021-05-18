/**
 * Copyright (C) LibDriver 2015-2021 All rights reserved
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
 * @file      main.c
 * @brief     main source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2021-03-07
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/03/07  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_sgp30_basic.h"
#include "driver_sgp30_advance.h"
#include "driver_sgp30_register_test.h"
#include "driver_sgp30_read_test.h"
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "uart.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];        /**< uart buffer */
uint16_t g_len;            /**< uart buffer length */

/**
 * @brief     sgp30 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 *             - 1 run failed
 *             - 5 param is invalid
 * @note      none
 */
uint8_t sgp30(uint8_t argc, char **argv)
{
    if (argc == 1)
    {
        goto help;
    }
    else if (argc == 2)
    {
        if (strcmp("-i", argv[1]) == 0)
        {
            sgp30_info_t info;
            
            /* print sgp30 info */
            sgp30_info(&info);
            sgp30_interface_debug_print("sgp30: chip is %s.\n", info.chip_name);
            sgp30_interface_debug_print("sgp30: manufacturer is %s.\n", info.manufacturer_name);
            sgp30_interface_debug_print("sgp30: interface is %s.\n", info.interface);
            sgp30_interface_debug_print("sgp30: driver version is %d.%d.\n", info.driver_version/1000, (info.driver_version%1000)/100);
            sgp30_interface_debug_print("sgp30: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
            sgp30_interface_debug_print("sgp30: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
            sgp30_interface_debug_print("sgp30: max current is %0.2fmA.\n", info.max_current_ma);
            sgp30_interface_debug_print("sgp30: max temperature is %0.1fC.\n", info.temperature_max);
            sgp30_interface_debug_print("sgp30: min temperature is %0.1fC.\n", info.temperature_min);
            
            return 0;
        }
        else if (strcmp("-p", argv[1]) == 0)
        {
            /* print pin connection */
            sgp30_interface_debug_print("sgp30: SCL connected to GPIOB PIN8.\n");
            sgp30_interface_debug_print("sgp30: SDA connected to GPIOB PIN9.\n");
            
            return 0;
        }
        else if (strcmp("-h", argv[1]) == 0)
        {
            /* show sgp30 help */
            
            help:
            
            sgp30_interface_debug_print("sgp30 -i\n\tshow sgp30 chip and driver information.\n");
            sgp30_interface_debug_print("sgp30 -h\n\tshow sgp30 help.\n");
            sgp30_interface_debug_print("sgp30 -p\n\tshow sgp30 pin connections of the current board.\n");
            sgp30_interface_debug_print("sgp30 -t reg\n\trun sgp30 register test.\n");
            sgp30_interface_debug_print("sgp30 -t read <times>\n\trun sgp30 read test.times means test times.\n");
            sgp30_interface_debug_print("sgp30 -c read <times>\n\trun spg30 read function.times means read times.\n");
            sgp30_interface_debug_print("sgp30 -c advance -read <times>\n\trun spg30 advance read function.times means read times.\n");
            sgp30_interface_debug_print("sgp30 -c advance -read <times> -baseline <tvoc> <co2eq>\n\trun spg30 advance read function with baseline.times means read times.");
            sgp30_interface_debug_print("tvoc means current tvoc.co2eq means current co2eq.\n");
            sgp30_interface_debug_print("sgp30 -c advance -read <times> -humidity <temperature> <rh>\n\trun spg30 advance read function with humidity.times means read times.");
            sgp30_interface_debug_print("temperature means current temperature.rh means current relative humidity.\n");
            sgp30_interface_debug_print("sgp30 -c -info id\n\tget spg30 id information.\n");
            sgp30_interface_debug_print("sgp30 -c -info product\n\tget spg30 product information.\n");
            
            return 0;
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 3)
    {
        /* run test */
        if (strcmp("-t", argv[1]) == 0)
        {
             /* reg test */
            if (strcmp("reg", argv[2]) == 0)
            {
                /* run reg test */
                if (sgp30_register_test())
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    else if (argc == 4)
    {
        /* run test */
        if (strcmp("-t", argv[1]) == 0)
        {
             /* read test */
            if (strcmp("read", argv[2]) == 0)
            {
                /* run read test */
                if (sgp30_read_test(atoi(argv[3])))
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* run function */
        else if (strcmp("-c", argv[1]) == 0)
        {
            /* read function */
            if (strcmp("read", argv[2]) == 0)
            {
                volatile uint8_t res;
                volatile uint32_t times;
                volatile uint32_t i;
                volatile uint16_t co2_eq_ppm;
                volatile uint16_t tvoc_ppb;
                
                res = sgp30_basic_init();
                if (res)
                {
                    return 1;
                }
                times = atoi(argv[3]);
                for (i=0; i<times; i++)
                {
                    sgp30_interface_delay_ms(1000);
                    res = sgp30_basic_read((uint16_t *)&co2_eq_ppm, (uint16_t *)&tvoc_ppb);
                    if (res)
                    {
                        sgp30_basic_deinit();
                        
                        return 1;
                    }
                    sgp30_interface_debug_print("sgp30: %d/%d.\n", (uint32_t)(i+1), (uint32_t)times);
                    sgp30_interface_debug_print("sgp30: co2 eq is %d ppm.\n", co2_eq_ppm);
                    sgp30_interface_debug_print("sgp30: tvoc is %d ppb.\n", tvoc_ppb);
                }
                sgp30_basic_deinit();
                
                return 0;
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    else if (argc == 5)
    {
        /* run advance */
        if (strcmp("-c", argv[1]) == 0)
        {
             /* advance */
            if (strcmp("advance", argv[2]) == 0)
            {
                if (strcmp("-read", argv[3]) == 0)
                {
                    volatile uint8_t res;
                    volatile uint32_t times;
                    volatile uint32_t i;
                    volatile uint16_t co2_eq_ppm;
                    volatile uint16_t tvoc_ppb;
                    
                    res = sgp30_advance_init();
                    if (res)
                    {
                        return 1;
                    }
                    times = atoi(argv[4]);
                    for (i=0; i<times; i++)
                    {
                        sgp30_interface_delay_ms(1000);
                        res = sgp30_advance_read((uint16_t *)&co2_eq_ppm, (uint16_t *)&tvoc_ppb);
                        if (res)
                        {
                            sgp30_advance_deinit();
                            
                            return 1;
                        }
                        sgp30_interface_debug_print("sgp30: %d/%d.\n", (uint32_t)(i+1), (uint32_t)times);
                        sgp30_interface_debug_print("sgp30: co2 eq is %d ppm.\n", co2_eq_ppm);
                        sgp30_interface_debug_print("sgp30: tvoc is %d ppb.\n", tvoc_ppb);
                    }
                    sgp30_advance_deinit();
                    
                    return 0;
                }
                else if (strcmp("-info", argv[3]) == 0)
                {
                    volatile uint8_t res;
                     
                    res = sgp30_advance_init();
                    if (res)
                    {
                        return 1;
                    }
                    if (strcmp("id", argv[4]) == 0)
                    {
                        uint16_t id[3];
                        
                        res = sgp30_advance_get_serial_id((uint16_t *)id);
                        if (res)
                        {
                            sgp30_advance_deinit();
                            
                            return 1;
                        }
                        sgp30_interface_debug_print("sgp30: serial id 0x%04X 0x%04X 0x%04X.\n", (uint16_t)(id[0]), (uint16_t)(id[1]), (uint16_t)(id[2]));
                        sgp30_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("product", argv[4]) == 0)
                    {
                        volatile uint8_t product_type;
                        volatile uint8_t product_version;
                        
                        res = sgp30_advance_get_feature((uint8_t *)&product_type, (uint8_t *)&product_version);
                        if (res)
                        {
                            sgp30_advance_deinit();
                            
                            return 1;
                        }
                        sgp30_interface_debug_print("sgp30: product type is 0x%02X.\n", product_type);
                        sgp30_interface_debug_print("sgp30: product version is 0x%02X.\n", product_version);
                        sgp30_advance_deinit();
                        
                        return 0;
                    }
                    else
                    {
                        sgp30_interface_debug_print("sgp30: info is invalid.\n");
                        sgp30_advance_deinit();
                        
                        return 5;
                    }
                }
                /* param is invalid */
                else
                {
                    return 5;
                }
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    else if (argc == 8)
    {
        /* run advance */
        if (strcmp("-c", argv[1]) == 0)
        {
             /* advance */
            if (strcmp("advance", argv[2]) == 0)
            {
                if (strcmp("-read", argv[3]) == 0)
                {
                    volatile uint8_t res;
                    volatile uint32_t i, times;
                    volatile uint16_t co2_eq_ppm;
                    volatile uint16_t tvoc_ppb;
                    
                    res = sgp30_advance_init();
                    if (res)
                    {
                        return 1;
                    }
                    times = atoi(argv[4]);
                    if (strcmp("-baseline", argv[5]) == 0)
                    {
                        sgp30_interface_debug_print("sgp30: tvoc is 0x%04X.\n", atoi(argv[6]));
                        sgp30_interface_debug_print("sgp30: co2 eq is 0x%04X.\n", atoi(argv[7]));
                        res = sgp30_advance_set_iaq_baseline(atoi(argv[6]), atoi(argv[7]));
                        if (res)
                        {
                            sgp30_advance_deinit();
                            
                            return 1;
                        }
                        for (i=0; i<times; i++)
                        {
                            sgp30_interface_delay_ms(1000);
                            res = sgp30_advance_read((uint16_t *)&co2_eq_ppm, (uint16_t *)&tvoc_ppb);
                            if (res)
                            {
                                sgp30_advance_deinit();
                                
                                return 1;
                            }
                            sgp30_interface_debug_print("sgp30: %d/%d.\n", (uint32_t)(i+1), (uint32_t)times);
                            sgp30_interface_debug_print("sgp30: co2 eq is %d ppm.\n", co2_eq_ppm);
                            sgp30_interface_debug_print("sgp30: tvoc is %d ppb.\n", tvoc_ppb);
                        }
                        sgp30_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("-humidity", argv[5]) == 0)
                    {
                        sgp30_interface_debug_print("sgp30: temp is %0.2fC.\n", atof(argv[6]));
                        sgp30_interface_debug_print("sgp30: rh is %0.2f%%.\n", atof(argv[7]));
                        res = sgp30_advance_set_absolute_humidity(atof(argv[6]), atof(argv[7]));
                        if (res)
                        {
                            sgp30_advance_deinit();
                            
                            return 1;
                        }
                        for (i=0; i<times; i++)
                        {
                            sgp30_interface_delay_ms(1000);
                            res = sgp30_advance_read((uint16_t *)&co2_eq_ppm, (uint16_t *)&tvoc_ppb);
                            if (res)
                            {
                                sgp30_advance_deinit();
                                
                                return 1;
                            }
                            sgp30_interface_debug_print("sgp30: %d/%d.\n", (uint32_t)(i+1), (uint32_t)times);
                            sgp30_interface_debug_print("sgp30: co2 eq is %d ppm.\n", co2_eq_ppm);
                            sgp30_interface_debug_print("sgp30: tvoc is %d ppb.\n", tvoc_ppb);
                        }
                        sgp30_advance_deinit();
                        
                        return 0;
                    }
                    /* param is invalid */
                    else
                    {
                        sgp30_advance_deinit();
                        
                        return 5;
                    }
                }
                /* param is invalid */
                else
                {
                    return 5;
                }
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    /* param is invalid */
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    volatile uint8_t res;
    
    /* stm32f407 clock init and hal init */
    clock_init();
    
    /* delay init */
    delay_init();
    
    /* uart1 init */
    uart1_init(115200);
    
    /* shell init && register sgp30 fuction */
    shell_init();
    shell_register("sgp30", sgp30);
    uart1_print("sgp30: welcome to libdriver sgp30.\n");
    
    while (1)
    {
        /* read uart */
        g_len = uart1_read(g_buf, 256);
        if (g_len)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart1_print("sgp30: run failed.\n");
            }
            else if (res == 2)
            {
                uart1_print("sgp30: unknow command.\n");
            }
            else if (res == 3)
            {
                uart1_print("sgp30: length is too long.\n");
            }
            else if (res == 4)
            {
                uart1_print("sgp30: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart1_print("sgp30: param is invalid.\n");
            }
            else
            {
                uart1_print("sgp30: unknow status code.\n");
            }
            uart1_flush();
        }
        delay_ms(100);
    }
}
