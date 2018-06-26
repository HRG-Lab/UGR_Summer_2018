#imports
import sys
import socket
import time
from machine import Pin,Signal
import machine


# setup the ADC on ADC channel 0 (the labeled ADC on board)
adc = machine.ADC(0)

# Setup client socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
address = '192.168.4.1'
port = 324
server_address = (address, port)
print("Connecting to",address,"on port",port)

# Connects to server
sock.connect(server_address)

# Create an array to hold the past four data values read
avg = 0
past4 = [0 ,0 ,0 ,0]

for i in range(0,30):
	sum = 0
	# Read in value from ADC pin
	value = adc.read()

	# Add the value read from ADC pin and remove first value from the array
	past4.append(value)
	past4.pop(0)

	# Sum up the 4 values in the array and take their average
	counter = 0
	for u in range(0,4):
		if(past4[u]!=0):
			sum += past4[u]
			counter+=1
	avg = sum / counter

	# Send Instantaneous and Average values to server
	print ('Instant: '+ str(value)+' Avg: '+str(avg))
	#sock.sendall('Instant: '+ str(value)+' Avg: '+str(avg))
	sock.sendall(str(value))
	time.sleep(0.5)
	sock.sendall(str(avg))
	time.sleep(0.5)
time.sleep(0.5)
sock.sendall('End of Message')
time.sleep(0.5)
sock.sendall('End of Message')
