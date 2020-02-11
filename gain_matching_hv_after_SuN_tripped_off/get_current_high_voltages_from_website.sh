#!/usr/bin/env bash

# Reads the voltages from the MPOD power supply, which are diplayed at
# https://daqwww.nscl.msu.edu/sundaq/
# and puts them into current_high_voltages.txt
# There is an "off by one" difference between the snmpget commands
# and the channels displayed on the website.
# For example, if channel were 0 or 100 in the snmpget command,
# the following error is given:
# u0 No Such Instance currently exists at this OID
# u100 No Such Instance currently exists at this OID
# Therefore, it is (( ${channel} + 1 ))

echo > current_high_voltages.txt

ip_address="35.9.56.79"

# TOP PMTs
for (( channel=0; channel<12; channel++ )); do
    voltage=$(snmpget -OqvU -v 2c -m +WIENER-CRATE-MIB -c public ${ip_address} outputVoltage.$(( ${channel} + 1 )))
    echo u${channel} ${voltage}
    echo u${channel} ${voltage} >> current_high_voltages.txt
done

# BOTTOM PMTs
for (( channel=100; channel<112; channel++ )); do
    voltage=$(snmpget -OqvU -v 2c -m +WIENER-CRATE-MIB -c public ${ip_address} outputVoltage.$(( ${channel} + 1 )))
    echo u${channel} ${voltage}
    echo u${channel} ${voltage} >> current_high_voltages.txt
done
