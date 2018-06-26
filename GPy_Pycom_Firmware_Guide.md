# Software
- You will need python installed, preferably the latest version of 3.x. Visit python.org, download the installer for Windows, and run the installer with all of the default options.
# Updating the Exp Board 3.0 Driver and Firmware
Before connecting your module to the expansion board 3.0, the expansion board 3.0 firmware must be updated first.
The official tutorial can be found at https://docs.pycom.io/chapter/pytrackpysense/installation/firmware.html
- Download the DFU (Device Firmware Update) for the exp board here : https://software.pycom.io/findupgrade?key=expansion3.dfu&type=all&redirect=true
- Download DFU-util v0.9 and Zadig: http://dfu-util.sourceforge.net/releases/dfu-util-0.9-win64.zip and http://zadig.akeo.ie/
- The Hardware drivers must be installed before updating the firmware; Open Zadig and select `libusk` as the driver. 
  * To install the drivers, the Exp Board 3.0 must be in DFU mode:
     1. Disconnect the USB cable
     2. Hold down the **S1** button 
     3. Connect the USB cable
     4. When **Unknown Device #1** shows on the Zadig app, click *Install Driver* (There is a 7 second window between connecting the USB and clicking *Install Driver*)
- Open Device Manager from the Windows start menu to check for success.
  * Unplug the board. Hold down the **S1** button. Plug it back in/
    1. If you see **USB Serial Device (COMX)** under **Ports**, you don't have the board in DFU mode.
    2. If you see **Unknown Device** under **Other devices** then the board is in DFU mode but the driver is not installed.
    3. If you see **Unknown Deivce #1** under **libusK USB Devices**  then the board is in DFU mode and the driver is correctly installed.
- We will now update the firmware on the Exp Board 3.0
  * Going back to the earlier downloads, unzip dfu-util-0.9-win64.zip. Move the 'dfu-util-0.9-win64' from inside the zipfile to somewhere accessible.
  * Move the firmware update 'expansion3_x.x.x.dfu' into 'dfu-util-0.9-win64'
  * Open "Command Prompt" by hitting the `windows button + r` and typing "cmd.exe"
    1. On the command line type `dfu-util-static.exe -D expansion3_x.x.x.dfu`
    2. Get the board in DFU mode (hold down the **S1** buttonw) and connect the USB.
    3. Hit enter in "Command Prompt". A progress bar will pop up follwed by a line. 
        * If the line read "No error condition is present", then the Exp Board 3.0 has been successfully updated. 
        * If the line reads "No DFU capable USB device available" then you were too slow to start the update in command prompt or the correct drivers are not installed.
- Double-check Serial USB (CDC) is installed in Application Mode:
  * If the libusK driver was installed while the USB ID was in Application Mode (0xEF98 for Exp Board 3.0), then `Serial USB (CDC)` must be installed for Application Mode.
    1. Open the Zadig app
    2. Connect the USB normally without holding down any buttons.
    2. Select `USB Serial (CDC)` from the click down menu and click on *Install Driver*
# Updating PyCOM GPy Device Firmware
- The official tutorial can be found here: https://docs.pycom.io/chapter/gettingstarted/installation/firmwaretool.html#second
- Download the update tool: https://software.pycom.io/findupgrade?product=pycom-firmware-updater&type=all&platform=win32&redirect=true
- Insert the GPy module onto the Exp Board 3.0
  1. Run the "Pycom Firmware Update" tool software
  2. Click continue twice
  3. Select the *Serial Port (COMx)* your device is using. (This can be found by opening Device manager)
  4. Check the `High speed transfer` box; Select `pybytes` as the *Type*
  5. Click continue and finish the update.
  6. Disconnect the USB and reconnect it. The device is now ready to use
