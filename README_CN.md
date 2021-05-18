<div align=center>
<img src="/doc/image/logo.png"/>
</div>

## LibDriver SGP30

[English](/README.md) | [ 简体中文](/README_CH.md)

SGP30是一个数字化多像素气体传感器平台，专为轻松集成到空气净化器、按需新风控制和物联网应用中而设计。SGP30 是一个数字化多像素气体传感器平台，专为轻松集成到空气净化器、按需新风控制和物联网应用中而设计。SGP30 设计用于智能家居、家电和物联网应用，它提供两个经过校准的空气质量信号和湿度补偿功能。Sensirion的CMOSens 技术在单个芯片上提供完整的传感器系统，该系统采用一个数字化 I2C 接口、一个温度控制的微加热器和两个经过预处理的室内空气质量信号。作为首款在单个芯片上采用多个传感元件的金属氧化物气体传感器，SGP30 可提供有关空气质量的更详尽指数。针对现实生活环境中的污染气体，该传感器具有无与伦比的可靠性，可以实现独一无二的长期稳定性和低漂移。 2.45 x 2.45 x 0.9 mm3 DFN 封装支持对空间有要求的应用，Sensirion 的先进生产工艺保证了高度的可再现性和可靠性，胶带和卷轴封装加上标准 SMD 装配工艺使得 SGP30 十分适合大规模生产的应用。

LibDriver SGP30是LibDriver推出的SGP30的全功能驱动，该驱动提供TVOC、CO2 eq读取、温湿度矫正等功能。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example advance](#example-advance)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver SGP30的源文件。

/interface目录包含了LibDriver SGP30与平台无关的IIC总线模板。

/test目录包含了LibDriver SGP30驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver SGP30编程范例。

/doc目录包含了LibDriver SGP30离线文档。

/datasheet目录包含了SGP30数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

### 安装

参考/interface目录下与平台无关的IIC总线模板，完成指定平台的IIC总线驱动。

将/src目录，/interface目录和/example目录加入工程。

### 使用

#### example basic

```C
uint8_t res;
uint8_t i;
uint16_t co2_eq_ppm;
uint16_t tvoc_ppb;

res = sgp30_basic_init();
if (res)
{
    return 1;
}

...

for (i = 0; i < 3; i++)
{
    sgp30_interface_delay_ms(1000);
    res = sgp30_basic_read((uint16_t *)&co2_eq_ppm, (uint16_t *)&tvoc_ppb);
    if (res)
    {
        sgp30_basic_deinit();

        return 1;
    }
    sgp30_interface_debug_print("sgp30: co2 eq is %d ppm.\n", co2_eq_ppm);
    sgp30_interface_debug_print("sgp30: tvoc is %d ppb.\n", tvoc_ppb);
    
    ...
    
}

...

sgp30_basic_deinit();

return 0;
```

#### example advance

```C
uint8_t res;
uint8_t i;
uint16_t co2_eq_ppm;
uint16_t tvoc_ppb;
uint16_t id[3];
uint8_t product_type;
uint8_t product_version;

res = sgp30_advance_init();
if (res)
{
    return 1;
}

...

res = sgp30_advance_get_serial_id((uint16_t *)id);
if (res)
{
    sgp30_advance_deinit();

    return 1;
}
sgp30_interface_debug_print("sgp30: serial id 0x%04X 0x%04X 0x%04X.\n", (uint16_t)(id[0]), (uint16_t)(id[1]), (uint16_t)(id[2]))

...

res = sgp30_advance_get_feature((uint8_t *)&product_type, (uint8_t *)&product_version);
if (res)
{
    sgp30_advance_deinit();

    return 1;
}
sgp30_interface_debug_print("sgp30: product type is 0x%02X.\n", product_type);
sgp30_interface_debug_print("sgp30: product version is 0x%02X.\n", product_version);

...

res = sgp30_advance_set_iaq_baseline(0x01, 0x02);
if (res)
{
    sgp30_advance_deinit();

    return 1;
}

...

res = sgp30_advance_set_absolute_humidity(25.6, 30.2f);
if (res)
{
    sgp30_advance_deinit();

    return 1;
}

...

for (i = 0; i < 3; i++)
{
    sgp30_interface_delay_ms(1000);
    res = sgp30_advance_read((uint16_t *)&co2_eq_ppm, (uint16_t *)&tvoc_ppb);
    if (res)
    {
        sgp30_advance_deinit();

        return 1;
    }
    sgp30_interface_debug_print("sgp30: co2 eq is %d ppm.\n", co2_eq_ppm);
    sgp30_interface_debug_print("sgp30: tvoc is %d ppb.\n", tvoc_ppb);
    
    ...
    
}

...

sgp30_advance_deinit();

return 0;
```

### 文档

在线文档: https://www.libdriver.com/docs/sgp30/index.html

离线文档: /doc/html/index.html

### 贡献

请联系lishifenging@outlook.com

### 版权

版权(C) LibDriver 2015-2021 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com