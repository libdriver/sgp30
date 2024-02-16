[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver SGP30

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/sgp30/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

SGP30은 공기 청정기, 수요 조절형 환기 및 IoT 애플리케이션에 쉽게 통합할 수 있도록 설계된 디지털 다중 픽셀 가스 센서입니다. Sensirion의 CMOSens 기술은 디지털 I2C 인터페이스, 온도 제어 마이크로 핫플레이트 및 2개의 사전 처리된 실내 공기 품질 신호를 특징으로 하는 단일 칩에 완전한 센서 시스템을 제공합니다. 하나의 칩에 여러 개의 감지 요소가 있는 최초의 금속 산화물 가스 센서인 SGP30은 공기 품질에 대한 보다 자세한 정보를 제공합니다. 감지 요소는 실제 애플리케이션에 존재하는 오염 가스에 대한 비할 데 없는 견고성을 갖추고 있어 고유한 장기 안정성을 가능하게 합니다. 그리고 낮은 드리프트. 초소형 2.45 x 2.45 x 0.9 mm3 DFN 패키지는 제한된 공간에서 애플리케이션을 가능하게 합니다. Sensirion의 최첨단 생산 공정은 높은 재현성과 신뢰성을 보장합니다. 테이프 및 릴 포장은 표준 SMD 조립 공정에 대한 적합성과 함께 SGP30을 대량 응용 분야에 적합하게 만듭니다.

LibDriver SGP30은 LibDriver에서 출시한 SGP30의 전체 기능 드라이버입니다. TVOC, CO2 eq 판독, 온도 및 습도 보정 및 기타 기능을 제공합니다. LibDriver는 MISRA를 준수합니다.

### 콘텐츠

  - [설명](#설명)
  - [설치](#설치)
  - [사용](#사용)
    - [example basic](#example-basic)
    - [example advance](#example-advance)
  - [문서](#문서)
  - [기고](#기고)
  - [저작권](#저작권)
  - [문의하기](#문의하기)

### 설명

/src 디렉토리에는 LibDriver SGP30의 소스 파일이 포함되어 있습니다.

/interface 디렉토리에는 LibDriver SGP30용 플랫폼 독립적인 IIC버스 템플릿이 포함되어 있습니다.

/test 디렉토리에는 LibDriver SGP30드라이버 테스트 프로그램이 포함되어 있어 칩의 필요한 기능을 간단히 테스트할 수 있습니다.

/example 디렉토리에는 LibDriver SGP30프로그래밍 예제가 포함되어 있습니다.

/doc 디렉토리에는 LibDriver SGP30오프라인 문서가 포함되어 있습니다.

/datasheet 디렉토리에는 SGP30데이터시트가 있습니다.

/project 디렉토리에는 일반적으로 사용되는 Linux 및 마이크로컨트롤러 개발 보드의 프로젝트 샘플이 포함되어 있습니다. 모든 프로젝트는 디버깅 방법으로 셸 스크립트를 사용하며, 자세한 내용은 각 프로젝트의 README.md를 참조하십시오.

/misra 에는 LibDriver misra 코드 검색 결과가 포함됩니다.

### 설치

/interface 디렉토리에서 플랫폼 독립적인IIC 버스 템플릿을 참조하여 지정된 플랫폼에 대한 IIC버스 드라이버를 완성하십시오.

/src 디렉터리, 플랫폼용 인터페이스 드라이버 및 자체 드라이버를 프로젝트에 추가합니다. 기본 예제 드라이버를 사용하려면 /example 디렉터리를 프로젝트에 추가합니다.

### 사용

/example 디렉터리의 예제를 참조하여 자신만의 드라이버를 완성할 수 있습니다. 기본 프로그래밍 예제를 사용하려는 경우 사용 방법은 다음과 같습니다.

#### example basic

```C
#include "driver_sgp30_basic.h"

uint8_t res;
uint8_t i;
uint16_t co2_eq_ppm;
uint16_t tvoc_ppb;

res = sgp30_basic_init();
if (res != 0)
{
    return 1;
}

...

for (i = 0; i < 3; i++)
{
    sgp30_interface_delay_ms(1000);
    res = sgp30_basic_read((uint16_t *)&co2_eq_ppm, (uint16_t *)&tvoc_ppb);
    if (res != 0)
    {
        (void)sgp30_basic_deinit();

        return 1;
    }
    sgp30_interface_debug_print("sgp30: co2 eq is %d ppm.\n", co2_eq_ppm);
    sgp30_interface_debug_print("sgp30: tvoc is %d ppb.\n", tvoc_ppb);
    
    ...
    
}

...

(void)sgp30_basic_deinit();

return 0;
```

#### example advance

```c
#include "driver_sgp30_advance.h"

uint8_t res;
uint8_t i;
uint16_t co2_eq_ppm;
uint16_t tvoc_ppb;
uint16_t id[3];
uint8_t product_type;
uint8_t product_version;

res = sgp30_advance_init();
if (res != 0)
{
    return 1;
}

...

res = sgp30_advance_get_serial_id((uint16_t *)id);
if (res != 0)
{
    (void)sgp30_advance_deinit();

    return 1;
}
sgp30_interface_debug_print("sgp30: serial id 0x%04X 0x%04X 0x%04X.\n", (uint16_t)(id[0]), (uint16_t)(id[1]), (uint16_t)(id[2]))

...

res = sgp30_advance_get_feature((uint8_t *)&product_type, (uint8_t *)&product_version);
if (res != 0)
{
    (void)sgp30_advance_deinit();

    return 1;
}
sgp30_interface_debug_print("sgp30: product type is 0x%02X.\n", product_type);
sgp30_interface_debug_print("sgp30: product version is 0x%02X.\n", product_version);

...

res = sgp30_advance_set_iaq_baseline(0x01, 0x02);
if (res != 0)
{
    (void)sgp30_advance_deinit();

    return 1;
}

...

res = sgp30_advance_set_absolute_humidity(25.6, 30.2f);
if (res != 0)
{
    (void)sgp30_advance_deinit();

    return 1;
}

...

for (i = 0; i < 3; i++)
{
    sgp30_interface_delay_ms(1000);
    res = sgp30_advance_read((uint16_t *)&co2_eq_ppm, (uint16_t *)&tvoc_ppb);
    if (res != 0)
    {
        (void)sgp30_advance_deinit();

        return 1;
    }
    sgp30_interface_debug_print("sgp30: co2 eq is %d ppm.\n", co2_eq_ppm);
    sgp30_interface_debug_print("sgp30: tvoc is %d ppb.\n", tvoc_ppb);
    
    ...
    
}

...

(void)sgp30_advance_deinit();

return 0;
```

### 문서

온라인 문서: [https://www.libdriver.com/docs/sgp30/index.html](https://www.libdriver.com/docs/sgp30/index.html).

오프라인 문서: /doc/html/index.html.

### 기고

CONTRIBUTING.md 를 참조하십시오.

### 저작권

저작권 (c) 2015 - 지금 LibDriver 판권 소유

MIT 라이선스(MIT)

이 소프트웨어 및 관련 문서 파일("소프트웨어")의 사본을 얻은 모든 사람은 이에 따라 무제한 사용, 복제, 수정, 통합, 출판, 배포, 2차 라이선스를 포함하여 소프트웨어를 처분할 수 있는 권리가 부여됩니다. 소프트웨어의 사본에 대한 라이선스 및/또는 판매, 그리고 소프트웨어가 위와 같이 배포된 사람의 권리에 대한 2차 라이선스는 다음 조건에 따릅니다.

위의 저작권 표시 및 이 허가 표시는 이 소프트웨어의 모든 사본 또는 내용에 포함됩니다.

이 소프트웨어는 상품성, 특정 목적에의 적합성 및 비침해에 대한 보증을 포함하되 이에 국한되지 않는 어떠한 종류의 명시적 또는 묵시적 보증 없이 "있는 그대로" 제공됩니다. 어떤 경우에도 저자 또는 저작권 소유자는 계약, 불법 행위 또는 기타 방식에 관계없이 소프트웨어 및 기타 소프트웨어 사용으로 인해 발생하거나 이와 관련하여 발생하는 청구, 손해 또는 기타 책임에 대해 책임을 지지 않습니다.

### 문의하기

연락주세요lishifenging@outlook.com.