# Debugging the ESP32

## esp-prog
You'll need to get the [esp-prog](https://s.click.aliexpress.com/e/_DlMRsU7) and a way to connect it to the [ESP32](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/jtag-debugging/configure-other-jtag.html#configure-hardware), [ESP32-C3](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/api-guides/jtag-debugging/configure-other-jtag.html#configure-hardware), [ESP32-S2](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/api-guides/jtag-debugging/configure-other-jtag.html#configure-hardware), [ESP32-S3](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-guides/jtag-debugging/configure-other-jtag.html).

For ESP32 this [adapter](https://www.tindie.com/products/drorgl/esp32-devkit-jtag-debugging-adapter/) may be used.

## esp-prog on Windows
Setting up esp-prog on Windows requires [Zadig](https://zadig.akeo.ie/), you may find instructions [here](https://community.platformio.org/t/esp32-pio-unified-debugger/4541/20)

# Running OpenOCD Commands
To run OpenOCD commands through GDB when a debug session is active, you can use the `monitor` command, for example:
```
monitor profile 10 testfile
```
This command will 

# Scripting GDB
Its possible to [automate](https://www.pythonsheets.com/appendix/python-gdb.html) [GDB](https://sourceware.org/gdb/onlinedocs/gdb/index.html) commands and process their results, gdb scripting is used for general command automation and [python](https://sourceware.org/gdb/onlinedocs/gdb/Python-API.html#Python-API) can be used for more advanced scripting and processing.

To use a gdbinit file, you'll need to tell gdb to load it by adding the following to `platformio.ini` section
```ini
debug_extra_cmds =
    source .gdbinit
```

## Scripting Examples

(Not necessarily working with ESP32)

* [FreeRTOS-GDB](https://github.com/autolycus/FreeRTOS-GDB)
* [FreeRTOS ARM Thread Debugging with Eclipse and GDB](https://mcuoneclipse.com/2015/10/03/freertos-arm-thread-debugging-with-eclipse-and-gdb/)
* [GEF](https://github.com/hugsy/gef)
* [stack-inspector](https://github.com/sharkdp/stack-inspector)
* [GDB dashboard](https://github.com/cyrus-and/gdb-dashboard)


### FreeRTOS
[Helper script](https://github.com/ErichStyger/McuOnEclipse_PEx/blob/master/Drivers/freeRTOS/gdbBacktraceDebug/.gdbinit-FreeRTOS-helpers) providing support for FreeRTOS-aware debugging.

```
freertos_show_threads
freertos_show_timers
freertos_show_queues
```

### STL Views
STL can be a pain to debug and inspecting elements of stl collections is sometimes cryptic, luckily there's a [script](https://github.com/vmingchen/profile/tree/master/.gdb) for that too.

 std::vector<T> -- via pvector command
* std::list<T> -- via plist or plist_member command
* std::map<T,T> -- via pmap or pmap_member command
* std::multimap<T,T> -- via pmap or pmap_member command
* std::set<T> -- via pset command
* std::multiset<T> -- via pset command
* std::deque<T> -- via pdequeue command
* std::stack<T> -- via pstack command
* std::queue<T> -- via pqueue command
* std::priority_queue<T> -- via ppqueue command
* std::bitset<n> -- via pbitset command
* std::string -- via pstring command
* std::widestring -- via pwstring command

