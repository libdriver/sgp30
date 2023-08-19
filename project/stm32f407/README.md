### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

IIC Pin: SCL/SDA PB8/PB9.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. SGP30

#### 3.1 Command Instruction

1. Show sgp30 chip and driver information.

   ```shell
   sgp30 (-i | --information)
   ```

2. Show sgp30 help.

   ```shell
   sgp30 (-h | --help)
   ```

3. Show sgp30 pin connections of the current board.

   ```shell
   sgp30 (-p | --port)
   ```

4. Run sgp30 register test.

   ```shell
   sgp30 (-t reg | --test=reg)
   ```

5. Run sgp30 read test, num means test times.

   ```shell
   sgp30 (-t read | --test=read) [--times=<num>]
   ```

6. Run sgp30 read function, num means read times.

   ```shell
   sgp30 (-e read | --example=read) [--times=<num>]
   ```

7. Run sgp30 advance read function, num means read times, ppb means current tvoc, ppm means current co2eq, temp means current temperature and rh means current relative humidity.

   ```shell
   sgp30 (-e advance-read | --example=advance-read) [--times=<num>] [--baseline-tvoc=<ppb>] [--baseline-co2eq=<ppm>] [--humidity-temperature=<temp>] [--humidity-rh=<rh>]
   ```

8. Get sgp30 id and product information.

   ```shell
   sgp30 (-e info | --example=info)
   ```

#### 3.2 Command Example

```shell
sgp30 -i

sgp30: chip is Sensirion SGP30.
sgp30: manufacturer is Sensirion.
sgp30: interface is IIC.
sgp30: driver version is 2.0.
sgp30: min supply voltage is 1.6V.
sgp30: max supply voltage is 2.0V.
sgp30: max current is 48.80mA.
sgp30: max temperature is 85.0C.
sgp30: min temperature is -40.0C.
```

```shell
sgp30 -p

sgp30: SCL connected to GPIOB PIN8.
sgp30: SDA connected to GPIOB PIN9.
```

```shell
sgp30 -t reg

sgp30: chip is Sensirion SGP30.
sgp30: manufacturer is Sensirion.
sgp30: interface is IIC.
sgp30: driver version is 2.0.
sgp30: min supply voltage is 1.6V.
sgp30: max supply voltage is 2.0V.
sgp30: max current is 48.80mA.
sgp30: max temperature is 85.0C.
sgp30: min temperature is -40.0C.
sgp30: start register test.
sgp30: sgp30_soft_reset test.
sgp30: check soft reset ok.
sgp30: sgp30_get_serial_id test.
sgp30: check get serial id ok.
sgp30: serial id 0x0000 0x014C 0x3E62.
sgp30: sgp30_get_feature_set test.
sgp30: check get feature set ok.
sgp30: product type is 0x00.
sgp30: product version is 0x22.
sgp30: sgp30_set_tvoc_baseline/sgp30_get_tvoc_inceptive_baseline test.
sgp30: set tvoc baseline 0xF6F7.
sgp30: get tvoc inceptive baseline 0x927B.
sgp30: sgp30_set_iaq_baseline/sgp30_get_iaq_baseline test.
sgp30: set tvoc baseline 0x934B.
sgp30: set co2 eq baseline 0xDADD.
sgp30: check iaq baseline ok.
sgp30: sgp30_set_absolute_humidity test.
sgp30: set temp 11.6C.
sgp30: set rh 25.83%.
sgp30: set register value 0xFF00.
sgp30: check absolute humidity ok.
sgp30: finish register test.
```

```shell
sgp30 -t read --times=3

sgp30: chip is Sensirion SGP30.
sgp30: manufacturer is Sensirion.
sgp30: interface is IIC.
sgp30: driver version is 2.0.
sgp30: min supply voltage is 1.6V.
sgp30: max supply voltage is 2.0V.
sgp30: max current is 48.80mA.
sgp30: max temperature is 85.0C.
sgp30: min temperature is -40.0C.
sgp30: start read test.
sgp30: raw read.
sgp30: raw co2 eq is 12876.
sgp30: raw tvoc is 17579.
sgp30: raw co2 eq is 13382.
sgp30: raw tvoc is 18211.
sgp30: raw co2 eq is 13642.
sgp30: raw tvoc is 18669.
sgp30: normal read.
sgp30: co2 eq is 400 ppm.
sgp30: tvoc is 0 ppb.
sgp30: co2 eq is 400 ppm.
sgp30: tvoc is 0 ppb.
sgp30: co2 eq is 400 ppm.
sgp30: tvoc is 0 ppb.
sgp30: finish read test.
```

```shell
sgp30 -e read --times=3

sgp30: 1/3.
sgp30: co2 eq is 400 ppm.
sgp30: tvoc is 0 ppb.
sgp30: 2/3.
sgp30: co2 eq is 400 ppm.
sgp30: tvoc is 0 ppb.
sgp30: 3/3.
sgp30: co2 eq is 400 ppm.
sgp30: tvoc is 0 ppb.
```

```shell
sgp30 -e advance-read --times=3 --baseline-tvoc=0 --baseline-co2eq=1 --humidity-temperature=25.0 --humidity-rh=50.0

sgp30: 1/3.
sgp30: co2 eq is 400 ppm.
sgp30: tvoc is 0 ppb.
sgp30: 2/3.
sgp30: co2 eq is 400 ppm.
sgp30: tvoc is 0 ppb.
sgp30: 3/3.
sgp30: co2 eq is 400 ppm.
sgp30: tvoc is 0 ppb.
```

```shell
sgp30 -e info

sgp30: serial id 0x0000 0x014C 0x3E62.
sgp30: product type is 0x00.
sgp30: product version is 0x22.
```

```shell
sgp30 -h

Usage:
  sgp30 (-i | --information)
  sgp30 (-h | --help)
  sgp30 (-p | --port)
  sgp30 (-t reg | --test=reg)
  sgp30 (-t read | --test=read) [--times=<num>]
  sgp30 (-e read | --example=read) [--times=<num>]
  sgp30 (-e advance-read | --example=advance-read) [--times=<num>] [--baseline-tvoc=<ppb>] [--baseline-co2eq=<ppm>]
        [--humidity-temperature=<temp>] [--humidity-rh=<rh>]
  sgp30 (-e info | --example=info)

Options:
      --baseline-co2eq=<ppm>              Set the co2eq baseline.([default: 0])
      --baseline-tvoc=<ppb>               Set the tvoc baseline.([default: 0])
  -e <read | advance-read | info>, --example=<read | advance-read | info>
                                          Run the driver example.
  -h, --help                              Show the help.
      --humidity-rh=<rh>                  Set the humidity rh.([default: 50.0f])
      --humidity-temperature=<temp>       Set the humidity temperature.([default: 25.0f])
  -i, --information                       Show the chip information.
  -p, --port                              Display the pin connections of the current board.
  -t <reg | read>, --test=<reg | read>    Run the driver test.
      --times=<num>                       Set the running times.([default: 3])
```

