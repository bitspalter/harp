//////////////////////////////////////////////////////////////////////////////////
// [ APP_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// APP
//
// [::Last modi: 24.03.17 L.ey (µ~)::]
//
//
#ifndef _C_APP_H_
 #define _C_APP_H_

 #include <iostream>
 
 using namespace std;
 
 #include <gtkmm/window.h>
 #include <gtkmm/button.h>
 #include <gtkmm/comboboxtext.h>
 #include <gtkmm/scrolledwindow.h>
 #include <gtkmm/layout.h>
 
 #include "C_TreeView.hpp"
 
 #include "C_Net.hpp"

//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
 const int C_APP_ERROR = 0x00;
 const int C_APP_READY = 0x01;
   
//////////////////////////////////////////////////////////////////////////////////
// STRUCT
//////////////////////////////////////////////////////////////////////////////////

 struct SButton {
    Gtk::Button send;
 };
 
 //////////////////////////////
 
 struct SCombo {
    Gtk::ComboBoxText interface;
    Gtk::ComboBoxText count;
    Gtk::ComboBoxText sleep;
 };
 
//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////
 
 class C_App : public Gtk::Window {
  
    public:
      
       C_App();
       virtual ~C_App();
       
       void on_arp_data();

    protected:

       Gtk::Layout m_layout;
       
       Gtk::ScrolledWindow m_ScrolledWindow;
       
       C_TreeView m_TreeView;
       
       SButton sbutton;
       SCombo  scombo;
       
       ////////////////////////
      
       //Signal handlers:
       void on_button_send();
       bool on_configure_changed(GdkEventConfigure* configure_event);
      
       ////////////////////////
      
       C_Net CNet;
 };

#endif // _C_APP_H_