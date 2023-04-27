# ISM330DHCX_CXX C++ Library

The ISM330DHCX is a six Degrees of Freedom (6DoF) Inertial Measurement Unit (IMU) that comprises a high-performance 3D digital accelerometer and 3D digital gyroscope. This IMU Breakout offers a broad range of capabilities, including a full-scale acceleration range of ±2/±4/±8/±16 g and a wide angular rate range of ±125/±250/±500/±1000/±2000/±4000 dps. It supports both SPI and I2C serial interfaces for data output from the gyroscope and accelerometer. However, it's important to note that this library exclusively supports the I2C interface.  If you require more detailed information, you can consult the information and documents available from [SparkFun](https://www.sparkfun.com/products/20176) or [Adafruit](https://www.adafruit.com/product/4502). Additionally, please be aware that the default I2C address for SparkFun is 0x6B, while for the Adafruit board, the default address is 0x6A.

Unlike the Arduino library, this particular library can be seamlessly integrated into any C++ project. It can be utilized on any Single-Board Computer (SBC), such as Raspberry Pi or Odroid n2, Odroid n2l, ..., without any issues. The library utilizes wiringpi to establish communication via the I2C protocol.

# Install WiringPi
## Raspberry Pi
To install WiringPi on a Raspberry Pi running either Raspbian or Ubuntu operating system, you can follow these steps:

```bash
sudo apt update
sudo apt install wiringpi
```
Then to check it is working:

```bash
gpio -v
```
## Odroid
On an Odroid board follow the following steps:

```bash
sudo apt install software-properties-common
sudo add-apt-repository ppa:hardkernel/ppa
sudo apt update
sudo apt install odroid-wiringpi
sudo apt install libwiringpi-dev
```
For further information regarding Odroid GPIO, you can refer to the [Hardkernel Wiki](https://wiki.odroid.com/start).
# Build 
A cmake list is provided in the main folder of the library. After cloning the repository, To build the library with examples follow these steps:
```bash
cd ./ISM330DHCX_CXX
mkdir build
cd build
cmake ..
make
```
use `make -j6` to use 6 threads for building. 

# Examples

The examples folder contains three illustrative examples. The first example demonstrates a straightforward process of reading and printing outputs. The second example showcases a multithreaded logger that can be compiled and built on any operating system. Finally, the third example presents a real-time implementation of the multithreaded logger, offering exceptionally accurate timing with a sampling rate of up to 833Hz. It's important to note that the third example utilizes Linux real-time threads and can only be built on Linux operating systems. Please be aware that the third example is still pending to be pushed to the repository.

# Sample Data

As part of my MSc thesis, I have gathered IMU recordings of regional trains. The third example in this repository was employed on an Odroid N2L single-board computer (SBC) running Ubuntu Server OS to acquire the data. The collected data can be found in the data folder of this repository. The train in question is a regional express train that commences its journey from Ferrara, a city in Italy, and terminates at the central train station in Bologna. Additionally, there is a stop along the route at San Pietro in Casale. The following figure shows the route in UTM zone 32:

![scatter plot of the recorded samples](https://github.com/Mahdi-Abdollahpour/ISM330DHCX_CXX/blob/main/figures/map.jpg?raw=true)

The initial five rows of the data file contain metadata such as the date and time of the recording and the coordinates of the departure and arrival points of the journey. The coordinates of  the departure and arrival points has a minimum uncertainty of 40 meters. The data itself consists of nine columns:

imu | dt | rawAccX | rawAccY | rawAccZ | rawGyrX | rawGyrY | rawGyrZ
column title | explanation
------------- | -------------
imu  | Data tag meaning this row of data is IMU data(as it can be ubx data too).
dt  | The number of micro-seconds from last sample
rawAccX | Raw acceleration in the x-axis
rawAccY | Raw acceleration in the y-axis
rawAccZ | Raw acceleration in the z-axis
rawGyrX | Raw angular rate in the x-axis
rawGyrY | Raw angular rate in the y-axis
rawGyrZ | Raw angular rate in the z-axis
label | Manual motion label; 0/1 stationary/moving states of the train respectively.
## Timing of the samples

The statistics of the timing dt in 3 different set-ups is illustrated in the following figures.

![scatter plot of the recorded samples](https://github.com/Mahdi-Abdollahpour/ISM330DHCX_CXX/blob/main/figures/dt_scatter.jpg?raw=true)

The improved-synchronization is the example 3 of this repository on an Odroid n2l with ubuntu-server OS. The Arduino is an impelementation of the IMU logger on an Arduino UNO rev3. The Typical-Synchronization shows a typical logging set-up which is widely used.

![scatter plot of the recorded samples](https://github.com/Mahdi-Abdollahpour/ISM330DHCX_CXX/blob/main/figures/dt_hist.jpg?raw=true)
## Hardware

An Arduino uno rev3 and and Odroid n2l are used for the recording of the data. The images are shown below:
![scatter plot of the recorded samples](https://github.com/Mahdi-Abdollahpour/ISM330DHCX_CXX/blob/main/figures/arduino.jpg?raw=true)
![scatter plot of the recorded samples](https://github.com/Mahdi-Abdollahpour/ISM330DHCX_CXX/blob/main/figures/odroid.jpg?raw=true)

## Converting Raw Data To ISI
A functionality to read data file and convert raw data to ISI units will be provided.
