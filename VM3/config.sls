# Configuration eth1
# RAPPEL: eth0 est Ã  vagrant, ne pas y toucher

## DÃ©sactivation de network-manager
NetworkManager:
  service:
    - dead
    - enable: False

## Suppression de la passerelle par dÃ©faut
ip route del default:
  cmd:
    - run

##Configuration de VM3
eth1:
  network.managed:
    - enabled: True
    - type: eth
    - proto: none
    - ipaddr: 172.16.2.163
    - netmask: 28

eth2:
  network.managed:
    - enabled: True
    - type: eth
    - proto: none
    - enable_ipv4: false
    - ipv6proto: static
    - enable_ipv6: true
    - ipv6_autoconf: no
    - ipv6ipaddr: fc00:1234:4::3
    - ipv6netmask: 64
    - ipv6gateway: fc00:1234:4::36

sudo ip -6 route add fc00:1234:1::/64 via fc00:1234:2::26 dev eth2:
  cmd:
    - run


## Configuration de la route vers LAN2 via VM2
routes:
  network.routes:
    - name: eth1
    - routes:
      - name: LAN2
        ipaddr: 172.16.2.128/28
        gateway: 172.16.2.162
