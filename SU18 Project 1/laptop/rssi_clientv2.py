import sys,socket,time,threading,struct,msvcrt,traceback
import numpy as np
import matplotlib.pyplot as plt

class plotter(threading.Thread):
    plot_data = []
    x_data = []

    delay = 0.01

    fig = None
    ax = None

    li_objs = []

    time_start = None

    

    def random_color(self):
        return list(np.random.choice(range(0,100,1), size=3)/100)

    def __init__(self, num_plots, names):
        threading.Thread.__init__(self)

        self.fig = plt.figure()
        self.ax = self.fig.add_subplot(111)

        for x in range(num_plots):
            li, = self.ax.plot(0,0,color = self.rand_color())
            li_objs.append(li)

        self.ax.legend(names)
        
        time_start = time.time()
        self.fig = plt.figure()
        self.ax = self.fig.add_subplot(111)
        self.fig.show()
        self.fig.canvas.draw()
        plt.show(block=False)
        plt.pause(self.delay)
    def add_value(self, value, position):
        for x in range(0,len(values)):
            if x is position:
                self.plot_data[x].append(value)
            else:
                if len(plot_data[x]) > 1 :
                    self.plot_data[x].append(plot_data[len(plot_data[x])-1])
                else:
                    self.plot_data[x].append(0)
        self.x_data.append(time.time()-self.time_start)
    def run(self):

        for li_obj in li_objs:
            li_obj.set_xdata(self.x_data)
        for plots in plot_data:
            li_obj.set_ydata(plot)
        
        #relimit the axes
        self.ax.relim()
        #autoscale the axes
        self.ax.autoscale_view(True,True,True)
        #update the axis
        self.fig.canvas.draw()
        #delay the plot, a annoying but required process
        plt.pause(self.delay)
        time.sleep(self.delay)


class thread_command(threading.Thread):

    
    def __init__(self):
        threading.Thread.__init__(self)

    def main(self):
        global plot_start
        while True:
            key_press = ord(msvcrt.getch())
            print(key_press)
            if key_press is 112:
                plot_start = True
                print("plot started")
            if key_press is 114:
                print("Press A to add or R to remove :: ")
                print("Currently Connected")
                print("-------------------")
                for x in range(0,len(rssi)):
                    print(x," ",rssi[x][0])
                key_press = ord(msvcrt.getch())
                if(key_press is 97):
                    ip = input("Enter IP of rssi client :: ")
                    self.stream_request(sock,source, ip,"rssi","True")
                if(key_press is 114):
                    try:
                        num = input("Enter the number to remove :: ")
                        self.stream_request(sock,source, ip,"rssi","False")
                        del rssi[num]
                    except:
                        print("Error, please try again")
            if key_press is 117:
                print("Press A to add or R to remove :: ")
                print("Currently Connected")
                print("-------------------")
                for x in range(0,len(rssi)):
                    print(x," ",ultrasonic[x][0])
                key_press = ord(msvcrt.getch())
                if(key_press is 97):
                    ip = input("Enter IP of ultrasonic client :: ")
                    self.stream_request(sock,source, ip,"ultrasonic","True")
                if(key_press is 114):
                    try:
                        num = input("Enter the number to remove :: ")
                        self.stream_request(sock,source, ip,"ultrasonic","False")
                        del ultrasonic[num]
                    except:
                        print("Error, please try again")
    def stream_request(self, sock,source, destination,label,data):
        tag = "stream-request"
        header = str(destination)+","+str(source)+","+str(tag)
        message = header+"+"+str(label)+"+"+str(data)+";"
        message = bytes(message.encode('utf-8'))
        msg = struct.pack('>I', len(message)) + message 
        sock.sendall(msg)

def decompose_header(header):
	destination = header[:header.index(",")]
	header = header[header.index(",")+1:]

	source = header[:header.index(",")]
	header = header[header.index(",")+1:]

	label = header

	return [destination,source,label]
def recv_msg(sock):
    # Read message length and unpack it into an integer
    raw_msglen = recvall(sock, 4)
    if not raw_msglen:
        return None
    msglen = struct.unpack('>I', raw_msglen)[0]
    # Read the message data
    return recvall(sock, msglen).decode('utf-8')
def recvall(sock, n):
    # Helper function to recv n bytes or return None if EOF is hit
    data = b''
    while len(data) < n:
        packet = sock.recv(n - len(data))
        if not packet:
            return None
        data += packet
    return data
def unpack(data,delimiter_sub,delimiter_break):
    data_header = []
    data_labels = []
    data_values = []
    while len(data) > 0:
        data_header.append(data[:data.index(delimiter_sub)])
        data = data[data.index(delimiter_sub)+1:]
        data_labels.append(data[:data.index(delimiter_sub)])
        data = data[data.index(delimiter_sub)+1:]
        data_values.append(data[:data.index(delimiter_break)])
        data = data[data.index(delimiter_break)+1:]
    return [data_header,data_labels,data_values]


lock = threading.Lock()

#server socket setup (client)
sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)

address = "192.168.4.2"

port = 324
print("hello?")
server_address = (address, port)
print("hello2")
sock.connect(server_address)
print("hello3")
source = "192.168.4.3"

sock.setblocking(0)

tc = thread_command()
tc.start()


rssi_num = 0
rssi = []
ultrasonic = []

plot_run = False



plot_start = False

try:
    while True:
        print("test1")

        try:
            data = recv_msg(sock)
            [recv_data_headers,recv_data_labels,recv_data_values] = unpack(data,"+",";")
        except:
            #no data
            pass
        try:

            for x in range(0,len(recv_data_headers)):
                header = recv_data_headers[x]
                label = recv_data_labels[x]
                value = recv_data_values[x]
                [dest, source, tag] = decompose_header(header)
                print(label)
                print(dest)
                print(source)
                print(tag)
                if tag == "stream-data" and value == "rssi":
                    found = False
                    for graphs in rssi:
                        if graphs[0] == source:
                            #add a position of the length to see if the
                            #list has been updated
                            graphs[1].append(value)
                            found = True
                            break
                    if not found:
                        rssi.append([source,[value]])
                if tag == "stream-data" and value == "ultrasonic":
                    found = False
                    for graphs in ultrasonic:
                        if graphs[0] == source:
                            graphs[1].append(value)
                            found = True
                            break
                    if not found:
                        ultrasonic.append([source,[value]])
            if(plot_start):
                rssi_plot = plotter([],"")
                rssi_plot.start()
                plot_run = True
            if(plot_run):
                
            

        except:
            pass
        
except Exception as e:
    print("Fatal Error ",e)
    traceback.print_exc()
