# Setting up Atom for micropython on the ESP8266
- To begin, download the Atom v1.27.2 release from the GitHub repository (this is not the latest version, but the Pymakr plugin currently only supports up to this version as of June 2018).  https://github.com/atom/atom/releases  (scroll down and look for the AtomSetup-x64/32-windows.exe file for your approprite version of windows.
- Once Atom is installed, open it, go to File->Settings.  On the left menu of the settings tab click on Packages, and type "Pymakr" into the filter input.  Under "Community Packages" you should see "pymakr" (version 1.3.3) listed.  Click Install. If successful, there will now be a console window at the bottom of your Atom window.
- In Atom, go to File->New File.  Browse to your desktop and create a new folder for your demo project, then save the file as "main.py" in this new folder.
- After saving the file, you should see Atom's left toolbar populate with your new folder and your main.py file.
- Now paste the following code into your main.py file:
```
import utime
from machine import Pin

def main():
    led = Pin(2, Pin.OUT)
    enabled = False
    while True:
        if enabled:
            led.off()
        else:
            led.on()
        utime.sleep_ms(1000)
        enabled = not enabled

if __name__ == '__main__':
    main()
```
This code will simply blink the ESP8266's built-in LED on and off every second.
- Save your main.py file.
- In the Pymakr console pane there are some buttons at the top right. Click "Settings->Global Settings".  In the new tab that appears, scroll down to the settings area and change "Device Address" to COM# -- changing the # sign to the ESP8266 COM port number (to find the COM port number in Windows, click Start->Control Panel->Device Manager and look under the Ports section).
- Press the reset button on the top of your ESP8266 board, and then click the "connect" button in the Pymaker console pane. In the console you might see some weird characters scroll by, but this is normal.
- In Atom, select the "main.py" tab, and then press "Upload".
- You should then see something similar to the following:
```
Uploading project (main folder)...
Not safe booting, disabled in settings
Reading file status
[1/1] Writing file main.py
Upload done, resetting board...
OK>
```
You will also probably see more weird characters scroll by, but this is normal.
- If successful, you should then see the LED on your ESP8266 board start blinking.
