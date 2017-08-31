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
 #include <thread>
 using namespace std;
  
 #include <string.h>
 #include <sys/socket.h>
 #include <unistd.h>
 #include <errno.h>
  
 #include <linux/if_packet.h>
  
 #include "C_Net_Interface.hpp"

 #include <sigc++/sigc++.h>
  
//////////////////////////////////////////////////////////////////////////////////

 const int C_NET_RAW_READY  = 0x01;
 const int C_NET_RAW_ERROR  = 0x00;

//////////////////////////////////////////////////////////////////////////////////

 class C_Net_Raw {
     
    public: 

       C_Net_Raw();  
      ~C_Net_Raw();  

       int open(const S_Net_Interface* pSInterface);
       int close();

       int send(unsigned char* pData, unsigned int cData);

       // Non Blocking
       int start(int id, unsigned char* pBuffer, unsigned int cBuffer);
       int stop();

       ///////////////////////////////
       // Signal
       typedef sigc::signal<void, int, int> type_signal_data;
       type_signal_data signal_data();

    protected:
        
       type_signal_data m_signal_data;

    private:

       struct sockaddr_ll socket_address;

       int  sockfd, id;

       bool bOpen, bRun;

       // ((( RECIVE THREAD )))
       thread m_thread;
       void run();

       /////////////////
         
       unsigned char* pBuffer;
       unsigned int   cBuffer;
 };

#endif // _C_NET_RAW_H_
