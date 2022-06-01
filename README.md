## Buggy Project 2022
  
used in this project: 
* [wiringPi](http://wiringpi.com/download-and-install/)
* [Adafruit motohat cpp lib](https://github.com/tomfclarke/adafruit-motor-hat-cpp-library)
* [make](https://man7.org/linux/man-pages/man1/make.1.html)
 
### Preparing the Pi
1. hook pi to router via ethernet
2. check ip address of pi
3. open terminal on your host and change to parent dir of EES-Buggy
4. 
```
# default password for pi is raspberry
scp -r EES-Buggy pi@<IPADDRESS>:~/EES-Buggy
ssh pi@<IPADDRESS>

# on rpi
cd EES-Buggy
chmod +x setUpWlan.sh
./setUpWlan.sh
```
5. Disconnect pi from lan and check for raspianWifi 
 
### Building
 
wiringPi must be installed
```
cd EES-Buggy
make 
```


