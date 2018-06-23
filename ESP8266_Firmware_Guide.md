# Software

- You will need python installed, preferably the latest version of 3.x.  Visit python.org, download the installer for Windows, and run the installer with all of the default options.

# Getting the board installed.

- Install drivers for ESP8266 by visiting https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers
- Connect the ESP8266 board via a USB cable to your computer.
- Check which COM port the board is assigned by clicking the Windows start menu and typing "devmgmt.exe".
- Expand the "Ports (COM & LPT)" section and look for the "Silicon Labs CP210x USB to UART Bridge". At the end of this line it will list the COM port number, take note of this.

# Flashing the firmware to support micropython

The official tutorial can be found at https://docs.micropython.org/en/latest/esp8266/esp8266/tutorial/intro.html for reference, but here is a quick guide:
- Downlaod the esp tool files at https://github.com/espressif/esptool/ and extract them to a folder.  Keep the folder open in the file browser, because we'll drop another file in here later.
- In windows, browse to the esptool folder where the files are kept.  In the file browser window, hold down Shift+Ctrl and right click, then select "Open command prompt here.
- In the command prompt that appears, type "esptool.py" --port COM# erase_flash" where the # sign is the port number you located above.  The output should look like the following:
```
esptool.py v2.4.1
Serial port COM13
Connecting....
Detecting chip type... ESP8266
Chip is ESP8266EX
Features: WiFi
MAC: 60:01:94:38:04:3a
Uploading stub...
Running stub...
Stub running...
Erasing flash (this may take a while)...
Chip erase completed successfully in 7.3s
Hard resetting via RTS pin...
```
- Keep this command prompt open, as we will use the same window for the next step.
- Download the latest micropython firmware from the micropython website: http://micropython.org/download#esp8266.  It should be listed under the heading "Firware for ESP8266 boards", and look for the .zip file labeled "latest".
- Extract the downloaded zip file, and find the .bin file hidden a few folders deep.  Copy this file and paste it into the same esptool files folder you used above.
- Go back to the command prompt and type in the following command, but with two changes: Replace the # sign with your COM port number you found above, and replace the FILENAME with the name of the .bin file you just pasted into the folder.
```
esptool.py --port COM# --baud 460800 write_flash --flash_size=detect -fm dio 0 FILENAME.bin
```
- You should see the following output:
```
esptool.py v2.4.1
Serial port COM13
Connecting....
Detecting chip type... ESP8266
Chip is ESP8266EX
Features: WiFi
MAC: 60:01:94:38:04:3a
Uploading stub...
Running stub...
Stub running...
Changing baud rate to 460800
Changed.
Configuring flash size...
Auto-detected Flash size: 4MB
Compressed 187920 bytes to 130185...
Wrote 187920 bytes (130185 compressed) at 0x00000000 in 3.1 seconds (effective 4
81.8 kbit/s)...
Hash of data verified.

Leaving...
Hard resetting via RTS pin...
```
--You've now loaded the latest micropython firmware to your ESP8266.
