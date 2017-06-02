//////////////////////////////////////////////////////////////////////////////////
// [ Raw_Arp_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_Net_Raw_Arp.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [getPacket]  
//////////////////////////////////////////////////////////////////////////////////
DWORD C_Net_Raw_Arp::getPacket(UCHAR* pData, DWORD* pcData, S_ArpPacket* pS_ArpPacket){

   if(!pData || !pcData || !pS_ArpPacket) return(C_NET_RAW_ARP_ERROR);

   ETHERNET_HEADER* _pETHERNET_HEADER = (ETHERNET_HEADER*)pData;
   ARP_HEADER*      _pARP_HEADER      = (ARP_HEADER*)(pData + cETHERNET_HEADER);

   ////////////////////////////////////////////////////////////////////////////
   // ETHERNET_HEADER
   _pETHERNET_HEADER->Type = ETH_TYP_ARP;

   *((DWORD*)_pETHERNET_HEADER->MAC_D) = *((DWORD*)pS_ArpPacket->pETH_MAC_D);
   *((DWORD*)_pETHERNET_HEADER->MAC_S) = *((DWORD*)pS_ArpPacket->pETH_MAC_S);

    *((WORD*)_pETHERNET_HEADER->MAC_D + 2) = *((WORD*)pS_ArpPacket->pETH_MAC_D + 2);
    *((WORD*)_pETHERNET_HEADER->MAC_S + 2) = *((WORD*)pS_ArpPacket->pETH_MAC_S + 2);

   ////////////////////////////////////////////////////////////////////////////
   // ARP_HEADER
   _pARP_HEADER->ARP_HType  = ARP_HTYP_ETH;
   _pARP_HEADER->ARP_PType  = ARP_PTYP_IP;

   _pARP_HEADER->ARP_cHAddr = ARP_MAC_LEN;
   _pARP_HEADER->ARP_cPAddr = ARP_PRO_LEN;

   _pARP_HEADER->ARP_OpCode = pS_ArpPacket->Operation;

   ////////////////////////////////////////////////////////////////////////////
   // ARP_MAC
   *((DWORD*)_pARP_HEADER->ARP_MAC_S) = *((DWORD*)pS_ArpPacket->pARP_MAC_S);
   *((DWORD*)_pARP_HEADER->ARP_MAC_D) = *((DWORD*)pS_ArpPacket->pARP_MAC_D);

    *((WORD*)_pARP_HEADER->ARP_MAC_S + 2) = *((WORD*)pS_ArpPacket->pARP_MAC_S + 2);
    *((WORD*)_pARP_HEADER->ARP_MAC_D + 2) = *((WORD*)pS_ArpPacket->pARP_MAC_D + 2);
   
   ////////////////////////////////////////////////////////////////////////////
   // ARP_IP
   *((DWORD*)_pARP_HEADER->ARP_IP_S) = *((DWORD*)pS_ArpPacket->pARP_IP_S);
   *((DWORD*)_pARP_HEADER->ARP_IP_D) = *((DWORD*)pS_ArpPacket->pARP_IP_D);

   ////////////////////////////////////////////////////////////////////////////
   // Return Size
   *pcData = cETHERNET_HEADER + cARP_HEADER;

   return(C_NET_RAW_ARP_READY);
}