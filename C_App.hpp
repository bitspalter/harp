//////////////////////////////////////////////////////////////////////////////////
// [ APP_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// harp ver:0.2
//
// [::Last modi: 28.03.21 L.ey (µ~)::]
//
//
#ifndef _C_APP_H_
 #define _C_APP_H_

 #include <iostream>
 #include <thread>
  
 using namespace std;
 
 #include <sys/capability.h>

 #include "C_GUI.hpp"
 #include "C_Net.hpp"
 
//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
 const int C_APP_ERROR = 0x00;
 const int C_APP_READY = 0x01;

//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////

 class C_App : public C_GUI { 
    
    public:
        
       C_App();
       virtual ~C_App(){};

    protected:

       static bool SortByIp(const S_host_Result& lhs, const S_host_Result& rhs){
          return *((DWORD*)lhs.IP) < *((DWORD*)rhs.IP);
       }

       //////////////////////////////////
       //Signal handlers:
       
       void on_button_start();
       void on_button_stop();
       void on_button_spoof();
       
       void on_packet();
       void on_finish();

       void on_device_changed();
       
       /////////////////////////////////
       
       C_Net CNet;

       /////////////////////////////////
       
       // ((( THREAD )))
       thread* pWorker = {nullptr};
       void scann();
       
       /////////////////////////////////
       const S_Net_Interface* TpSInterface = {nullptr};
       
       int TcPackets = {0};
       int TcSleep   = {0};
 };

#endif // _C_APP_H_
