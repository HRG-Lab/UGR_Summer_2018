import sys,socket,time,machine,network
from machine import Pin, Signal

sta_if = network.WLAN(network.STA_IF)
ap_if = network.WLAN(network.AP_IF)

sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)

address = "192.168.4.2"
port = 8000
server_address = (address, port)
sock.connect(server_address)
sensorID = 1 # Assign the huzzah an ID

counter = 0
print('Connected')
for i in range(10):
    rssi = sta_if.status("rssi")*-1
    print(rssi)

    # Send sensorID and data in a tuple to server(laptop) to be parsed
    sock.sendto('{0:d},{1:d}'.format(sensorID, rssi),server_address)

    time.sleep(1)

message = 'End of Message'
time.sleep(0.5)
sock.sendto('{0:d},{1:s}'.format(sensorID,(message).encode()), (address,port))
print("exiting")
