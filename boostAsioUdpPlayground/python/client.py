#!/bin/python
""" Simple client to send some data to a server """
from __future__ import print_function
import socket
import time

SOCKET = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
SERVER_ADDRESS = ('localhost', 4567)

try:
    # Send data
    CURRENT_MESSAGE = 0
    while CURRENT_MESSAGE < 120:
        print('sending "%d"' % CURRENT_MESSAGE)
        SOCKET.sendto(chr(CURRENT_MESSAGE), SERVER_ADDRESS)
        CURRENT_MESSAGE = CURRENT_MESSAGE + 1
        time.sleep(0.01)

finally:
    print('Closing Socket')
    SOCKET.close()
