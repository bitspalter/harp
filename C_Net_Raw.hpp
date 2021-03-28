//////////////////////////////////////////////////////////////////////////////////
//[ RawSocket_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
//
// [::Raw Socket Class::]
//
// [::Last modi: 12.03.21 L.ey (µ~)::]  
//
//
#ifndef _C_NET_RAW_H_
 #define _C_NET_RAW_H_
 
 #include <cstdint>
 #include <iostream>
 #include <thread>
 using namespace std;
  
 //#include <string.h>
 #include <sys/socket.h>
 #include <unistd.h>
 #include <errno.h>
  
 #include <linux/if_packet.h>
  
 #include "C_Net_Interface.hpp"

 #include <sigc++/sigc++.h>
  
//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////

 const int C_NET_RAW_READY  = 0x01;
 const int C_NET_RAW_ERROR  = 0x00;

//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////

 class C_Net_Raw {
     
    public: 

       C_Net_Raw(){};  
      ~C_Net_Raw(){};  

       int open(const S_Net_Interface* pSInterface);
       int close();

       int send(uint8_t* pData, uint32_t cData);

       // Non Blocking
       int start(int id, uint8_t* pBuffer, uint32_t cBuffer);
       int stop();

       ///////////////////////////////
       // Signal
       typedef sigc::signal<void, int, int> type_signal_data;
       type_signal_data signal_data();

    protected:
        
       type_signal_data m_signal_data;

    private:

       struct sockaddr_ll socket_address;

       int32_t sockfd = {0};
       int32_t id     = {0};

       bool bOpen = {false};
       bool bRun  = {false};

       // ((( RECIVE THREAD )))
       thread m_thread;
       void run();

       /////////////////
         
       uint8_t*  pBuffer = {nullptr};
       uint32_t  cBuffer = {0};
 };

#endif // _C_NET_RAW_H_
