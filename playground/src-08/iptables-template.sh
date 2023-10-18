#!/bin/sh
# IPTables init script for systemd
# Project: Raspberry Pi Router
# by ph0en1x - https://ph0en1x.net

IPT=/sbin/iptables

ETH=eth0
WLAN=wlan0
PPP=ppp0
ETH_EXT=eth1

# Flush active rules, flush custom tables, and set default policy
Flush_Rules () {
	$IPT --flush
	$IPT -t nat --flush
	$IPT -t mangle --flush
	$IPT --delete-chain
	$IPT -t nat --delete-chain
	$IPT -t mangle --delete-chain
	# Set default policies to ACCEPT
	$IPT -P INPUT ACCEPT
	$IPT -P FORWARD ACCEPT
	$IPT -P OUTPUT ACCEPT
}

# Loading rules
Load_Rules () {
        # Flush
        Flush_Rules
        # DEFAULT POLICIES
        $IPT -P INPUT DROP
        $IPT -P FORWARD DROP
        $IPT -P OUTPUT ACCEPT

        # PASS localhost traffic
        $IPT -A INPUT  -i lo -j ACCEPT
        $IPT -A OUTPUT -o lo -j ACCEPT

        # ALLOW traffic from already established connections
        $IPT -A INPUT -m state --state RELATED,ESTABLISHED -j ACCEPT
        # ALLOW local connections to router via SSH
        $IPT -A INPUT -p tcp --dport 22 -i $ETH -j ACCEPT
        $IPT -A INPUT -p tcp --dport 22 -i $WLAN -j ACCEPT
        # DROP all other incoming connections
        $IPT -A INPUT -j DROP

        # ALLOW packets forwarding between internal networks
        $IPT -A FORWARD -i $ETH -o $WLAN -j ACCEPT
        $IPT -A FORWARD -i $WLAN -o $ETH -j ACCEPT
        # ALLOW packets forwarding between local net and internet
        $IPT -A FORWARD -i $PPP -o $ETH -m state --state RELATED,ESTABLISHED -j ACCEPT
        $IPT -A FORWARD -i $ETH -o $PPP -j ACCEPT
        $IPT -A FORWARD -i $PPP -o $WLAN -m state --state RELATED,ESTABLISHED -j ACCEPT
        $IPT -A FORWARD -i $WLAN -o $PPP -j ACCEPT
        
        $IPT -A FORWARD -i $ETH_EXT -o $ETH -m state --state RELATED,ESTABLISHED -j ACCEPT
        $IPT -A FORWARD -i $ETH -o $ETH_EXT -j ACCEPT
        $IPT -A FORWARD -i $ETH_EXT -o $WLAN -m state --state RELATED,ESTABLISHED -j ACCEPT
        $IPT -A FORWARD -i $WLAN -o $ETH_EXT -j ACCEPT

        # DROP all other forwarding traffic
        $IPT -A FORWARD -j DROP 

        # DROP ports scanning traffic
        $IPT -N PORT-SCAN
        $IPT -A PORT-SCAN -p tcp --tcp-flags SYN,ACK,FIN,RST RST -m limit --limit 1/s -j RETURN
        $IPT -A PORT-SCAN -j DROP

        # NAT for external interface
        $IPT -t nat -A POSTROUTING -o $PPP -j MASQUERADE
        $IPT -t nat -A POSTROUTING -o $ETH_EXT -j MASQUERADE
}


case $1 in
	start)
		Load_Rules
		echo "IPTables rules loaded."
		;;
	stop)
		Flush_Rules
		echo "IPTables rules flushed."
		;;
	restart)
		Flush_Rules
		Load_Rules
		echo "IPTables rules reloaded."
		;;
	*)
		echo "Usage: systemctl {start|stop|restart} iptables.service"
		exit 1
esac

exit 0
