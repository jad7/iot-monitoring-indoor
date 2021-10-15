//#pragma SPARK_NO_PREPROCESSOR
//#include "Particle.h"
#include <stdint.h>
//#define DEBUG 1
#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif
/*
#define D0 (16)
#define D1 (5)
#define D2 (4)
#define D3 (0)
#define D4 (2)
#define D5 (14)
#define D6 (12)
#define D7 (13)
#define D8 (15)
#define TX (1)
*/
#ifndef METEO_H
#define METEO_H
struct MeteoData {
  float tempr;
  int humidity;
  uint16_t co2;
};
#endif