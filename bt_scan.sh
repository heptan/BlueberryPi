#!/bin/bash
# Scan for bluetooth devices

address="$(hcitool scan | grep Én)"

address=`echo "${address:1:17}"`

rfcomm release 0

rfcomm connect 0 $address 19
