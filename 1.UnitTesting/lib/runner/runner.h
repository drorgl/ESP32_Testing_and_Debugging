/**
 * @file runner.h
 * @author Dror Gluska
 * @brief This snippet is designed to run a function in native, arduino and esp-idf frameworks
 * @version 0.1
 * @date 2022-12-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#ifdef __cplusplus
extern "C" {
    #ifdef ARDUINO

void loop();
void setup();

#elif defined(ESP_PLATFORM)

void app_main();

#else

int main(int argc, char **argv);

#endif

}
#endif

#ifdef ARDUINO

#define MAIN() \
void loop(){ \
    while(1){}
} \
int call_setup();\
void setup(){\
    call_setup();\
}\
int call_setup()

#elif defined(ESP_PLATFORM)

#define MAIN() \
int call_app_main();\
void app_main(){\
    call_app_main();\
}\
int call_app_main()

#else

#define MAIN() \
int call_app_main();\
int main(int argc, char**argv){\
    (void)(argc);\
    (void)(argv);\
    return call_app_main();\
}\
int call_app_main()

#endif