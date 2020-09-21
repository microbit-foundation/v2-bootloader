# v2-bootloader

There should be these folders here: <br />

nRF5SDK - Nordic SDK when downloaded <br />
nRF5SDK_mods - modified versions of some SDK files
microbit_sdk - for the progress display
bootloader - from examples/dfu/secure_bootloader <br />
testapp - from examples/ble_peripheral/ble_app_buttonless_dfu <br />
scripts - some scrpts for merging the bootloader and test app, etc. 

### getting started <br />
Clone this repo and download nRF5SDK as below. <br />
We are using the armgcc builds<br />
The makefile has a target "bob" which builds the bootloader in object format for copying to codal-microbit/lib/bootloader.o.

### nRF5SDK <br />
Download the SDK folder zip (tested nRF5SDK_17.0.0_9d13099) from <br />
https://www.nordicsemi.com/Software-and-tools/Software/nRF5-SDK/Download <br />
Unzip and rename to nRF5SDK <br />
Download and build micro-ecc as described here: <br />
https://infocenter.nordicsemi.com/topic/sdk_nrf5_v17.0.0/lib_crypto_backend_micro_ecc.html <br />
I installed the ARM GCC toolchain at the location specified in
nRF5SDK⁩/components⁩/toolchain⁩gcc/Makefile.posix <br />
and built nRF5SDK⁩/external⁩/micro-ecc/nrf52hf_armgcc/armgcc/Makefile <br />


### Memory map <br />
The bootloader address is 0x72000. <br />
FLASH_START=0x27000 is correct for S140 7.0.1. <br />
RAM_START=0x20002270 is the value calculated by sd_ble_enable. <br />
https://infocenter.nordicsemi.com/topic/com.nordic.infocenter.s140.api.v7.0.1/group___b_l_e___c_o_m_m_o_n___f_u_n_c_t_i_o_n_s.html <br />
See Memory layout here: <br />
https://infocenter.nordicsemi.com/topic/sdk_nrf5_v17.0.0/lib_bootloader.html <br />
The application start address is given as 0x0002 6000, which seems to be wrong. <br />
 
7F000 bootloader_settings_page <br />
7E000 mbr_params_page / bootloader settings backup <br />
72000 Bootloader <br />
27000 Application <br />
01000 SoftDevice <br />
00000 MBR <br />


### Making a complete hex <br />
See "scripts/merge_testapp and merge_testapp_debug" below. <br />
The bootloader won't launch the application without a settings page. <br />
https://infocenter.nordicsemi.com/topic/sdk_nrf5_v17.0.0/lib_bootloader.html. <br />
https://infocenter.nordicsemi.com/topic/ug_nrfutil/UG/nrfutil/nrfutil_settings_generate_display.html <br />


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

