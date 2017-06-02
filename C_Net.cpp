//////////////////////////////////////////////////////////////////////////////////
// [ Net_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_Net.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [ start ]
//////////////////////////////////////////////////////////////////////////////////
int C_Net::start(){

   if(CNInterface.create() != C_NET_INTERFACE_READY){
      cout << "ERROR: CNInterface.create" << endl;
      return(C_NET_ERROR);
   }

   CA_Arp.create(1, 5000);
   
   return(C_NET_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [ send ]
//////////////////////////////////////////////////////////////////////////////////
int C_Net::send(S_Net_Interface* pSInterface, int cPackets, int cSleep){
  
   if(!pSInterface) return(C_NET_ERROR);

   UCHAR  pData[5000];
   DWORD  cData = 0;
   
   UCHAR IP_REQUEST[SIZE_IP];
   UCHAR IP_SOURCE[SIZE_IP];
	
   UCHAR MAC_REQUEST[SIZE_MAC];
   UCHAR MAC_SOURCE[SIZE_MAC];
   
   UCHAR MAC_ETH_D[SIZE_MAC];
   UCHAR MAC_ETH_S[SIZE_MAC];
   
   S_ArpPacket sArp;
   
   ////////////////////////////////////
   
   memcpy(IP_SOURCE,  &pSInterface->_dw_IP,      SIZE_IP);
   memcpy(IP_REQUEST, &pSInterface->_dw_Network, SIZE_IP);
   
   memcpy(MAC_SOURCE,  pSInterface->_uc_MAC, SIZE_MAC);
   memcpy(MAC_ETH_S,   pSInterface->_uc_MAC, SIZE_MAC);
   
   memcpy(MAC_REQUEST, MAC_BROADCAST, SIZE_MAC);
   memcpy(MAC_ETH_D,   MAC_BROADCAST, SIZE_MAC);
   
   ////////////////////////////////////
   
   sArp.Operation  = ARP_REQUEST;
   
   sArp.pETH_MAC_D = MAC_ETH_D;
   sArp.pETH_MAC_S = MAC_ETH_S;
   
   sArp.pARP_MAC_D = MAC_REQUEST;
   sArp.pARP_MAC_S = MAC_SOURCE;
   
   sArp.pARP_IP_D  = IP_REQUEST;
   sArp.pARP_IP_S  = IP_SOURCE;

   CNArp.getPacket(pData, &cData, &sArp);
   
   ETHERNET_HEADER* pRCV_ethhdr = (ETHERNET_HEADER*)pData;
   ARP_HEADER*      pRCV_arp    = (ARP_HEADER*)(pData + cETHERNET_HEADER);
      
   CDA_Result.clear();
   
   CNRaw.open(pSInterface);
   CNRaw.start(C_NET_ID_ARP, &CA_Arp);

   
   for(int n = 1; n < 255; n++){
   
      pRCV_arp->ARP_IP_D[3] = n;
   
      CNRaw.send(pData, cData);
   
      for(int n = 0; n < cPackets; n++){
        CNRaw.send(pData, cData);
        usleep(cSleep * 1000);
      }
  
      usleep(10000);
   }
   
   usleep(20000);
   
   CNRaw.stop();
   CNRaw.close();
   
   ///////////////////////
   
   sig_arp_data();

   return(C_NET_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_arp_data ]
//////////////////////////////////////////////////////////////////////////////////
void C_Net::on_arp_data(int id, int cData){
   if(id == C_NET_ID_ARP){
     
      if(cData < cETHERNET_HEADER + cARP_HEADER) return;
      
      UCHAR* pBuffer = (UCHAR*)CA_Arp.getpBuffer();
      
      ETHERNET_HEADER* pRCV_ethhdr = (ETHERNET_HEADER*)pBuffer;
      ARP_HEADER*      pRCV_arp    = (ARP_HEADER*)(pBuffer + cETHERNET_HEADER);
   
      if(pRCV_ethhdr->Type == ETH_TYP_ARP){
         if(pRCV_arp->ARP_OpCode == ARP_RESPONSE){
            C_Array* pCA_Response = 0;
            if(!(pCA_Response = CDA_Result.addItem(1, sizeof(S_Arp_Result)))) return;

            S_Arp_Result* pSArpResult = 0;
            if(!(pSArpResult = (S_Arp_Result*)pCA_Response->getpBuffer())) return;

            memcpy(pSArpResult->MAC, pRCV_arp->ARP_MAC_S, SIZE_MAC);
            memcpy(pSArpResult->IP,  pRCV_arp->ARP_IP_S,  SIZE_IP);
         }
      }
   }
}