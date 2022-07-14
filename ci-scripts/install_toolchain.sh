#!/bin/sh
alias pretty_echo='{ set +x; } 2> /dev/null; f(){ echo "#\n#\n# $1\n#\n#"; set -x; }; f'

pretty_echo "Ensure Ubuntu has basic tools..."
apt-get update -qq
# software-properties-common needed to easily add PPAs
apt-get install -y software-properties-common
apt-get install -y build-essential
apt-get install -y git
apt-get install -y curl
apt-get install -y unzip

pretty_echo "Installing GCC ARM compiler version .."
apt-get install -y gcc-arm-none-eabi

pretty_echo "Clean up a bit to reduce image size"
apt-get clean
rm -rf /var/lib/apt/lists/*

pretty_echo "Done installing tool chain"

# Fetch SDK from GH release
pretty_echo "Fetch nRFSDK v16..."
url="https://github.com/microbit-foundation/v2-bootloader/releases/download/sdk16/nRF5SDK.zip"
curl -sSL "$url" > nrf.zip

# Unzip
unzip -q nrf.zip
pretty_echo "Done fetching nRFSDK v16..."

# Remove Makefile.posix
rm nRF5SDK/components/toolchain/gcc/Makefile.posix
printf 'GNU_INSTALL_ROOT ?= \r\nGNU_VERSION ?= 7.3.1\r\nGNU_PREFIX ?= arm-none-eabi\r\n' > nRF5SDK/components/toolchain/gcc/Makefile.posix

# Download micro-ecc
git clone https://github.com/kmackay/micro-ecc nRF5SDK/external/micro-ecc/micro-ecc

# Build micro-ecc
make -C nRF5SDK/external/micro-ecc/nrf52hf_armgcc/armgcc

# Build bootloader
make -C bootloader/microbit/armgcc bob

cp bootloader/microbit/armgcc/bootloader_s113.o artifacts/bootloader.o
