//////////////////////////////////////////////////////////////////////////////////
// [ APP_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_App.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [ Konstructor ]  
//////////////////////////////////////////////////////////////////////////////////
C_App::C_App(){

   if(geteuid()){

      cap_flag_value_t cap_flag_value;  
      cap_t caps = cap_get_proc();
      
      if(caps){
         
         if(cap_get_flag(caps, CAP_NET_RAW, CAP_EFFECTIVE, &cap_flag_value) == 0){
         
            if(cap_flag_value != CAP_SET){
               
               Gtk::MessageDialog dialog(*this, 
                                         "You have to be ROOT or need CAP_NET_RAW capabilities!", 
                                         false, Gtk::MESSAGE_ERROR);
               dialog.run();
               
               cap_free(caps);
               exit(EXIT_FAILURE);
            }
         }

         cap_free(caps);
      }
   }

   /////////////////////////////////////////////
   
   if(CNet.init() != C_NET_READY){
      cout << "ERROR: CNet.init" << endl;
      Gtk::MessageDialog dialog(*this, "Network Error", false, Gtk::MESSAGE_ERROR);
      dialog.run();
      return;
   }
   
   /////////////////////////////////////////////
   //Fill the interface combo:
   const vector<S_Net_Interface>* pDA_Interface = CNet.CNInterface.get_pDA_Interface();

   for(auto iface : *pDA_Interface) scombo.interface.append(iface.ps_Name);

   int nInterface = CNet.CNInterface.get_First_Active();
   
   if(nInterface == -1){
      scombo.interface.set_sensitive(false);
      sbutton.start.set_sensitive(false);
      Gtk::MessageDialog dialog(*this, "no active network interface", false, Gtk::MESSAGE_ERROR);
      dialog.run();
   }else{
      scombo.interface.set_active(nInterface);
   }

   scombo.interface.signal_changed().connect(sigc::mem_fun(*this, &C_App::on_device_changed));
   
   //////////////////////////////////////////////////////////////
   //Fill the Ip and MAC Edit:

   const S_Net_Interface* pSInterface = 0;
   
   pSInterface = CNet.CNInterface.get_pInterface(scombo.interface.get_active_row_number());

   EMacS.setMAC(pSInterface->uc_MAC[0],
                pSInterface->uc_MAC[1],
                pSInterface->uc_MAC[2],
                pSInterface->uc_MAC[3],
                pSInterface->uc_MAC[4],
                pSInterface->uc_MAC[5]);
   
   EIpS.setIP(pSInterface->ps_IP);
   
   //////////////////////////////////////////////////////////////
   // Button

   sbutton.start.signal_clicked().connect(sigc::mem_fun(*this, &C_App::on_button_start));
   sbutton.stop.signal_clicked().connect(sigc::mem_fun(*this, &C_App::on_button_stop));

   sbutton.spoof.signal_clicked().connect(sigc::mem_fun(*this, &C_App::on_button_spoof));

   /////////////////////////////////////////////
   // net signal

   CNet.sig_packet.connect(sigc::mem_fun(*this, &C_App::on_packet));
   CNet.sig_finish.connect(sigc::mem_fun(*this, &C_App::on_finish));
   
   ////////////////////////
   
   show_all_children();
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_button_start ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_button_start(){

   const S_Net_Interface* pSInterface = 0;
   
   pSInterface = CNet.CNInterface.get_pInterface(scombo.interface.get_active_row_number());
   
   if(pSInterface == nullptr){
      cout << "Error get_pInterface:" << scombo.interface.get_active_row_number() << endl;  
      return; 
   }
   
   //////////////////////////////

   stview.host.view.LStore->clear();
   stview.packet.view.LStore->clear();
   
   //////////////////////////////
   
   int cPackets = 0;
   
   int type = scombo.count.get_active_row_number();
   
   switch(type){
      case 0: cPackets = 1; break; 
      case 1: cPackets = 3; break;
      case 2: cPackets = 5; break;
   }
   
   int cSleep = 0;
   
   type = scombo.sleep.get_active_row_number();
   
   switch(type){
      case 0: cSleep = 10;  break; 
      case 1: cSleep = 50;  break;
      case 2: cSleep = 500; break;
   }

   //////////////////////////////
   
   UCHAR IP_SOURCE[SIZE_IP];
   UCHAR MAC_SOURCE[SIZE_MAC];
   
   if(sbutton.spoof.get_active()){
      EMacS.getMAC(MAC_SOURCE);
      EIpS.getIP(IP_SOURCE);
      
      memcpy((void*)&pSInterface->dw_IP, IP_SOURCE,  SIZE_IP);
      memcpy((void*)pSInterface->uc_MAC, MAC_SOURCE, SIZE_MAC);
   }

   //////////////////////////////
   
   TpSInterface = pSInterface;
   TcPackets    = cPackets;
   TcSleep      = cSleep;

   CNet.stop();
   
   pWorker = new std::thread([this]{this->scann();});

   this->set_title("scanning ... ");

   //////////////////////////////

   sbutton.stop.set_sensitive(true);
   sbutton.start.set_sensitive(false);
   scombo.interface.set_sensitive(false);
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_button_stop ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_button_stop(){
   CNet.stop();
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_button_spoof ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_button_spoof(){
   
   if(sbutton.spoof.get_active()){
      EIpS.enable();
      EMacS.enable();
   }else{
      EIpS.disable();
      EMacS.disable();
   }
}
//////////////////////////////////////////////////////////////////////////////////
// [ scann ] (THREAD)
//////////////////////////////////////////////////////////////////////////////////
void C_App::scann(){
   
   CNet.start(TpSInterface, TcPackets, TcSleep);

   sbutton.stop.set_sensitive(false);
   sbutton.start.set_sensitive(true);
   scombo.interface.set_sensitive(true);
   
   this->set_title("harp 0.2");
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_packet ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_packet(){

   if(CNet.CDA_Result.empty()) return;
   
   auto iTRes = CNet.CDA_Result.back();
    
   S_host_Result* iTResult = &iTRes;
   
   char pItemIP[256];

   sprintf(pItemIP, "%d.%d.%d.%d", 
          (int)iTResult->IP[0], (int)iTResult->IP[1], 
          (int)iTResult->IP[2], (int)iTResult->IP[3]);

   char pItemMAC[256];

   sprintf(pItemMAC, "%02X:%02X:%02X:%02X:%02X:%02X", 
          (int)iTResult->MAC[0], (int)iTResult->MAC[1], 
          (int)iTResult->MAC[2], (int)iTResult->MAC[3],
          (int)iTResult->MAC[4], (int)iTResult->MAC[5]);

   ///////////////////////////////////////////////////

   Gtk::TreeModel::iterator iter = stview.packet.view.LStore->append();
   Gtk::TreeModel::Row row       = *iter;

   row.set_value(0, stview.packet.view.LStore->children().size());
   row.set_value(1, (Glib::ustring)pItemIP);
   row.set_value(2, (Glib::ustring)pItemMAC);

   // autoscroll
   Gtk::TreeModel::Path path = stview.packet.view.LStore->get_path(iter);
   stview.packet.view.scroll_to_row(path);
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_finish ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_finish(){
   list<S_host_Result> CDA_TResult; 
   list<S_host_Result>::iterator iResult;
   list<S_host_Result>::iterator iTResult;
   bool bInsert = true;
   
   ///////////////////////

   if(pWorker->joinable())
      pWorker->join();
   delete pWorker;
   pWorker = nullptr;
      
   ///////////////////////////////////////////////////////
   
   for(iResult = CNet.CDA_Result.begin(); iResult != CNet.CDA_Result.end(); ++iResult){
      
      if(*((DWORD*)iResult->IP) == 0 || (*((DWORD*)iResult->IP) & 0x00FFFFFF) != TpSInterface->dw_Network)
         continue;
      
      bInsert = true;

      for(iTResult = CDA_TResult.begin(); iTResult != CDA_TResult.end(); ++iTResult){

         if(*((DWORD*)iResult->IP) == *((DWORD*)iTResult->IP)){
            bInsert = false;
            break;
         }
      }
      
      if(bInsert) CDA_TResult.push_back(*iResult);
   }

   ///////////////////////////////////////////////////////

   CDA_TResult.sort(this->SortByIp);
   
   for(iTResult = CDA_TResult.begin(); iTResult != CDA_TResult.end(); ++iTResult){

      char pItemIP[256];

      sprintf(pItemIP, "%d.%d.%d.%d", 
             (int)iTResult->IP[0], (int)iTResult->IP[1], 
             (int)iTResult->IP[2], (int)iTResult->IP[3]);

      char pItemMAC[256];

      sprintf(pItemMAC, "%02X:%02X:%02X:%02X:%02X:%02X", 
             (int)iTResult->MAC[0], (int)iTResult->MAC[1], 
             (int)iTResult->MAC[2], (int)iTResult->MAC[3],
             (int)iTResult->MAC[4], (int)iTResult->MAC[5]);

      ///////////////////////////////////////////////////

      Gtk::TreeModel::iterator iter = stview.host.view.LStore->append();
      Gtk::TreeModel::Row row       = *iter;

      row.set_value(0, stview.host.view.LStore->children().size());
      row.set_value(1, (Glib::ustring)pItemIP);
      row.set_value(2, (Glib::ustring)pItemMAC);

      // autoscroll
      Gtk::TreeModel::Path path = stview.host.view.LStore->get_path(iter);
      stview.host.view.scroll_to_row(path);
   }
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_device_changed ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_device_changed(){
   
   const S_Net_Interface* pSInterface = nullptr;
   
   pSInterface = CNet.CNInterface.get_pInterface(scombo.interface.get_active_row_number());

   if(!pSInterface->uc_Active || pSInterface->uc_LoopBack) 
      sbutton.start.set_sensitive(false);
   else 
      sbutton.start.set_sensitive(true);
}
