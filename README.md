# boot-test

There should be these folders here:

nRF5SDK - Nordic SDK v16
bootloader - from examples/dfu/secure_bootloader
testapp - from examples/ble_peripheral/ble_app_buttonless_dfu

### getting started
Download nRFSDK as below.
I have been using and updating the SES projects
SES latest version is 4.30a but SDK 16 release notes mention v4.18.
https://infocenter.nordicsemi.com/topic/struct_nrf5gs/struct/nrf5gs_sw_dev.html

### nRFSDK
Download the SDK folder zip (tested 16.0.0) from
https://www.nordicsemi.com/Software-and-tools/Software/nRF5-SDK/Download
Unzip and rename to nRFSDK
Download and build micro-ecc as described here
https://infocenter.nordicsemi.com/topic/sdk_nrf5_v16.0.0/lib_crypto_backend_micro_ecc.html
I installed the ARM GCC toolchain and location specified in
nRF5SDK⁩/components⁩/toolchain⁩gcc/Makefile.posix
and built nRF5SDK⁩/external⁩/micro-ecc/nrf52hf_armgcc/armgcc/Makefile
