

/*

 todo:
  Calibration points.
  Listening/Sending to RFM69Pi Serial data - On/Off setting to format serial data.
  Sleep mode scheme.
  Setting Alarm levels.
  SoC Information.
  Midnight calibration.
  
   -------------------------------------------------------------------
   EmonESP Serial to Emoncms gateway
   -------------------------------------------------------------------
   Adaptation of Chris Howells OpenEVSE ESP Wifi
   by Trystan Lea, Glyn Hudson, OpenEnergyMonitor
   All adaptation GNU General Public License as below.

   Changed for emonDC ADC sampling by Daniel Bates.
   -------------------------------------------------------------------

   This file is part of OpenEnergyMonitor.org project.
   EmonESP is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.
   EmonESP is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with EmonESP; see the file COPYING.  If not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include "emonesp.h"
#include "config.h"
#include "wifi.h"
#include "web_server.h"
#include "ota.h"
#include "input.h"
#include "emoncms.h"
#include "mqtt.h"

#include "emondc.h" // emonDC code additions.


// -------------------------------------------------------------------
// SETUP
// -------------------------------------------------------------------
void setup() {
  Serial.begin(115200);

  main_emondc_interval = 10; // emondc post interval in seconds
  currentfirmware = "v2.3.1_emonDCmod";

  DEBUG.println();
  DEBUG.print("EmonESP ");
  DEBUG.println(ESP.getChipId());
  DEBUG.println("Firmware: " + currentfirmware);

  // Read saved settings from the config
  config_load_settings();

  // Initialise the WiFi
  wifi_setup();

  // Bring up the web server
  web_server_setup();

  // Start the OTA update systems
  // ota_setup();

  // emonDC adc sampling setup.
  emondcsetup();

  DEBUG.println("Server started");


} // end setup

// -------------------------------------------------------------------
// LOOP
// -------------------------------------------------------------------
void loop()
{
  // ota_loop();
  web_server_loop();
  wifi_loop();

  emondc_loop();

  String input = "";
  boolean gotInput = input_get(input);

  if (wifi_mode == WIFI_MODE_STA || wifi_mode == WIFI_MODE_AP_AND_STA)
  {
    if (emoncms_apikey != 0 && gotInput) {
      emoncms_publish(input);
    }
    if (mqtt_server != 0)
    {
      mqtt_loop();
      if (gotInput) {
        mqtt_publish(input);
      }
    }
  }


}
