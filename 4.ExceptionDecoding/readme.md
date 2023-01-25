# Exception Decoding

The ESP32 Exception decoder can parse the UART input containing a backtrace and decode each frame into the function, file and line so it will be easier to understand what went wrong.

for example:
```
abort() was called at PC 0x400d0c1f on core 0


Backtrace:0x400818fa:0x3ffb9eb00x40085151:0x3ffb9ed0 0x4008a7a6:0x3ffb9ef0 0x400d0c1f:0x3ffb9f60 0x400d0c32:0x3ffb9f90 0x40087f91:0x3ffb9fb0 0x40087fd1:0x3ffb9fd0 0x40088140:0x3ffba000 0x4008822d:0x3ffba030
```

will be decoded into:
```
  #0  0x400818fa:0x3ffb9eb0 in panic_abort at .platformio\packages\framework-espidf\components\esp_system/panic.c:402
  #1  0x40085151:0x3ffb9ed0 in esp_system_abort at .platformio\packages\framework-espidf\components\esp_system/esp_system.c:128
  #2  0x4008a7a6:0x3ffb9ef0 in abort at .platformio\packages\framework-espidf\components\newlib/abort.c:46
  #3  0x400d0c1f:0x3ffb9f60 in crashDump(char const*, int, char const*, char const*) at lib/freertos-debuging/freertos_debug_helper.h:39
  #4  0x400d0c32:0x3ffb9f90 in vFailTimerCallback(void*) at src/main.cpp:24
  #5  0x40087f91:0x3ffb9fb0 in prvProcessExpiredTimer at .platformio\packages\framework-espidf\components\freertos/timers.c:565
  #6  0x40087fd1:0x3ffb9fd0 in prvProcessTimerOrBlockTask at .platformio\packages\framework-espidf\components\freertos/timers.c:634
  #7  0x40088140:0x3ffba000 in prvTimerTask at .platformio\packages\framework-espidf\components\freertos/timers.c:597 (discriminator 1)
  #8  0x4008822d:0x3ffba030 in vPortTaskWrapper at .platformio\packages\framework-espidf\components\freertos\port\xtensa/port.c:131

```

If you want to decode a backtrace without using PlatformIO, you may use the [python script](https://gist.github.com/sarfata/a06d8aaff7697a742f885ef774a7df1d) but you'll need to keep the elf version for that particular stack trace in order for it to work correctly.
