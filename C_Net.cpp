//////////////////////////////////////////////////////////////////////////////////
// [ Net_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_Net.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [ init ]
//////////////////////////////////////////////////////////////////////////////////
int C_Net::init(){

   if(CNInterface.create() != C_NET_INTERFACE_READY){
      cout << "ERROR: CNInterface.create" << endl;
      return(C_NET_ERROR);
   }
   
   CNRaw.signal_data().connect(sigc::mem_fun(*this, &C_Net::on_raw_data));

   return(C_NET_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [ start ]
//////////////////////////////////////////////////////////////////////////////////
int C_Net::start(const S_Net_Interface* pSInterface, int cPackets, int cSleep){
   
   if(!pSInterface || bscann) return(C_NET_ERROR);

   //////////////////////////////////
   
   memcpy(IP_SOURCE,  &pSInterface->dw_IP,      SIZE_IP);
   memcpy(IP_REQUEST, &pSInterface->dw_Network, SIZE_IP);
   
   memcpy(MAC_SOURCE,  pSInterface->uc_MAC, SIZE_MAC);
   memcpy(MAC_ETH_S,   pSInterface->uc_MAC, SIZE_MAC);
   
   memcpy(MAC_REQUEST, MAC_BROADCAST, SIZE_MAC);
   memcpy(MAC_ETH_D,   MAC_BROADCAST, SIZE_MAC);
   
   //////////////////////////////////
   
   sArp.Operation  = ARP_REQUEST;
   
   sArp.pETH_MAC_D = MAC_ETH_D;
   sArp.pETH_MAC_S = MAC_ETH_S;
   
   sArp.pARP_MAC_D = MAC_REQUEST;
   sArp.pARP_MAC_S = MAC_SOURCE;
   
   sArp.pARP_IP_D  = IP_REQUEST;
   sArp.pARP_IP_S  = IP_SOURCE;

   CNArp.getPacket(pData, &cData, &sArp);
   
   ARP_HEADER* pRCV_arp = (ARP_HEADER*)(pData + cETHERNET_HEADER);

   //////////////////////////////////
   
   CDA_Result.clear();

   //////////////////////////////////
   
   if(CNRaw.open(pSInterface) != C_NET_RAW_READY)                      return(C_NET_ERROR);
   if(CNRaw.start(C_NET_ID, pBuffer, C_NET_BUFFER) != C_NET_RAW_READY) return(C_NET_ERROR);
   
   bscann = true;
   
   for(int n = 1; n < 255; n++){
   
      pRCV_arp->ARP_IP_D[3] = n;

      for(int n = 0; n < cPackets; n++){
         CNRaw.send(pData, cData);
         usleep(cSleep * 1000);
      } 
      
      if(!bscann){
         CNRaw.stop();
         CNRaw.close();
         sig_finish();
         return(C_NET_READY);
      }
   }
   
   CNRaw.stop();
   CNRaw.close();

   sig_finish();

   return(C_NET_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [ stop ]
//////////////////////////////////////////////////////////////////////////////////
int C_Net::stop(){
   
   if(!bscann) return(C_NET_ERROR);
   
   bscann = false;

   return(C_NET_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_raw_data ]
//////////////////////////////////////////////////////////////////////////////////
void C_Net::on_raw_data(int id, int cData){
   
   if(cData < (int)(cETHERNET_HEADER)) return;

   ETHERNET_HEADER* pRCV_ethhdr = (ETHERNET_HEADER*)pBuffer;
   
   switch(pRCV_ethhdr->Type){
      
      case ETH_TYP_ARP:{
        
         if(cData < (int)(cETHERNET_HEADER + cARP_HEADER)) return;
         
         ARP_HEADER* pRCV_arp = (ARP_HEADER*)(pBuffer + cETHERNET_HEADER);

         if(pRCV_arp->ARP_OpCode != ARP_RESPONSE || 
            *((DWORD*)pRCV_arp->ARP_MAC_D)      != *((DWORD*)MAC_SOURCE) || 
             *((WORD*)pRCV_arp->ARP_MAC_D + 2)  !=  *((WORD*)MAC_SOURCE + 2)) return;
         
         *((DWORD*)SResult.MAC)    = *((DWORD*)pRCV_arp->ARP_MAC_S);
         *((WORD*)SResult.MAC + 2) = *((WORD*)pRCV_arp->ARP_MAC_S + 2);

         *((DWORD*)SResult.IP) = *((DWORD*)pRCV_arp->ARP_IP_S);
         
         CDA_Result.push_back(SResult);
         
         sig_packet();
         
         break;
      }
   }
}
