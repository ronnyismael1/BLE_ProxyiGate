# About Using this Project

Turn on venv: `get_idf`

## HOW TO BUILD AND FLASH ESP32 DEVICE

### Building the Project

Okay since we entered venv and ran cmake, we can now build the project.

```
cd ~/Repos/project_mcu_dog/implementation/src
idf.py build
idf.py -p /dev/ttyUSB0 flash
```

You can also monitor the serial output after flashing:
`idf.py -p /dev/ttyUSB0 monitor`
To exit the monitor, press:
`Ctrl+]`

Or to run both at same time: `idf.py -p /dev/ttyUSB0 flash monitor`

Easy peasy, but since we are developing in WSL2 instance, we need to do some extra work, explained below.

### Binding DEVICE to WSL2 Instance
Download latest release of usbipd from their github.
Then we can run in powershell `usbipd list` to see available devices connected.

Run this before, and run after plugging in the ESP32 device. Run Powershell commands as admin.

Mine shows up like this below:
```
BUSID  VID:PID    DEVICE                                                        STATE
1-8    10c4:ea60  CP2102 USB to UART Bridge Controller                          Not shared
```

Now we will attach it the device to our WSL2 instance.

First, share the device by binding it to the WSL instance.
`usbipd bind --busid 1-8`

Then run the below. You should see that the STATE changed to shared.
`usbipd attach --busid 1-8 --wsl archlinux`

Now we can verified that it worked in our WSL2 instance by running `ls /dev/ttyUSB*`

## MENUCONFIG

We have a default sdkconfig, and then we have the real sdkconfig file. We can modify it with menuconfig.
`idf.py menuconfig`

Run that in the src directory. You might need to delete old build and cache first.

If you run into an issue with running idf.py I needed to change my locale settings.
```
export LANG=en_US.UTF-8
sudo vim /etc/locale.gen
# uncomment this line: en_US.UTF-8 UTF-8
sudo locale-gen
# verify with below
locale
```

