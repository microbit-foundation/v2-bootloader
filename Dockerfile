# Simple Dockerfile to run all the scripts on a base Ubuntu image
FROM ubuntu:22.04

# Add the scripts and Pipfiles to the docker image
COPY ci-scripts/install_toolchain.sh /home/ci-scripts/
COPY ci-scripts/run_all.sh /home/ci-scripts/
COPY bootloader /home/bootloader/
COPY microbit_sdk /home/microbit_sdk/
COPY nRF5SDK_mods /home/nRF5SDK_mods/
COPY scripts /home/scripts/

# Execute the scripts
WORKDIR /home/
RUN mkdir /home/artifacts
RUN chmod +x /home/ci-scripts/run_all.sh
RUN /home/ci-scripts/run_all.sh /home/artifacts

