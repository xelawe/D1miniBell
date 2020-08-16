/*
   MQTT Bell plays MP3 files
   Turns on the onboard LED on for one second, then off for one second, repeatedly.
   This uses delay() to pause between LED toggles.
*/
#include <cy_serdebug.h>
#include <cy_serial.h>

const char *gc_hostname =  "D1miniBell";

#include <Ticker.h>

#include "cy_wifi.h"
#include "cy_ota.h"


Ticker TickerBell;


void setup() {
  cy_serial::start(__FILE__);

  init_dfplayer();

  wifi_init(gc_hostname);
  delay(500);

  init_ota(gv_clientname);

}

