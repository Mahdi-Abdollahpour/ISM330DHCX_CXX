# ISM330DHCX_CXX C++ Library

The ISM330DHCX is a six Degrees of Freedom (6DoF) Inertial Measurement Unit (IMU) that comprises a high-performance 3D digital accelerometer and 3D digital gyroscope. This IMU Breakout offers a broad range of capabilities, including a full-scale acceleration range of ±2/±4/±8/±16 g and a wide angular rate range of ±125/±250/±500/±1000/±2000/±4000 dps. It supports both SPI and I2C serial interfaces for data output from the gyroscope and accelerometer. However, it's important to note that this library exclusively supports the I2C interface.  If you require more detailed information, you can consult the information and documents available from [SparkFun](https://www.sparkfun.com/products/20176) or [Adafruit](https://www.adafruit.com/product/4502). Additionally, please be aware that the default I2C address for SparkFun is 0x6B, while for the Adafruit board, the default address is 0x6A.

Unlike the Arduino library, this particular library can be seamlessly integrated into any C++ project. It can be utilized on any Single-Board Computer (SBC), such as Raspberry Pi or Odroid n2, Odroid n2l, ..., without any issues. The library utilizes wiringpi to establish communication via the I2C protocol.

## Install WiringPi
#Raspberry Pi
To install WiringPi on a Raspberry Pi running either Raspbian or Ubuntu operating system, you can follow these steps:

```bash
sudo apt update
sudo apt install wiringpi
```
Then to check it is working:

```bash
gpio -v
```
# Odroid
On an Odroid board follow the following steps:

```bash
sudo apt install software-properties-common
sudo add-apt-repository ppa:hardkernel/ppa
sudo apt update
sudo apt install odroid-wiringpi
sudo apt install libwiringpi-dev
```
For further information regarding Odroid GPIO, you can refer to the [Hardkernel Wiki](https://wiki.odroid.com/start).

