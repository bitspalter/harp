//////////////////////////////////////////////////////////////////////////////////
// [ Raw_Socket_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_Net_Raw.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [Konstructor]  
//////////////////////////////////////////////////////////////////////////////////
C_Net_Raw::C_Net_Raw(){
   bOpen = false;
   bRun  = false;
}
//////////////////////////////////////////////////////////////////////////////////
// [Destructor]  
//////////////////////////////////////////////////////////////////////////////////
C_Net_Raw::~C_Net_Raw(){
}
//////////////////////////////////////////////////////////////////////////////////
// [open]  
//////////////////////////////////////////////////////////////////////////////////
int C_Net_Raw::open(const S_Net_Interface* pSInterface){

   if(bOpen || !pSInterface) return(C_NET_RAW_READY);
  
   memset(&socket_address, 0, sizeof(struct sockaddr_ll));
   
   socket_address.sll_ifindex = pSInterface->dw_index;
   socket_address.sll_halen   = 6;
   
   if((sockfd = socket(AF_PACKET, SOCK_RAW, 0x0300)) == -1){
      cout << "ERROR: socket " << strerror(errno) << endl;
      return(C_NET_RAW_ERROR);
   }
   
   bOpen = true;

   ////////////////////////////
   struct ifreq ifopts; /* set promiscuous mode */
   
   /* Set interface to promiscuous mode - do we need to do this every time? */
   strncpy(ifopts.ifr_name, pSInterface->ps_Name, IFNAMSIZ - 1);
   ioctl(sockfd, SIOCGIFFLAGS, &ifopts);
   ifopts.ifr_flags |= IFF_PROMISC;
   ioctl(sockfd, SIOCSIFFLAGS, &ifopts);

   return(C_NET_RAW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [shutdown]  
//////////////////////////////////////////////////////////////////////////////////
int C_Net_Raw::close(){
  
   if(bOpen){
      if(bRun) bRun = false;
      ::close(sockfd);
      bOpen = false;
   }
   
   return(C_NET_RAW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [send]  
//////////////////////////////////////////////////////////////////////////////////
int C_Net_Raw::send(unsigned char* pData, unsigned int cData){
  
   if(bOpen){
      if(pData != 0 && cData > 0){
         if(sendto(sockfd, pData, cData, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll)) < 0){
            cout << "ERROR: send " << strerror(errno) << endl;
            return(C_NET_RAW_ERROR);
         }
      }
   }else return(C_NET_RAW_ERROR);
   
   return(C_NET_RAW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [start]  
//////////////////////////////////////////////////////////////////////////////////
int C_Net_Raw::start(int idEx, unsigned char* pBuf, unsigned int cBuf){
  
   if(bRun || !bOpen || !pBuf || !cBuf) return(C_NET_RAW_ERROR);

   ////////////
   pBuffer = pBuf;
   cBuffer = cBuf;
   id      = idEx;
   ////////////
   
   if(CThread.create(this, &C_Net_Raw::run) == C_THREAD_READY){
      bRun = true;
   }else return(C_NET_RAW_ERROR);
   
   return(C_NET_RAW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [stop]  
//////////////////////////////////////////////////////////////////////////////////
int C_Net_Raw::stop(){
  if(!bRun || !bOpen) return(C_NET_RAW_ERROR);
   CThread.terminate();
   bRun = false;
   return(C_NET_RAW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [recv]  
//////////////////////////////////////////////////////////////////////////////////
int C_Net_Raw::recv(){

   if(CThread.create(this, &C_Net_Raw::run) == C_THREAD_READY){
      bRun = true;
   }else return(C_NET_RAW_ERROR);
   
   return(C_NET_RAW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [run]  
//////////////////////////////////////////////////////////////////////////////////
void C_Net_Raw::run(){

   struct sockaddr sa;
   
   int cData  = 0;
   int sa_len = sizeof(sa);
   
   while(bRun){
      cData = recvfrom(sockfd, pBuffer, cBuffer, 0, &sa, (socklen_t*) &sa_len);
      if(cData > 0) m_signal_data.emit(id, cData);
   }
   
   return;
}
//////////////////////////////////////////////////////////////////////////////////
// [signal_data]
//////////////////////////////////////////////////////////////////////////////////
C_Net_Raw::type_signal_data C_Net_Raw::signal_data(){
   return(m_signal_data);
}
