//////////////////////////////////////////////////////////////////////////////////
// [ C_GUI_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_GUI.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [ Konstructor ]  
//////////////////////////////////////////////////////////////////////////////////
C_GUI::C_GUI(){

   this->set_default_size(1100, 600);
   
   ////////////////////////
   
   signal_configure_event().connect(sigc::mem_fun(*this, &C_GUI::on_configure_changed), false);
   
   /////////////////////////////////////////////////
   // Button
   sbutton.start.set_image_from_icon_name("media-playback-start");
   sbutton.start.set_size_request(50, 30);
   
   sbutton.stop.set_image_from_icon_name("media-playback-stop");
   sbutton.stop.set_size_request(50, 30);
   sbutton.stop.set_sensitive(false);

   sbox.button.pack_start(sbutton.start);
   sbox.button.pack_start(sbutton.stop);

   auto context = sbox.button.get_style_context();
   
   context->add_class(GTK_STYLE_CLASS_LINKED);
   
   sbutton.spoof.set_label(" spoof ip: ");
   
   /////////////////////////////////////////////////
   // Label
   slabel.mac.set_label(" mac: ");
   
   /////////////////////////////////////////////////
   // scombo
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
   
   /////////////////////////////////////////////////
   // Edit
   EIpS.setSize(3, 30);
   EIpS.disable();
   
   EMacS.setSize(3, 30);
   EMacS.disable();
   
   /////////////////////////////////////////////////
   // TreeView
   
   stview.host.view.setWindow(this);
   stview.host.window.add(stview.host.view);
   stview.host.window.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
   
   auto column = stview.host.view.get_column(0);
   column->set_title("Host");
   
   stview.packet.view.setWindow(this);
   stview.packet.window.add(stview.packet.view);
   stview.packet.window.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

   column = stview.packet.view.get_column(0);
   column->set_title("Packet");
   
   /////////////////////////////////////////////////
   // Paned
   spaned.ph.add1(stview.packet.window);
   spaned.ph.add2(stview.host.window);
   
   spaned.ph.set_position(300);
   
   //////////////////////////////////////////////////////////////
   // HeaderBar
   set_titlebar(header);
   
   header.set_title("harp 0.2");
   header.set_subtitle("Welcome");
   header.set_show_close_button();
   header.set_property("spacing", 1);
   
   header.pack_start(scombo.interface);
   header.pack_start(scombo.count);
   header.pack_start(scombo.sleep);

   header.pack_start(sbox.button);
   
   header.pack_end(EMacS.aBox);
   header.pack_end(slabel.mac);
   header.pack_end(EIpS.aBox);
   header.pack_end(sbutton.spoof);

   ///////////////////////////////////////////////////////
   add(spaned.ph);
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_configure_changed ]
//////////////////////////////////////////////////////////////////////////////////
bool C_GUI::on_configure_changed(GdkEventConfigure* configure_event){

   if(configure_event->send_event) return(false);
   
   if(!this->is_maximized()){
      spaned.ph.set_position(configure_event->width / 2 - 20);
   }else{
      spaned.ph.set_position(configure_event->width / 2);
   }

   return(false);
}
