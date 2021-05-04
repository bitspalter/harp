# harp (alpha)

1. compile:
g++ *.cpp -std=c++11 -no-pie -o start -Wall \`pkg-config gtkmm-3.0 --cflags --libs\` -lcap

2. set capabilities for raw socket and promiscuous mode as non root:

   setcap cap_net_raw,cap_net_admin=eip ./harp

![alt tag](https://github.com/bitspalter/harp/blob/master/harp.png "harp")
