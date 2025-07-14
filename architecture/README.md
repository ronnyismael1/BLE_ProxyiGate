# DESIGN OF THE PROJECT

## Technology used
1. ESP32 microcontroller
2. nRF52 Bluetooth Beacon
3. Servo motors

## BOM
1. [nRF52 Bluetooth Beacon](https://www.alibaba.com/product-detail/Low-Energy-IoT-Sensor-Bluetooth-Beacon-1601293289216.html)
  * Order no. [#268518575501029968](https://biz.alibaba.com/ta/detail.htm?orderId=268518575501029968)
2. [Miuzei MG90S 9G Micro Servo Motor](https://www.amazon.com/gp/product/B0BWJ4RKGV/ref=ewc_pr_img_1?smid=A34CQKEVNF2MJX&th=1)
**how to open bom excel** `libreoffice --calc ~/Repos/project_ble_proxigate/architecture/bom_ble_proxigate.xlsx &`


## Project Design
- Simon will wear BLE beacon tag on his collar, and ESP32 native BLE will be used to detect the tag. ESP32 will tell approximate proximity of the tag using RSSI (Received Signal Strength Indicator). Not precise, but good enough. Trigger motors when dog is within like 1 meter.

