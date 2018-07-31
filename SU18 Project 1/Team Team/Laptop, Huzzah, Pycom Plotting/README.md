# Pycom
- Run `network_pycom.py` on the Pycom module to set up the wifi over which the huzzah and the laptop connect
# Laptop
- Connect to the Pycom Wifi on your laptop. It should be named `'Pycom'` seen in `network_pycom.py`.
- Run `PlottingDatapath.py` in a Python IDE. This will create a socket for the laptop/huzzah connection.
# Huzzah
- Before running `rssi_client.py` on the Huzzah, check your laptop's IP address after connecting to the wifi.
- Change `address` to the IP address for your huzzah to connect to the laptop and then run `rssi_client.py`.
# Notes
- There are potential errors that I have run into even though the code works correctly: 
  * `PlottingDatapath.py` might give an error in regards to  'Address already bound'. 
You must end the script and detach the process. In pycharm, this is done by hitting the `Pause and Kill or 'X'` on the console.
  * `rssi_client.py` might display `ECONNABORTED` or `ECONNRESET`. To get by this, 
  I ran the code until it connected to the laptop and started to send data

- I have attempted this with 2 huzzahs using 2 sensor IDs. After connecting the first client, the second client can be connected 
anytime afterwards and continue to be plotted.

# Further
- Currently, the plot plots a compressing line as time goes on. We want to implement a plot that has a moving time window, i.e
it only plots the last 10 seconds or last 10 data points
