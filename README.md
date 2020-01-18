# boot-test

There should be these folders here:

nRF5SDK - Nordic SDK v16
bootloader - from examples/dfu/secure_bootloader
testapp - from examples/ble_peripheral/ble_app_buttonless_dfu

### getting started
Download nRF5SDK as below.
I have been using and updating the SES projects
SES latest version is 4.30a but SDK 16 release notes mention v4.18.
https://infocenter.nordicsemi.com/topic/struct_nrf5gs/struct/nrf5gs_sw_dev.html

### nRF5SDK
Download the SDK folder zip (tested 16.0.0) from
https://www.nordicsemi.com/Software-and-tools/Software/nRF5-SDK/Download
Unzip and rename to nRF5SDK
Download and build micro-ecc as described here
https://infocenter.nordicsemi.com/topic/sdk_nrf5_v16.0.0/lib_crypto_backend_micro_ecc.html
I installed the ARM GCC toolchain and location specified in
nRF5SDK⁩/components⁩/toolchain⁩gcc/Makefile.posix
and built nRF5SDK⁩/external⁩/micro-ecc/nrf52hf_armgcc/armgcc/Makefile

### scripts/make_dfu_keys
Uses "nrfutil keys" to generate private and public keys in its folder.
The public key needs to be copied to the bootloader source.
The private key is used create a signature for each application hex to be checked by the bootloader.

### testapp placement macros
These are modified to add calculations.
FLASH_SIZE=(0x72000-FLASH_START)
Release bootloader FLASH_START is 0x78000.
Debug bootloader FLASH_START is 0x72000.
FLASH_START=0x27000 is correct for S140 7.0.1.
RAM_START=0x20002270 is the value calculated by sd_ble_enable.
https://infocenter.nordicsemi.com/topic/com.nordic.infocenter.s140.api.v7.0.1/group___b_l_e___c_o_m_m_o_n___f_u_n_c_t_i_o_n_s.html
