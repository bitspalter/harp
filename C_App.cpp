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
   const vector<S_Net_Interface>* pDA_Interface = CNet.CNInterface.get_pDA_Interface();

   for(auto iface : *pDA_Interface) scombo.interface.append(iface.ps_Name);
   
   /////////////////////////////////////////////////////////////////////
   
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

   const S_Net_Interface* pSInterface = 0;
   
   pSInterface = CNet.CNInterface.get_pInterface(scombo.interface.get_active_row_number());
   
   if(pSInterface == nullptr){
      cout << "Error get_pInterface:" << scombo.interface.get_active_row_number() << endl;  
      return; 
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
   list<S_Arp_Result> CDA_TResult; 
   list<S_Arp_Result>::iterator iResult;
   list<S_Arp_Result>::iterator iTResult;
   bool bInsert = true;
  
   m_TreeView.m_refTreeModel->clear();
   
   for(iResult = CNet.CDA_Result.begin(); iResult != CNet.CDA_Result.end(); ++iResult){
      bInsert = true;   
      for(iTResult = CDA_TResult.begin(); iTResult != CDA_TResult.end(); ++iTResult){
         if(*((DWORD*)iResult->IP) == *((DWORD*)iTResult->IP)){
            bInsert = false;
            break;
         } 
      }
      
      if(bInsert) CDA_TResult.push_back(*iResult);   
   }
   
   /////////////////////////////////////////////////////////////////////////////////
   
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
