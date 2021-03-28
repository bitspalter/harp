//////////////////////////////////////////////////////////////////////////////////
// [ Net_Interface_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// Last modi: 13.03.21 L.ey (µ~)
//
#ifndef _C_NET_INTERFACE_H_
 #define _C_NET_INTERFACE_H_

  #include <cstdint>
  #include <iostream>
  #include <stdio.h>
  #include <vector>
  
  using namespace std;

  #include <sys/ioctl.h> 
  #include <net/if.h>
  #include <sys/socket.h> 
  #include <arpa/inet.h> // in_addr Struktur
  #include <unistd.h>

  #include <string.h>
  
//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////

 const int C_NET_INTERFACE_READY = 0x01;
 const int C_NET_INTERFACE_ERROR = 0x00;

//////////////////////////////////////////////////////////////////////////////////
// STRUCT
//////////////////////////////////////////////////////////////////////////////////

 struct S_Net_Interface {
    char      ps_Name[16];
    uint32_t  dw_IP;
    uint32_t  dw_Netmask;
    uint32_t  dw_Network;
    uint32_t  dw_Broadcast;
    char      ps_IP[16];
    char      ps_Netmask[16];
    char      ps_Network[16];
    char      ps_Broadcast[16];
    char      ps_MAC[20];
    uint8_t   uc_MAC[6];
    uint32_t  dw_Flags;
    uint8_t   uc_Active;
    uint8_t   uc_LoopBack;
    uint32_t  dw_index;
 };

//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////

 class C_Net_Interface {
   
   public:

     C_Net_Interface(){};
    ~C_Net_Interface(){};

     int create();
     int get_First_Active();

     const S_Net_Interface* get_pInterface(uint32_t nInterface);
     
     // Inline
     //
     const vector<S_Net_Interface>* get_pDA_Interface(){return(&vInterface);}

   private:

     int fill(char* psDevice, S_Net_Interface* pS_Interface);

     vector<S_Net_Interface> vInterface;
 };

#endif // _C_NET_INTERFACE_H_
