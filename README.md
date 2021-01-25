# micro:bit v2 bootloader

This repository contains the code required to compile the bootloader for micro:bit V2. This description is taken from the Nordic infocenter:


```

Bootloader

The bootloader module is responsible for:

    booting into an application,
    activating new firmware,
    optionally, entering DFU mode where DFU transports are activated and new firmware can be delivered,
    feeding the watchdog timer.

Each bootloader example provided in this SDK contains one DFU transport.

```

The source of the bootloader can be found in `./bootloader`, a simple SDK to interact with the micro:bit's peripherals can be found in `./microbit_sdk`, and modifications to the nRF5SDK can be found in `./nRF5SDK_mods`.

The mods are required to add a custom init packet, skip write protection, and display DFU progress on the LEDs.

# Building this repo

There should be these folders here: <br />

nRF5SDK - Nordic SDK v16 when downloaded <br />
bootloader - from examples/dfu/secure_bootloader <br />
testapp - from examples/ble_peripheral/ble_app_buttonless_dfu <br />


### getting started <br />
Clone this repo and download nRF5SDK as below. <br />

@martinwork has been using and updating the SES (Segger Embedded Studio) projects <br />
SES latest version is 4.30a but SDK 16 release notes mention v4.18. <br />
SES uses .emProject files to store project information. These are XML files listing information such as source files, directories, and compiler flags. <br />
https://infocenter.nordicsemi.com/topic/struct_nrf5gs/struct/nrf5gs_sw_dev.html <br />

@microbit-sam has been using and updating the Makefile based projects. <br />


