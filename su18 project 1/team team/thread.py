import socket
import sys
import pycom
import binascii
from _thread import *
import time

import network
from network import WLAN

#this sets up the network access point with the following parameters
#wlan = WLAN(mode=WLAN.AP, ssid = 'Pycom', auth =None, channel=1,antenna=None,power_save=False,hidden=True)
print("Network Setup!")

#turns off the pycom blue heartbeat
pycom.heartbeat(False)

#sets the rgbled to a purple color
pycom.rgbled(0x111111)

def main():
    port = 324
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    print('Socket created')
    server_address = ('',port)
    sock.bind(server_address)
    print('Socket bind complete')
    sock.listen(1)
    print('Socket now listening')
    print('Accepting clients on port',port)
    try:
        while True:
            conn,addr = sock.accept()
            print('Client connected:: ', addr)
            start_new_thread(clientthread, (conn,port,sock))
    finally:
        print('End')


def clientthread(connection,port,sock):
    # Sending message to connected client
    connection.sendto(('Welcome to the server.\n').encode(), ('',port))

    try:
        # Server accepts 5 messages before ending connection with client
        while True:
            data = connection.recv(16)
            if(data.decode()=='End of Message'):
                print('Breaking...')
                time.sleep(1)
                break
            print('Counter: ', int(data.decode()))
            # send message back to client

    finally:
        print('Closing connection')
        connection.close()


main()
