### 1. chip

#### 1.1 chip info

chip name : Raspberry Pi 4B

iic pin: SCL/SDA GPIO3/GPIO2

### 2. install

#### 2.1 install info

```shell
make
```

### 3. sgp30

#### 3.1 command Instruction

​          sgp30 is a basic command which can test all sgp30 driver function:

​           -i        show sgp30 chip and driver information.

​           -h       show sgp30 help.

​           -p       show sgp30 pin connections of the current board.

​           -t (reg | read <times>)           

​           -t reg        run sgp30 register test.

​           -t read <times>        run spg30 read test. times means test times.

​           -c (read <times> | advance (-read <times>|-read <times> (-baseline <tvoc> <co2eq>|-humidity <temperature> <rh>)) | -info (id | product)))

​           -c read <times>        run spg30 read function. times means read times.

​           -c advance -read <times>        run spg30 advance read function. times means read times.

​           -c advance -read <times> -baseline <tvoc> <co2eq>        run spg30 advance read function with baseline. times means read times. tvoc means current tvoc. co2eq means current co2eq.

​           -c advance -read <times> -humidity <temperature> <rh>        run spg30 advance read function with humidity. times means read times. temperature means current temperature.rh means current relative humidity.

​           -c -info id        get spg30 id information.

​           -c -info product        get spg30 product information.

#### 3.2 command example

```shell
./sgp30 -i

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
./sgp30 -p

sgp30: SCL connected to GPIO3(BCM).
sgp30: SDA connected to GPIO2(BCM).
```

```shell
./sgp30 -t reg

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
./sgp30 -t read 3

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
sgp30: raw co2 eq is 13198.
sgp30: raw tvoc is 18242.
sgp30: raw co2 eq is 13487.
sgp30: raw tvoc is 18442.
sgp30: raw co2 eq is 13614.
sgp30: raw tvoc is 18570.
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
./sgp30 -c read 3

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
./sgp30 -c advance -read 3

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
./sgp30 -c advance -read 3 -baseline 0 1

sgp30: tvoc is 0x0000.
sgp30: co2 eq is 0x0001.
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
./sgp30 -c advance -read 3 -humidity 25.1 30.2

sgp30: temp is 25.10C.
sgp30: rh is 30.20%.
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
./sgp30 -c advance -info id

sgp30: serial id 0x0000 0x014C 0x3E62.
```

```shell
./sgp30 -c advance -info product

sgp30: product type is 0x00.
sgp30: product version is 0x22.
```

```shell
./sgp30 -h

sgp30 -i
	show sgp30 chip and driver information.
sgp30 -h
	show sgp30 help.
sgp30 -p
	show sgp30 pin connections of the current board.
sgp30 -t reg
	run sgp30 register test.
sgp30 -t read <times>
	run sgp30 read test.times means test times.
sgp30 -c read <times>
	run spg30 read function.times means read times.
sgp30 -c advance -read <times>
	run spg30 advance read function.times means read times.
sgp30 -c advance -read <times> -baseline <tvoc> <co2eq>
	run spg30 advance read function with baseline.times means read times.tvoc means current tvoc.co2eq means current co2eq.
sgp30 -c advance -read <times> -humidity <temperature> <rh>
	run spg30 advance read function with humidity.times means read times.temperature means current temperature.rh means current relative humidity.
sgp30 -c -info id
	get spg30 id information.
sgp30 -c -info product
	get spg30 product information.
```

