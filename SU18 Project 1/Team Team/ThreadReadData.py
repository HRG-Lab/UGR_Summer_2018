import socket
import threading
from _thread import *

from collections import deque
import time

from drawnow import *

dataArray = []

class DataPlot:
    def __init__(self, max_entries=15):
        self.axis_x = deque(maxlen=max_entries)
        self.axis_y = deque(maxlen=max_entries)

        self.max_entries = max_entries

        self.buf1 = deque(maxlen=5)

    def add(self, x, y):
        self.axis_x.append(x)
        self.axis_y.append(y)

class RealtimePlot:
    def __init__(self, axes):
        self.axes = axes

        self.lineplot, = axes.plot([], [], 'ro-')

    def plot(self, dataPlot):
        self.lineplot.set_data(dataPlot.axis_x, dataPlot.axis_y)

        self.axes.set_xlim(min(dataPlot.axis_x), max(dataPlot.axis_x))
        ymin = -80
        ymax = -10
        self.axes.set_ylim(ymin, ymax)
        self.axes.relim()

# Note : Split data mistake into 2 parts

class HuzzahClient(threading.Thread):
    # A client class that adds to the thread for every client created
    def __init__(self, sock, addr, conn):
        threading.Thread.__init__(self)
        self.socket = sock
        self.address = addr
        self.connection = conn
        self.dataArray = []
        self.decoded_data = 0
        self.count = 0

    def run(self):
        # Function for the thread's activity
        while True:
            # Begin receiving data from connection
            self.decoded_data = self.connection.recv(16).decode()

            # Case to break from client if end of message is reached
            if self.decoded_data == 'End of Message':
                print('Breaking...')
                time.sleep(1)
                break

            # Ignore mistakes from received data
            if int(self.decoded_data) > 100:
                pass
            else:
                self.dataArray.append(-1 * int(self.decoded_data))
            drawnow(self.makeFig)

            # Plot newest 5 points AKA moving time window
            self.count += 1
            if self.count > 5:
                self.dataArray.pop(0)
            print('Receieved Data', self.decoded_data)

    def makeFig(self):
        # This function creates the plot to be animated
        plt.plot(self.dataArray, 'ro-', label='Rssi [dB]')
        time.sleep(1)


def spain():
    plt.ylim([-80, -20])
    plt.title('Rssi v. Time')
    plt.grid(True)
    plt.ylabel('Rssi [dB]')
    # Define socket and port
    port = 8000
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    print('Socket created')

    # Bind to port
    server_address = ('', port)
    sock.bind(server_address)
    print('Socket bind complete')

    # Start listening from port
    sock.listen(1)
    print('Socket now listening')
    print('Accepting clients on port', port)

    # Create an interactive live plot
    plt.ion()
    cnt = 0
    try:
        while True:
            conn, addr = sock.accept()
            print('Client connected:: ', addr)
            # Send a message to the connected client
            conn.sendto('Welcome to the server.'.encode(), ('', port))

            h = HuzzahClient(sock, addr, conn)
            start_new_thread(h.run, (conn,))
            conn.close()
            print('Broken')
    finally:
        print('End')

'''
def clientthread(conn, port, dp, rp):
    # Send welcome message to connected client
    conn.sendto('Welcome to the server.'.encode(), ('', port))
    counter = 0
    try:
        # Server accepts 5 messages before ending connection with client
        while True:
            data = conn.recv(16)
            decodeddata = data.decode()
            if decodeddata == 'End of Message':
                print('Breaking..')
                time.sleep(1)
                break
            # print('Counter:', int(decodeddata))
            counter += 1
            dp.add(x=counter, y=-1 * int(decodeddata))
            rp.plot(dp)
            plt.pause(0.001)
    finally:
        print('Closing connection')
        conn.close()
'''
spain()
