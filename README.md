

Es geht um eine Uhr, bei der währen der Schlafenszeit, das Display ausgeschlaten wird.

Teile: 


![](Bilder/OLED-Display-modul-SH1106.png)



![](Bilder/RISC-V-XIAO-ESP32C6--1024x350.png)


Um 4:30 Uhr wird die Zeit über einen NTP-Server synchronisiert. in der restlichen Zeit ist Wlan abgeschaltet und der ESP32 geht in den modem_sleep.


Dies ist die zweite Version. Leider musste ich feststellen, dass mein Wlan-Signal an den Ort wo ich die Uhr aufgestellt habe zu schwach war.
Deshald nutze ich jetzt einen XIOA-ESP32C6 von SEED mit einer externen Antenne.