#!/bin/bash
# Waiting for interfaces assigning IP, than restarting isc-dhcp-server.
# https://ph0en1x.net

declare -A INTERFACES
INTERFACES=(
  [eth0]="192.168.1.1"
  [wlan0]="192.168.2.1"
)

for i in "${!INTERFACES[@]}"
do
  # wait until interface accepts IP
  for ((n=0; n < 60; n++))
  do
    has_ip=`ip a show "$i" | grep "inet ${INTERFACES[$i]}"`
    if [ ! -z "$has_ip" ]; then
      echo "ISC_Watchdog: interface $i ready."
      break
    fi
    sleep 1
  done
done
sleep 15
echo "ISC_Watchdog: restarting isc-dhcp-server..."
systemctl restart isc-dhcp-server
exit 0
