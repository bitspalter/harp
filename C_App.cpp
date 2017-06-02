//////////////////////////////////////////////////////////////////////////////////
// [ APP_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_App.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [ Konstructor ]  
//////////////////////////////////////////////////////////////////////////////////
C_App::C_App(){
  
   this->set_title("harp");
   this->resize(400, 500);

   /////////////////////////////////////////////
   
   if(CNet.start() != C_NET_READY){
      cout << "ERROR: CNet.start" << endl;
      return;
   }
   
   /////////////////////////////////////////////
   // signal
   CNet.CNRaw.signal_data().connect(sigc::mem_fun(CNet, &C_Net::on_arp_data));
   CNet.sig_arp_data.connect(sigc::mem_fun(this, &C_App::on_arp_data));
   
   /////////////////////////////////////////////
   // treeview
   
   m_TreeView.setWindow(this);
   
   //Add the TreeView, inside a ScrolledWindow, with the button underneath:
   m_ScrolledWindow.add(m_TreeView);
   
   //Only show the scrollbars when they are necessary:
   m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
   
   /////////////////////////////////////////////
   //Fill the interface combo:
   C_DArray*        pDA_Interface = 0;
   S_Net_Interface* pSInterface   = 0;
   
   if((pDA_Interface = CNet.CNInterface.get_pDA_Interface())){
      for(int n = 0; n < pDA_Interface->getnItems(); n++){
         S_C_DArray* pCA_F = pDA_Interface->getpItem(n);
         if((pSInterface = (S_Net_Interface*)pCA_F->pData->getpBuffer()))
	    scombo.interface.append(pSInterface->_ps_Name);
      }
   }else{
      cout << "ERROR: CNInterface.get_pDA_Interface" << endl;
      return;
   }
   
   scombo.interface.set_active(CNet.CNInterface.get_First_Active());
   scombo.interface.set_size_request(100, 30); 
   
   ////////////////
   scombo.count.set_size_request(100, 30);
   scombo.count.append("1 Packet");
   scombo.count.append("3 Packets");
   scombo.count.append("5 Packets");
   scombo.count.set_active(0);
   
   ////////////////
   scombo.sleep.set_size_request(100, 30);
   scombo.sleep.append("10 ms");
   scombo.sleep.append("50 ms");
   scombo.sleep.append("500 ms");
   scombo.sleep.set_active(0);
   
   //////////////////////////////////////////////////////////////
   // Button
   sbutton.send.set_label("send");
   sbutton.send.set_size_request(100, 30);
   sbutton.send.signal_clicked().connect(sigc::mem_fun(*this, &C_App::on_button_send));

   ///////////////////////////////////////////////////////
   // layout
   m_layout.put(sbutton.send,      100,  0);
   
   m_layout.put(scombo.interface,   0,  0);
   m_layout.put(scombo.count,     200,  0);
   m_layout.put(scombo.sleep,     300,  0);

   m_layout.put(m_ScrolledWindow, 0, 30);

   add(m_layout);
   
   ////////////////////////
   
   signal_configure_event().connect(sigc::mem_fun(*this, &C_App::on_configure_changed), false);
   
   ////////////////////////
   
   show_all_children();
}
//////////////////////////////////////////////////////////////////////////////////
// [ Destructor ]
//////////////////////////////////////////////////////////////////////////////////
C_App::~C_App(){
  
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_button_send ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_button_send(){
   C_DArray*        pDA_Interface = 0;
   S_Net_Interface* pSInterface   = 0;
   
   //////////////////////////////

   if((pDA_Interface = CNet.CNInterface.get_pDA_Interface())){
     
      int nInterface = scombo.interface.get_active_row_number();
     
      S_C_DArray* pCA_F = pDA_Interface->getpItem(nInterface);
     
      if(!(pSInterface = (S_Net_Interface*)pCA_F->pData->getpBuffer())){
         cout << "ERROR: pSInterface" << endl;
         return;
      }
   }
   
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
   
   CNet.send(pSInterface, cPackets, cSleep);
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_arp_data ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_arp_data(){
   C_DArray CDA_TResult;
   bool bInsert = true;
  
   m_TreeView.m_refTreeModel->clear();
   
   for(int n = 0; n < CNet.CDA_Result.getnItems(); n++){
      C_Array* pCA_Response = 0; 
      if(!(pCA_Response = CNet.CDA_Result.getpItempData(n))) continue;
      
      S_Arp_Result* pSArpResult = 0;    
      if(!(pSArpResult = (S_Arp_Result*)pCA_Response->getpBuffer())) continue;
      
      bInsert = true;
      
      for(int m = 0; m < CDA_TResult.getnItems(); m++){
         C_Array* pCA_TResponse = 0; 
         if(!(pCA_TResponse = CDA_TResult.getpItempData(m))) continue;
      
         S_Arp_Result* pSArpTResult = 0;
         if(!(pSArpTResult = (S_Arp_Result*)pCA_TResponse->getpBuffer())) continue;

         if(*((DWORD*)pSArpResult->IP) == *((DWORD*)pSArpTResult->IP)){
            bInsert = false;
            break;
         }
      }
      
      if(bInsert){
         C_Array* pCA_TResponse = 0; 
	 if(!(pCA_TResponse = CDA_TResult.addItem(1, sizeof(S_Arp_Result)))) continue;
	 
	 S_Arp_Result* pSArpTResult = 0;
	 if(!(pSArpTResult = (S_Arp_Result*)pCA_TResponse->getpBuffer())) return;
	 
	 memcpy(pSArpTResult, pSArpResult, sizeof(S_Arp_Result));
      }
   }
   
   /////////////////////////////////////////////////////////////
   
   for(int m = 0; m < CDA_TResult.getnItems(); m++){
      C_Array* pCA_TResponse = 0; 
      if(!(pCA_TResponse = CDA_TResult.getpItempData(m))) continue;
      
      S_Arp_Result* pSArpTResult = 0;
      if(!(pSArpTResult = (S_Arp_Result*)pCA_TResponse->getpBuffer())) continue;

      ////////////////////////////////////////////////
      
      char pItemIP[256];
      
      sprintf(pItemIP, "%d.%d.%d.%d", 
             (int)pSArpTResult->IP[0], (int)pSArpTResult->IP[1], 
             (int)pSArpTResult->IP[2], (int)pSArpTResult->IP[3]);
      
      char pItemMAC[256];
      
      sprintf(pItemMAC, "%02X:%02X:%02X:%02X:%02X:%02X", 
             (int)pSArpTResult->MAC[0], (int)pSArpTResult->MAC[1], 
             (int)pSArpTResult->MAC[2], (int)pSArpTResult->MAC[3],
             (int)pSArpTResult->MAC[4], (int)pSArpTResult->MAC[5]);
      
      ///////////////////////////////////////////////////

      Gtk::TreeModel::iterator iter = m_TreeView.m_refTreeModel->append();
      Gtk::TreeModel::Row row       = *iter;

      row.set_value(0, m_TreeView.m_refTreeModel->children().size());
      row.set_value(1, (Glib::ustring)pItemIP);
      row.set_value(2, (Glib::ustring)pItemMAC);
   
      // autoscroll
      Gtk::TreeModel::Path path = m_TreeView.m_refTreeModel->get_path(iter);
      m_TreeView.scroll_to_row(path);
   }
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_configure_changed ]
//////////////////////////////////////////////////////////////////////////////////
bool C_App::on_configure_changed(GdkEventConfigure* configure_event){
   m_ScrolledWindow.set_size_request(configure_event->width, configure_event->height - 30);
   return(false);
}