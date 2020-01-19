# boot-test

There should be these folders here:

nRF5SDK - Nordic SDK v16 when downloaded
bootloader - from examples/dfu/secure_bootloader
testapp - from examples/ble_peripheral/ble_app_buttonless_dfu


### getting started
Clone this repo and download nRF5SDK as below.
I have been using and updating the SES projects
SES latest version is 4.30a but SDK 16 release notes mention v4.18.
https://infocenter.nordicsemi.com/topic/struct_nrf5gs/struct/nrf5gs_sw_dev.html


### nRF5SDK
Download the SDK folder zip (tested 16.0.0) from
https://www.nordicsemi.com/Software-and-tools/Software/nRF5-SDK/Download
Unzip and rename to nRF5SDK
Download and build micro-ecc as described here:
https://infocenter.nordicsemi.com/topic/sdk_nrf5_v16.0.0/lib_crypto_backend_micro_ecc.html
I installed the ARM GCC toolchain at the location specified in
nRF5SDK⁩/components⁩/toolchain⁩gcc/Makefile.posix
and built nRF5SDK⁩/external⁩/micro-ecc/nrf52hf_armgcc/armgcc/Makefile


### Testing DFU
With SES, load the emProject in bootloader/microbit_debug/ses.
Choose Build/Build and Run - this will download SoftDevice and the bootloader.
Load the emProject in testapp/microbit/ses.
Select the Debug configuration and choose Build/Build Solution.

Using "scripts/package_testapp_debug" [see "scripts/testapp_package" below],
generate "testapp_debug.zip" .
Copy the zip to an Android device or to e.g. Google Drive

With Android nRF Connect, download testapp.zip:
Refresh the scan to find DfuTarg and connect to it.
    Tap DFU at top right, choose "Distribution packet (ZIP)" and locate the zip.
Return to SCANNER, scan to find "Nordic_buttonless" and connect to it.
    Tap Secure DFU Service
    Tap the up/down arrows to enable notifications
    Tap the up arrow and tap SEND in the alert
Return to SCANNER, scan to find "DfuTarg"

Reset microbit and check "Nordic_buttonless" reappears.
With SES, choose Debug/Go to debug the app.
Switch to the microbit_bootloader_debug project and choose Debug/Go to debug the bootloader.


### Making a complete hex
See "scripts/merge_testapp and merge_testapp_debug" below.
The bootloader won't launch the application without a settings page.
https://infocenter.nordicsemi.com/topic/sdk_nrf5_v16.0.0/lib_bootloader.html
https://infocenter.nordicsemi.com/topic/ug_nrfutil/UG/nrfutil/nrfutil_settings_generate_display.html


### testapp placement macros
These are modified to add calculations.
FLASH_SIZE=(0x72000-FLASH_START), so it will work with both release and debug bootloaders. 
The bootloaders have FLASH_START 0x72000 (debug) and 0x78000 (release).
FLASH_START=0x27000 is correct for S140 7.0.1.
RAM_START=0x20002270 is the value calculated by sd_ble_enable.
https://infocenter.nordicsemi.com/topic/com.nordic.infocenter.s140.api.v7.0.1/group___b_l_e___c_o_m_m_o_n___f_u_n_c_t_i_o_n_s.html
See Memory layout here:
https://infocenter.nordicsemi.com/topic/sdk_nrf5_v16.0.0/lib_bootloader.html
The application start address is given as 0x0002 6000, which seems to be wrong.
 
7F000 bootloader_settings_page
7E000 mbr_params_page
78000 Bootloader (72000 for debug bootloader)
27000 Application
01000 SoftDevice
00000 MBR


### scripts/make_dfu_keys
This is a bash script. Double click it or copy the nrfutil command out of it.
Uses "nrfutil keys" to generate private and public keys in its folder.
The public key needs to be copied to the bootloader source.
The private key is used create a signature for each application hex to be checked by the bootloader.
https://infocenter.nordicsemi.com/topic/sdk_nrf5_v16.0.0/lib_bootloader_dfu_keys.html


### scripts/package_testapp and package_testapp_debug 
This is a bash script. Double click it or copy the nrfutil command out of it.
Uses "nrfutil pkg generate" to make a zip package combining testapp.hex with the private key.
The zip contains a bin file, which is simply the hex converted to bin and a dat file which provides the init packet and contains the signature.
https://infocenter.nordicsemi.com/topic/sdk_nrf5_v16.0.0/lib_bootloader_dfu_validation.html
Looking at the code in nrf_dfu_ver_validation_check makes the documentation clearer!


### scripts/merge_testapp and merge_testapp_debug
This is a bash script. Double click it or copy the nrfutil command out of it.
Uses "nrfutil settings generate" to make a bootloader settings page hex for the application.hex.
Uses "mergehex" to merge the softdevice, application, bootloader and settings page.

