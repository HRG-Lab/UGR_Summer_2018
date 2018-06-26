import socket
import sys
global led
import pycom, binascii

# Run network_pycom then run network_huzzah before running server.py and client.py

#turns off the pycom blue heartbeat
pycom.heartbeat(False)

#sets the rgbled to a purple color
pycom.rgbled(0xF333FF)


def main():

	# Setup port and socket
	port = 324
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

	# Bind to port
	server_address = ('', port)
	try:
		sock.bind(server_address)
	except socket.error as msg:
		print('Bind Failed. Error code ' + str(msg[0]))
		sys.exit
	print('Socket bind complete')

	# Start the listening to incoming connections
	sock.listen(1)
	print('Socket now listening')
	accept_connection(sock,port)

def accept_connection(sock, port):
	# Start the client acceptance system
	# This function will run on the successful connection of a client

	# conn is a new socket object able to send/receive data on the accept_connection
	# addr is the address bound to the socket on the other end of the accept_connection
	conn,addr = sock.accept()
	print('Connected with ' + addr[0] + ':' + str(addr[1]))
	print("client connected :: ",addr)
	receive_data(conn,sock)


def receive_data(conn,sock):
	# This function attempts to receive data from the client
	try:
	# While true, loop to receive data
		while True:
			# Receive 16 bytes of data
			inst = conn.recv(16).decode()
			avg = conn.recv(16).decode()

			if(inst == 'End of Message' or avg =='End of Message'):
				# If 'End of message' is received, stop receiving data from client
				print("Inst: " + inst + ' avg: ' + avg)
				break
			else:
				mappedInst = convertData(float(inst))
				mappedAvg = convertData(float(avg))
				ledData(mappedInst)
				print("Inst:", mappedInst, ' avg:' , mappedAvg)
	finally:
		# Close the connection b/w socket then close the socket
		conn.close()
		sock.close()

def convertData(oldValue):
	# This function converts the ADC value to cm
	# Min and Max of Sensor integers
	oldMin = 70
	oldMax = 210
	oldRange = oldMax - oldMin

	# Min and Max of distance (cm)
	newMax = 76.2
	newMin = 12.7
	newRange = newMax - newMin
	newVal = (((oldValue - oldMin) * newRange)/ oldRange) + newMin

	return newVal

def ledData(mappedVal):
	if(mappedVal < 12.7):
		pycom.rgbled(0x000000)
	if(12.7 < mappedVal < 20):
		pycom.rgbled(0xFF00FF)
	if(20 < mappedVal < 30):
		pycom.rgbled(0x9000FF)
	if(30 < mappedVal < 40):
		pycom.rgbled(0x0000FF)
	if(40 < mappedVal < 50):
		pycom.rgbled(0x00FFF2)
	if(50 < mappedVal < 60):
		pycom.rgbled(0x10FF00)
	if(60 < mappedVal < 70):
		pycom.rgbled(0xF2FF00)
	if(mappedVal > 70):
		pycom.rgbled(0xFF0000)

'''
def process_data(data):
	#process the data, and change LED based on input
	if(data == b'led green'):
		pycom.rgbled(0x007f00)
	if(data == b'led red'):
		pycom.rgbled(0x7f0000)
	if(data == b'led blue'):
		pycom.rgbled(0x0000ff)
	if("0x" in data):
		#all the data is sent in binary mode, which we need to decode to get the string version of the data
		data = data.decode('utf-8')
		print(data)
		#gets rid of the 'led 0x', and typecasts it in base 16 (aka hex)
		color = int(data[data.index("x")+1:],16)
		#sets the color
		pycom.rgbled(color)
	return False
'''

#runs the main function on an import
main()
