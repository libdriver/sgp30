[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver SGP30

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/sgp30/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

SGP30は、空気清浄機、デマンド制御換気、およびIoTアプリケーションに簡単に統合できるように設計されたデジタルマルチピクセルガスセンサーです。 SensirionのCMOSensテクノロジーは、デジタルI2Cインターフェース、温度制御されたマイクロホットプレート、および2つの前処理された室内空気質信号を備えた単一チップ上の完全なセンサーシステムを提供します。 SGP30は、1つのチップに複数の検知素子を備えた最初の金属酸化物ガスセンサーとして、空気の質に関するより詳細な情報を提供します。検知素子は、実世界のアプリケーションに存在する汚染ガスに対する比類のない堅牢性を備えており、独自の長期安定性を実現します。と低ドリフト。非常に小さい2.45x2.45 x 0.9 mm3 DFNパッケージにより、限られたスペースでのアプリケーションが可能になります。センシリオンの最先端の製造プロセスは、高い再現性と信頼性を保証します。テープとリールのパッケージングは、標準のSMDアセンブリプロセスへの適合性とともに、SGP30を大量のアプリケーションに適したものにします。

LibDriver SGP30は、LibDriverによって起動されたSGP30の全機能ドライバーであり、TVOC、CO2 eq読み取り、温度および湿度補正、およびその他の機能を提供します。LibDriverはMISRAに準拠しています。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example advance](#example-advance)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver SGP30のソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver SGP30用のプラットフォームに依存しないIICバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver SGP30ドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver SGP30プログラミング例が含まれています。

/ docディレクトリには、LibDriver SGP30オフラインドキュメントが含まれています。

/ datasheetディレクトリには、SGP30データシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないIICバステンプレートを参照して、指定したプラットフォームのIICバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

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

return 0
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

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/sgp30/index.html](https://www.libdriver.com/docs/sgp30/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。