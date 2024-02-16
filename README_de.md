[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver SGP30
[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/sgp30/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Der SGP30 ist ein digitaler Multipixel-Gassensor, der für die einfache Integration in Luftreiniger, bedarfsgesteuerte Lüftung und IoT-Anwendungen entwickelt wurde. Die CMOSens-Technologie von Sensirion bietet ein komplettes Sensorsystem auf einem einzigen Chip mit einer digitalen I2C-Schnittstelle, einer temperaturgesteuerten Mikroheizplatte und zwei vorverarbeiteten Raumluftqualitätssignalen. Als erster Metalloxid-Gassensor mit mehreren Sensorelementen auf einem Chip liefert der SGP30 detailliertere Informationen über die Luftqualität. Das Sensorelement zeichnet sich durch eine unübertroffene Robustheit gegenüber kontaminierenden Gasen aus, die in realen Anwendungen vorkommen, was eine einzigartige Langzeitstabilität ermöglicht und geringe Drift. Das mit 2,45 x 2,45 x 0,9 mm3 sehr kleine DFN-Gehäuse ermöglicht Anwendungen auf engstem Raum. Der hochmoderne Produktionsprozess von Sensirion garantiert eine hohe Reproduzierbarkeit und Zuverlässigkeit. Die Tape-and-Reel-Verpackung sowie die Eignung für Standard-SMD-Bestückungsprozesse prädestinieren den SGP30 für High-Volume-Anwendungen.

LibDriver SGP30 ist ein voll funktionsfähiger Treiber von SGP30, der von LibDriver gestartet wurde. Er bietet TVOC, CO2-Eq-Messung, Temperatur- und Feuchtigkeitskorrektur und andere Funktionen. LibDriver ist MISRA-konform.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
    - [example advance](#example-advance)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver SGP30-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver SGP30 IIC.

/test enthält den Testcode des LibDriver SGP30-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver SGP30-Beispielcode.

/doc enthält das LibDriver SGP30-Offlinedokument.

/Datenblatt enthält SGP30-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige IIC-Schnittstellenvorlage und stellen Sie Ihren Plattform-IIC-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

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

### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/sgp30/index.html](https://www.libdriver.com/docs/sgp30/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.