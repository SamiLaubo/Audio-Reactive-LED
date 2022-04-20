# import sys
# print(sys.executable)

from __future__ import print_function
from __future__ import division

import platform
import numpy as np
import config

# ESP8266 uses WiFi communication
import socket
_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

def _update_esp8266():
    MAX_PIXELS_PER_PACKET = 126
    # Pixel indices
    n_packets = 300 // MAX_PIXELS_PER_PACKET + 1
    idx = np.array_split(list(range(300)), n_packets)
    for packet_indices in idx:
        m = []
        for i in packet_indices:
            m.append(i >> 8)    # Index of pixel to change - 100-place
            m.append(i & 0xff)  # Index of pixel to change - rest
            m.append(255 * (i < 100))   # Pixel red value
            m.append(255 * (i < 200 and i > 100))   # Pixel green value
            m.append(255 * (i < 300 and i > 200))   # Pixel blue value
        m = bytes(m)
        _sock.sendto(m, (config.UDP_IP, config.UDP_PORT))


# If everything is working, you should see a the led strip split into three red, green, and blue pixel scroll
if __name__ == '__main__':
    print('Starting LED strand test')
    _update_esp8266()
