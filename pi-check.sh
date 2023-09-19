#!/bin/bash

# USAGE: pi-check.sh
# Returns a non-zero exit code if the system is detected to be running Raspberry Pi OS.
# Returns 0 when Raspberry Pi is NOT detected.

# NOTE:
# Thanks to: https://www.cyberciti.biz/faq/linux-command-print-raspberry-pi-os-version-on-raspberry-pi/
# BCM = Broadcom SOC channel
# SOC = System on a chip (a single electronic circuit board that combines various computer components onto a single integrated circuit or chip)
#   Also see this for Bash help:
#   https://devmanual.gentoo.org/tools-reference/bash/index.html

isRaspberryPi=false

filePath="/proc/cpuinfo"
if [ -f "$filePath" ]; then
    grep -qE '(BCM)|(Raspberry Pi)' $filePath;
    result=$?
    if [ $result -eq 0 ]; then
        isRaspberryPi=true
    fi
else
    printf "It does NOT exist!\n"
fi

if [ "$isRaspberryPi" == true ]; then
    exit 1
fi
exit 0
