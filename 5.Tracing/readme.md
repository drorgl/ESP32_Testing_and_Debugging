# Application Level Tracing - Logging to Host (app_trace_to_host)

Example from [app_trace_to_host](https://github.com/espressif/esp-idf/tree/master/examples/system/app_trace_to_host).

# How to run

1. Connect JTAG interface to ESP32 board

2. Start Tracing
- Pause execution
```bash
mon esp32 apptrace start file://trace.log 1 -1
```
- Resume execution
```bash
mon esp32 apptrace stop
```

3. Decode and print out retrieved log file by executing:

```bash
pio run -t decode-logtrace -a trace.log
```

This should provide a similar output:

```
Parse trace file 'trace.log'...
Unprocessed 7 bytes of log record args!
Parsing completed.
====================================================================
I (59369) example: Sample:1, Value:3717
I (59369) example: Sample:2, Value:3647
I (59369) example: Sample:3, Value:3575
I (59369) example: Sample:4, Value:3491
...
```
