//////////////////////////////////////////////////////////////////////////////////
// [ Net_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// Net
//
// [::Last modi: 17.03.21 L.ey (µ~)::]
//
//
#ifndef _C_NET_H_
 #define _C_NET_H_
 
   #include <cstdint>
   #include <iostream>
   #include <list>
   using namespace std;
 
   #include <sigc++/sigc++.h>
   #include <glibmm/dispatcher.h>
   
   #include "Net_Protokoll_Header.hpp"
   
   #include "C_Net_Interface.hpp"
   #include "C_Net_Raw.hpp"
   #include "C_Net_Raw_Arp.hpp"
   
//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
   const int C_NET_ERROR  = 0x00;
   const int C_NET_READY  = 0x01;
   const int C_NET_ID     = 0xFFAA;
   const int C_NET_BUFFER = 0x1400;
   
//////////////////////////////////////////////////////////////////////////////////
// STRUCT
//////////////////////////////////////////////////////////////////////////////////

   struct S_host_Result {
      UCHAR MAC[SIZE_MAC];  
      UCHAR IP[SIZE_IP];  
   };
   
//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////
   
 class C_Net : public sigc::trackable {
     
    public:

       C_Net(){};
      ~C_Net(){};
      
       int init();
       
       int start(const S_Net_Interface* pSInterface, int cPackets, int cSleep);
       int stop();
       
       ///////////////////////////////////////////////////////////////

       list<S_host_Result> CDA_Result;

       ////////////////////////////////

       C_Net_Interface CNInterface;

       ////////////////////////////////

       Glib::Dispatcher sig_packet;
       Glib::Dispatcher sig_finish;
       
    private: 

       C_Net_Raw     CNRaw;
       C_Net_Raw_Arp CNArp;
       
       // Recive Signal
       void on_raw_data(int id, int cData);
       
       UCHAR pBuffer[C_NET_BUFFER];
       
       S_host_Result SResult;
       
       bool bscann = {false};
       
       /////////////////////////////////
   
       UCHAR  pData[5000];
       DWORD  cData = 0;
   
       UCHAR IP_REQUEST[SIZE_IP];
       UCHAR IP_SOURCE[SIZE_IP];

       UCHAR MAC_REQUEST[SIZE_MAC];
       UCHAR MAC_SOURCE[SIZE_MAC];

       UCHAR MAC_ETH_D[SIZE_MAC];
       UCHAR MAC_ETH_S[SIZE_MAC];
   
       S_ArpPacket sArp;
 };
 
#endif // _C_NET_H_
