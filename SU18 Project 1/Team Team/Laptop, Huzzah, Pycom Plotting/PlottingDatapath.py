import socket
from multiprocessing import Queue
import threading
from threading import Thread
import time

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Initialize information needed to create a socket
PORT = 8000
IP = ''
SERVER_ADDRESS = (IP, PORT)


stopEvent = threading.Event()  # Initialize threading. An event object manages an internal flag that is set to T/F


class Server(Thread):
    """
    Server class to initialize socket connection to Pycom wifi
    """

    def __init__(self, dataQueue):
        Thread.__init__(self)
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

        # Bind To Socket
        sock.bind(SERVER_ADDRESS)
        print('Socket Bound')

        self.sock = sock
        self.dataQueue = dataQueue
        self._stopped = False



    def run(self):
        """
        Starts loop that listens for and processes client connections
        """
        print('Starting Server Thread')
        self.sock.listen(1)
        try:
            print('Listening for incoming connections')
            while not stopEvent.isSet():
                conn, addr = self.sock.accept()
                print('Client connected::', addr)

                # Creates a client in a thread with the accepted connection
                receiver = Receiver(conn, addr, self.dataQueue, self)
                receiver.start()
        finally:
            print('End')

    def close(self):
        self._stopped = True
        stopEvent.set()
        self.sock.close()


class Receiver(Thread):
    """
    Creates a single client to receive Data from
    """

    def __init__(self, conn, addr, dataQueue, server):
        Thread.__init__(self)
        self.conn = conn
        self.addr = addr
        self.dataQueue = dataQueue
        self._stopped = False
        self._server = server
        self.ydata = []

    def getOneDataPoint(self):
        print('Getting Data')
        print('Empty?:', self.dataQueue.empty())
        while not stopEvent.isSet():

            # Receives sensor and data from client
            sensorID, data = self.conn.recv(16).decode().split(',')

            # Ends the client connection once 'End of Message' is received
            if data == 'End of Message':
                print('Breaking...')
                time.sleep(1)
                print('Broken')
                break

            self.ydata.append(int(data))  # Loads data received into an array to be plotted

            self.dataQueue.put((sensorID, self.ydata))  # Uploads a tuple of sensorID and array onto the Queue

            print('SensorID:', sensorID, 'Y:', self.ydata)

        self.postReceive()

    def run(self):
        print('Client Started')
        self.getOneDataPoint()
        print('Ending due to lack of data')
        self.stop()

    def postReceive(self):
        pass

    def stop(self):
        self._stopped = True
        self.close()

    def close(self):
        self.conn.close()


class Plotter:
    """
    Class creates a Plotting window that updates as data becomes available from clients
    """
    def __init__(self, dataq):
        self.fig, self.ax = plt.subplots()

        self.ax.set_ylim(20, 80)

        self.ax.grid()
        plt.xlabel('Time (s)')
        plt.ylabel(r'RSSI [dB]')
        plt.legend(loc='lower left')

        self.dataq = dataq  # The Queue

        self._stopped = False

    def stop(self):
        self._stopped = True
        stopEvent.set()

    def getData(self, i):  # i is included because running FuncAnimation gives 2 parameters to getData
        """
        Consume data in the Queue as fast as it becomes available

        This parses the data from the Queue
        """

        while not self._stopped and not self.dataq.empty():

            data = self.dataq.get()  # Gets the tuple Data from the Queue and removes it from the Queue
            print('DataQ', data)
            print('Got the Data')

            sensorID, val = int(data[0]), data[1]  # Val is the y-data array

            # Differentiating between 2 sensor IDs
            print('Setting Data')
            if sensorID == 1:
                plt.plot(val, 'r--')

            elif sensorID == 2:
                plt.plot(val, 'b--')


    def run(self):
        print('Starting Plotting Thread')
        ani = animation.FuncAnimation(self.fig, self.getData)
        plt.show()

if __name__ == '__main__':

    dataqueue = Queue()  # Initialize the Queue

    try:
        # .start() allows for the run function to begin in a separate thread of control

        # Initialize the server
        server = Server(dataqueue)
        server.start()

        # Initialize the plotter
        plotter = Plotter(dataqueue)
        plotter.run()

    finally:
        if not server:
            print('closing')
            server.close()

