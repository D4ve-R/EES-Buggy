#!/bin/sh

sudo apt install hostapd -y
sudo systemctl unmask hostapd
sudo systemctl enable hostapd
sudo apt install dnsmasq -y
sudo DEBIAN_FRONTEND=noninteractive apt install -y netfilter-persistent iptables-persistent

# Enable routing to Wifi/Ethernet Connection of the pi

#sudo echo -e 'interface wlan0\n\tstatic ip_address=102.168.4.1/24\n\tnohook wpa_supplicant' >> /etc/dhcpcd.conf
#sudo touch /etc/sysctl.d/routed-ap.conf
#sudo echo -e '# Enable IPv4 routing\nnet.ipv4.ip_forward=1' >> /etc/sysctl.d/routed-ap.conf
#sudo iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
#sudo netfilter-persistent save

sudo mv /etc/dnsmasq.conf /etc/dnsmasq.conf.orig
sudo touch /etc/dnsmasq.conf
sudo echo -e 'interface=wlan0 # Listening interface\ndhcp-range=192.168.4.2,192.168.4.20,255.255.255.0,24h\n# Pool of IP addresses served via DHCP\ndomain=wlan \t# Local wireless DNS domain\naddress=/gw.wlan/192.168.4.1\n# Alias for this router' >> /etc/sysctl.d/routed-ap.conf

sudo rfkill unblock wlan

sudo touch /etc/hostapd/hostapd.conf
sudo echo -e 'country_code=DE\ninterface=wlan0\nssid=RaspianWifi\nhw_mode=g\nchannel=7\nmacaddr_acl=0\nauth_algs=1\nignore_broadcast_ssid=0\nwpa=2\nwpa_passphrase=raspberry\nwpa_key_mgmt=WPA-PSK\nwpa_pairwise=TKIP\nrsn_pairwise=CCMP\n' >> /etc/hostapd/hostapd.conf

sudo systemctl reboot

