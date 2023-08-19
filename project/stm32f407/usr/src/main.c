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
#include "getopt.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];        /**< uart buffer */
volatile uint16_t g_len;   /**< uart buffer length */

/**
 * @brief     sgp30 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t sgp30(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"baseline-co2eq", required_argument, NULL, 1},
        {"baseline-tvoc", required_argument, NULL, 2},
        {"humidity-rh", required_argument, NULL, 3},
        {"humidity-temperature", required_argument, NULL, 4},
        {"times", required_argument, NULL, 5},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    uint32_t times = 3;
    uint16_t co2_eq_ppm = 0;
    uint16_t tvoc_ppb = 0;
    float rh = 50.0f;
    float temp = 25.0f;

    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* baseline co2eq */
            case 1 :
            {
                /* set the baseline co2 eq */
                co2_eq_ppm = atoi(optarg);
                
                break;
            }
            
            /* baseline tvoc */
            case 2 :
            {
                /* set the baseline tvoc  */
                tvoc_ppb = atoi(optarg);
                
                break;
            }
            
            /* rh */
            case 3 :
            {
                rh = atof(optarg);
                
                break;
            }
             
            /* temperature */
            case 4 :
            {
                temp = atof(optarg);
                
                break;
            }
            
            /* running times */
            case 5 :
            {
                /* set the times */
                times = atol(optarg);
                
                break;
            } 
            
            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        /* run reg test */
        if (sgp30_register_test() != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("t_read", type) == 0)
    {
        /* run read test */
        if (sgp30_read_test(times) != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("e_read", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        
        /* init */
        res = sgp30_basic_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            /* delay 1000ms */
            sgp30_interface_delay_ms(1000);
            
            /* read data */
            res = sgp30_basic_read((uint16_t *)&co2_eq_ppm, (uint16_t *)&tvoc_ppb);
            if (res != 0)
            {
                (void)sgp30_basic_deinit();
                
                return 1;
            }
            
            /* output */
            sgp30_interface_debug_print("sgp30: %d/%d.\n", (uint32_t)(i + 1), (uint32_t)times);
            sgp30_interface_debug_print("sgp30: co2 eq is %d ppm.\n", co2_eq_ppm);
            sgp30_interface_debug_print("sgp30: tvoc is %d ppb.\n", tvoc_ppb);
        }
        
        /* deinit */
        (void)sgp30_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_advance-read", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        
        /* init */
        res = sgp30_advance_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* set the iaq baseline */
        res = sgp30_advance_set_iaq_baseline(tvoc_ppb, co2_eq_ppm);
        if (res != 0)
        {
            (void)sgp30_advance_deinit();
            
            return 1;
        }
        
        /* set the temp && rh */
        res = sgp30_advance_set_absolute_humidity(temp, rh);
        if (res != 0)
        {
            (void)sgp30_advance_deinit();
            
            return 1;
        }
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            /* delay 1000ms */
            sgp30_interface_delay_ms(1000);
            
            /* read data */
            res = sgp30_advance_read((uint16_t *)&co2_eq_ppm, (uint16_t *)&tvoc_ppb);
            if (res != 0)
            {
                (void)sgp30_advance_deinit();
                
                return 1;
            }
            
            /* output */
            sgp30_interface_debug_print("sgp30: %d/%d.\n", (uint32_t)(i + 1), (uint32_t)times);
            sgp30_interface_debug_print("sgp30: co2 eq is %d ppm.\n", co2_eq_ppm);
            sgp30_interface_debug_print("sgp30: tvoc is %d ppb.\n", tvoc_ppb);
        }
        
        /* deinit */
        (void)sgp30_advance_deinit();
        
        return 0;
    }
    else if (strcmp("e_info", type) == 0)
    {
        uint8_t res;
        uint8_t product_type;
        uint8_t product_version;
        uint16_t id[3];
        
        /* init */
        res = sgp30_advance_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* get the id */
        res = sgp30_advance_get_serial_id((uint16_t *)id);
        if (res != 0)
        {
            (void)sgp30_advance_deinit();
            
            return 1;
        }
        
        /* get the type */
        res = sgp30_advance_get_feature((uint8_t *)&product_type, (uint8_t *)&product_version);
        if (res != 0)
        {
            (void)sgp30_advance_deinit();
            
            return 1;
        }
        
        /* output */
        sgp30_interface_debug_print("sgp30: serial id 0x%04X 0x%04X 0x%04X.\n", (uint16_t)(id[0]), (uint16_t)(id[1]), (uint16_t)(id[2]));
        sgp30_interface_debug_print("sgp30: product type is 0x%02X.\n", product_type);
        sgp30_interface_debug_print("sgp30: product version is 0x%02X.\n", product_version);
        
        /* deinit */
        (void)sgp30_advance_deinit();
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        sgp30_interface_debug_print("Usage:\n");
        sgp30_interface_debug_print("  sgp30 (-i | --information)\n");
        sgp30_interface_debug_print("  sgp30 (-h | --help)\n");
        sgp30_interface_debug_print("  sgp30 (-p | --port)\n");
        sgp30_interface_debug_print("  sgp30 (-t reg | --test=reg)\n");
        sgp30_interface_debug_print("  sgp30 (-t read | --test=read) [--times=<num>]\n");
        sgp30_interface_debug_print("  sgp30 (-e read | --example=read) [--times=<num>]\n");
        sgp30_interface_debug_print("  sgp30 (-e advance-read | --example=advance-read) [--times=<num>] [--baseline-tvoc=<ppb>] [--baseline-co2eq=<ppm>]\n");
        sgp30_interface_debug_print("        [--humidity-temperature=<temp>] [--humidity-rh=<rh>]\n");
        sgp30_interface_debug_print("  sgp30 (-e info | --example=info)\n");
        sgp30_interface_debug_print("\n");
        sgp30_interface_debug_print("Options:\n");
        sgp30_interface_debug_print("      --baseline-co2eq=<ppm>              Set the co2eq baseline.([default: 0])\n");
        sgp30_interface_debug_print("      --baseline-tvoc=<ppb>               Set the tvoc baseline.([default: 0])\n");
        sgp30_interface_debug_print("  -e <read | advance-read | info>, --example=<read | advance-read | info>\n");
        sgp30_interface_debug_print("                                          Run the driver example.\n");
        sgp30_interface_debug_print("  -h, --help                              Show the help.\n");
        sgp30_interface_debug_print("      --humidity-rh=<rh>                  Set the humidity rh.([default: 50.0f])\n");
        sgp30_interface_debug_print("      --humidity-temperature=<temp>       Set the humidity temperature.([default: 25.0f])\n");
        sgp30_interface_debug_print("  -i, --information                       Show the chip information.\n");
        sgp30_interface_debug_print("  -p, --port                              Display the pin connections of the current board.\n");
        sgp30_interface_debug_print("  -t <reg | read>, --test=<reg | read>    Run the driver test.\n");
        sgp30_interface_debug_print("      --times=<num>                       Set the running times.([default: 3])\n");
        
        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        sgp30_info_t info;
        
        /* print sgp30 info */
        sgp30_info(&info);
        sgp30_interface_debug_print("sgp30: chip is %s.\n", info.chip_name);
        sgp30_interface_debug_print("sgp30: manufacturer is %s.\n", info.manufacturer_name);
        sgp30_interface_debug_print("sgp30: interface is %s.\n", info.interface);
        sgp30_interface_debug_print("sgp30: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        sgp30_interface_debug_print("sgp30: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        sgp30_interface_debug_print("sgp30: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        sgp30_interface_debug_print("sgp30: max current is %0.2fmA.\n", info.max_current_ma);
        sgp30_interface_debug_print("sgp30: max temperature is %0.1fC.\n", info.temperature_max);
        sgp30_interface_debug_print("sgp30: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        sgp30_interface_debug_print("sgp30: SCL connected to GPIOB PIN8.\n");
        sgp30_interface_debug_print("sgp30: SDA connected to GPIOB PIN9.\n");
        
        return 0;
    }
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
    uint8_t res;
    
    /* stm32f407 clock init and hal init */
    clock_init();
    
    /* delay init */
    delay_init();
    
    /* uart init */
    uart_init(115200);
    
    /* shell init && register sgp30 function */
    shell_init();
    shell_register("sgp30", sgp30);
    uart_print("sgp30: welcome to libdriver sgp30.\n");
    
    while (1)
    {
        /* read uart */
        g_len = uart_read(g_buf, 256);
        if (g_len != 0)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart_print("sgp30: run failed.\n");
            }
            else if (res == 2)
            {
                uart_print("sgp30: unknown command.\n");
            }
            else if (res == 3)
            {
                uart_print("sgp30: length is too long.\n");
            }
            else if (res == 4)
            {
                uart_print("sgp30: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart_print("sgp30: param is invalid.\n");
            }
            else
            {
                uart_print("sgp30: unknown status code.\n");
            }
            uart_flush();
        }
        delay_ms(100);
    }
}
