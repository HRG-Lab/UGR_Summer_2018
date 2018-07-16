1. Connect huzzah to pycom network, record dhcp ip
2. Connect laptop to pycom network, record dhcp ip
3. Edit rssi_clientv2.py and data_acquisition.py address to pycom host address (should be 192.168.4.1)
4. Edit rssi_clientv2.py and data_acquisition.py client address to their corresponding dhcp ip from the pycom network
5. Launch switchboard_server.py
6. Launch data_acquisition.py
7. Launch run.bat to run the rssi_clientv2.py (critical, do not run in idle, will not work)
8. Press "r" in rssi_clientv2.py batch menu to add an rssi client
9. Press "a" to enter the adding process
10. Type ip of huzzah that you are trying to connect to
11. Repeat 8-10 for all huzzah devices
12. Press "p" to begin plotting (this will likely not work)


There is a lot of debugging information still outputting, I will be working on this later today to try and narrow it down
I will be working on the plotting as well, it is coming along in theory okay, but I have yet to actually plot anything. Will have time tonight to work on it.

