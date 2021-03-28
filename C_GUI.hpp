//////////////////////////////////////////////////////////////////////////////////
// [ C_GUI_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
//
// [::Last modi: 28.03.21 L.ey (µ~)::]
//
//
#ifndef _C_GUI_H_
#define _C_GUI_H_

 #include <iostream>

 using namespace std;
 
 #include <gtkmm.h>
 
 #include "C_TreeView.hpp"
 #include "C_Edit_IP.hpp"
 #include "C_Edit_MAC.hpp"
 
//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
 const int C_GUI_ERROR = 0x00;
 const int C_GUI_READY = 0x01;
 
//////////////////////////////////////////////////////////////////////////////////
// STRUCT
//////////////////////////////////////////////////////////////////////////////////
 
 struct SBox {
    Gtk::Box button;
 };
 
 //////////////////////////////
 
 struct SButton {
    Gtk::Button start;
    Gtk::Button stop;
    Gtk::CheckButton spoof;
 };
 
 //////////////////////////////
 
 struct SLabel {
    Gtk::Label mac;
 };
 
 //////////////////////////////
 
 struct SCombo {
    Gtk::ComboBoxText interface;
    Gtk::ComboBoxText count;
    Gtk::ComboBoxText sleep;
 };
 
 //////////////////////////////
 
 struct STreeView {
    Gtk::ScrolledWindow window;    
    C_TreeView view; 
 };
 
 struct STView {
    STreeView host;
    STreeView packet;
 };
 
 //////////////////////////////
 
 struct SPaned {
    Gtk::Paned ph{Gtk::ORIENTATION_HORIZONTAL};  
 };
 
//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////
 
 class C_GUI : public Gtk::Window {
 
    public:
      
       C_GUI();
       virtual ~C_GUI(){};

    protected:
       
       Gtk::HeaderBar header; 

       //////////////////////////////////
       
       SBox     sbox;
       SButton  sbutton;
       SLabel   slabel;
       SCombo   scombo;
       STView   stview;
       SPaned   spaned;
       
       //////////////////////////////////
       
       C_Edit_IP  EIpS;
       C_Edit_MAC EMacS;
       
       //////////////////////////////////
       
       bool on_configure_changed(GdkEventConfigure* configure_event);
 };
 
#endif // _C_GUI_H_
