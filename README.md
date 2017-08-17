# harp (alpha)

1. compile:
g++ *.cpp -std=c++11 -o harp `pkg-config gtkmm-3.0 --cflags --libs`

2. set right to open raw socket as non root:
setcap 'cap_net_raw+ep' ./harp

![alt tag](https://github.com/bitspalter/harp/blob/master/harp.png "harp")
