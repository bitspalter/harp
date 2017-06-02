//////////////////////////////////////////////////////////////////////////////////
//+++++++++++++++++[ RawSocket_Class_Header ]+++++++++++++++++++++++++++++++++++//
//////////////////////////////////////////////////////////////////////////////////
//
//
// [::Raw Socket Class::]
//
// [::Last modi: 04.01.17 L.ey (µ~)::]  
//
//
#ifndef _C_NET_RAW_H_
 #define _C_NET_RAW_H_
 
  #include <iostream>
  
  using namespace std;
  
  #include <string.h>
  #include <sys/socket.h>
  #include <unistd.h>
  #include <errno.h>
  
  #include <linux/if_packet.h>
  
  #include "C_Thread.hpp"
  #include "C_Net_Interface.hpp"
  #include "C_Array.hpp" 
 
  #include <sigc++/sigc++.h>
  
//////////////////////////////////////////////////////////////////////////////////

   const int C_NET_RAW_READY  = 0x01;
   const int C_NET_RAW_ERROR  = 0x00;

//////////////////////////////////////////////////////////////////////////////////

   class C_Net_Raw {
     
      public: 

         C_Net_Raw();  
        ~C_Net_Raw();  

         int open(S_Net_Interface* pSInterface);
         int close();

         int send(unsigned char* pData, unsigned int cData);
         int recv();

	 // Non Blocking
         int start(int id, C_Array* pCArray);
         int stop();

         ///////////////////////////////
         //
         // Inline 
         //
         bool bRunning(){return(bRun);};
	 
	 int getSocket(){return(sockfd);};
	 
         // Signal
         typedef sigc::signal<void, int, int> type_signal_data;
	 type_signal_data signal_data();
	 
      protected:
         type_signal_data m_signal_data;
	 
      private:
	
         struct sockaddr_ll socket_address;
	 
         int  sockfd;

         bool bOpen;

         // ((( RECIVE THREAD )))
         bool bRun;

         C_Thread <C_Net_Raw> CThread;

         void run();
	 
         int  id;
	 
         /////////////////
	 C_Array* pCAData;
   };

#endif // _C_NET_RAW_H_