### nRF5SDK <br />
Download the SDK folder zip (tested 16.0.0) from Nordic, or from the tagged release on this [repository](https://github.com/microbit-foundation/v2-bootloader/releases/tag/sdk16)<br />
https://www.nordicsemi.com/Software-and-tools/Software/nRF5-SDK/Download <br />
Unzip and rename to nRF5SDK <br />
Download and build micro-ecc as described here: <br />
https://infocenter.nordicsemi.com/topic/sdk_nrf5_v16.0.0/lib_crypto_backend_micro_ecc.html <br />
I installed the ARM GCC toolchain at the location specified in
nRF5SDK⁩/components⁩/toolchain⁩gcc/Makefile.posix <br />
and built nRF5SDK⁩/external⁩/micro-ecc/nrf52hf_armgcc/armgcc/Makefile <br />


### Testing DFU <br />
With SES, load the emProject in bootloader/microbit_debug/ses. <br />
Choose Build/Build and Run - this will download SoftDevice and the bootloader. <br />
Load the emProject in testapp/microbit/ses. <br />
Select the Debug configuration and choose Build/Build Solution. <br />

Alternatively, build the test app using make 
`make -C ./testapp/microbit/armgcc`

Using "scripts/package_testapp_debug" [see "scripts/testapp_package" below], generate "testapp_debug.zip" . <br />
Copy the zip to an Android device or to e.g. Google Drive. <br />

With Android nRF Connect, download testapp.zip: <br />
    Refresh the scan to find DfuTarg and connect to it. <br />
    Tap DFU at top right, choose "Distribution packet (ZIP)" and locate the zip. <br />

Return to SCANNER, scan to find "Nordic_buttonless" and connect to it. <br />
    Tap Secure DFU Service. <br />
    Tap the up/down arrows to enable notifications. <br />
    Tap the up arrow and tap SEND in the alert. <br />

Return to SCANNER, scan to find "DfuTarg". <br />
    Reset microbit and check "Nordic_buttonless" reappears. <br />

With SES, choose Debug/Go to debug the app. <br />
Switch to the microbit_bootloader_debug project and choose Debug/Go to debug the bootloader.


### Making a complete hex <br />
See "scripts/merge_testapp and merge_testapp_debug" below. <br />
The bootloader won't launch the application without a settings page. <br />
https://infocenter.nordicsemi.com/topic/sdk_nrf5_v16.0.0/lib_bootloader.html. <br />
https://infocenter.nordicsemi.com/topic/ug_nrfutil/UG/nrfutil/nrfutil_settings_generate_display.html <br />


### Bonding <br />
The bootloader can require bonds by setting NRF_DFU_BLE_REQUIRES_BONDS and NRF_SDH_BLE_SERVICE_CHANGED. <br />
In this case, if the bootloader does not find valid peer data (see nrf_dfu_settings_peer_data_is_valid), it will simply reset. <br />
The peer data can only be provided by a valid application which has done the bonding (with NRF_DFU_BLE_BUTTONLESS_SUPPORTS_BONDS and NRF_SDH_BLE_SERVICE_CHANGED set). <br />


### testapp placement macros <br />
These are modified to add calculations. <br />
FLASH_SIZE=(0x72000-FLASH_START), so it will work with both release and debug bootloaders. <br />
The bootloaders have FLASH_START 0x72000 (debug) and 0x78000 (release). <br />
FLASH_START=0x27000 is correct for S140 7.0.1. <br />
RAM_START=0x20002270 is the value calculated by sd_ble_enable. <br />
https://infocenter.nordicsemi.com/topic/com.nordic.infocenter.s140.api.v7.0.1/group___b_l_e___c_o_m_m_o_n___f_u_n_c_t_i_o_n_s.html <br />
See Memory layout here: <br />
https://infocenter.nordicsemi.com/topic/sdk_nrf5_v16.0.0/lib_bootloader.html <br />
The application start address is given as 0x0002 6000, which seems to be wrong. <br />
 
7F000 bootloader_settings_page <br />
7E000 mbr_params_page / bootloader settings backup <br />
78000 Bootloader (72000 for debug bootloader) <br />
27000 Application <br />
01000 SoftDevice <br />
00000 MBR <br />


### scripts/make_dfu_keys <br />
This is a bash script. Double click it or copy the nrfutil command out of it. <br />
Uses "nrfutil keys" to generate private and public keys in its folder. <br />
The public key needs to be copied to the bootloader source. <br />
The private key is used create a signature for each application hex to be checked by the bootloader. <br />
https://infocenter.nordicsemi.com/topic/sdk_nrf5_v16.0.0/lib_bootloader_dfu_keys.html <br />


### scripts/package_testapp and package_testapp_debug  <br />
This is a bash script. Double click it or copy the nrfutil command out of it. <br />
Uses "nrfutil pkg generate" to make a zip package combining testapp.hex with the private key. <br />
The zip contains a bin file, which is simply the hex converted to bin and a dat file which provides the init packet and contains the signature. <br />
https://infocenter.nordicsemi.com/topic/sdk_nrf5_v16.0.0/lib_bootloader_dfu_validation.html <br />
Looking at the code in nrf_dfu_ver_validation_check makes the documentation clearer! <br />


### scripts/merge_testapp and merge_testapp_debug <br />
This is a bash script. Double click it or copy the nrfutil command out of it. <br />
Uses "nrfutil settings generate" to make a bootloader settings page hex for the application.hex. <br />
Uses "mergehex" to merge the softdevice, application, bootloader and settings page. <br />

## Enabling logging in the bootloader

git patch:

```
diff --git bootloader/main.c bootloader/main.c
index fa4cf3b..71c6dd6 100755
--- bootloader/main.c
+++ bootloader/main.c
@@ -46,6 +46,11 @@
  *
  */
 
+#define NRF_LOG_ENABLED 1
+#define NRF_LOG_STR_FORMATTER_TIMESTAMP_FORMAT_ENABLED 0 
+#define NRF_LOG_DEFAULT_LEVEL 1
+
+
 #include <stdint.h>
 #include "nrf_mbr.h"
 #include "nrf_bootloader.h"
diff --git bootloader/microbit/armgcc/Makefile bootloader/microbit/armgcc/Makefile
index fae7c00..4a6e1d5 100755
--- bootloader/microbit/armgcc/Makefile
+++ bootloader/microbit/armgcc/Makefile
@@ -87,6 +87,13 @@ SRC_FILES += \
   $(SDK_ROOT)/components/libraries/crypto/backend/oberon/oberon_backend_eddsa.c \
   $(SDK_ROOT)/components/libraries/crypto/backend/oberon/oberon_backend_hash.c \
   $(SDK_ROOT)/components/libraries/crypto/backend/oberon/oberon_backend_hmac.c \
+  $(SDK_ROOT)/components/libraries/log/src/nrf_log_backend_flash.c \
+  $(SDK_ROOT)/components/libraries/log/src/nrf_log_backend_rtt.c \
+  $(SDK_ROOT)/components/libraries/log/src/nrf_log_backend_serial.c \
+  $(SDK_ROOT)/components/libraries/log/src/nrf_log_backend_uart.c \
+  $(SDK_ROOT)/components/libraries/log/src/nrf_log_default_backends.c \
+  $(SDK_ROOT)/components/libraries/log/src/nrf_log_frontend.c \
+  $(SDK_ROOT)/components/libraries/memobj/nrf_memobj.c \
 
 # Include folders common to all targets
 INC_FOLDERS += \
@@ -140,6 +147,7 @@ INC_FOLDERS += \
   $(SDK_ROOT)/external/nano-pb \
   $(SDK_ROOT)/components/libraries/queue \
   $(SDK_ROOT)/components/libraries/ringbuf \
+  $(SDK_ROOT)/external/fprintf/ \
 
 # Libraries common to all targets
 LIB_FILES += \
diff --git bootloader/microbit/config/sdk_config.h bootloader/microbit/config/sdk_config.h
index 3519dac..b119319 100755
--- bootloader/microbit/config/sdk_config.h
+++ bootloader/microbit/config/sdk_config.h
@@ -1701,7 +1701,7 @@
 // <e> NRF_LOG_ENABLED - nrf_log - Logger
 //==========================================================
 #ifndef NRF_LOG_ENABLED
-#define NRF_LOG_ENABLED 0
+#define NRF_LOG_ENABLED 1
 #endif
 // <h> Log message pool - Configuration of log message pool
 
```

# License

This repository contains code with different licenses

The modified Nordic SDK and examples are licensed by Nordic Semiconductor ASA. Each of these files contains it's own license.
Any other files were created by the Micro:bit Educational Foundation and are [MIT licensed](LICENSE).
