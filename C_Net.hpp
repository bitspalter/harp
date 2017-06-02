//////////////////////////////////////////////////////////////////////////////////
// [ Net_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// Net
//
// [::Last modi: 01.03.17 L.ey (µ~)::]
//
//
#ifndef _C_NET_H_
 #define _C_NET_H_
 
   #include <iostream>
   #include <fstream>
   #include <string>
   #include <sstream>
   #include <iomanip>
   
   #include <errno.h>
   
   using namespace std;
 
   #include <sigc++/sigc++.h>
   
   #include <glibmm/dispatcher.h>
   
   #include "Net_Protokoll_Header.hpp"
   
   #include "C_Net_Interface.hpp"
   #include "C_Net_Raw.hpp"
   #include "C_Net_Raw_Arp.hpp"
   
   typedef unsigned char  UCHAR;
   typedef unsigned short WORD;
   typedef unsigned int   DWORD;
 
//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
   const int C_NET_ERROR = 0x00;
   const int C_NET_READY = 0x01;
   
   const int C_NET_ID_ARP = 0xFFAA;
   
//////////////////////////////////////////////////////////////////////////////////
// STRUCT
//////////////////////////////////////////////////////////////////////////////////

   struct S_Arp_Result {
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

	 int start();
	 int send(S_Net_Interface* pSInterface, int cPackets, int cSleep);

	 ///////////////////////////////////////////////////////////////
       
         C_Net_Interface  CNInterface;
	 
	 C_Array CA_Arp;
	 
         C_Net_Raw_Arp CNArp;
	 C_Net_Raw     CNRaw;
	 
	 C_DArray CDA_Result;
	 
	 /////////////////////////////////////////
	 
	 // Recive Signal
	 void on_arp_data(int id, int cData);
	 
	 Glib::Dispatcher sig_arp_data;

      private: 

         int status;
   };
 
#endif // _C_NET_H_
 