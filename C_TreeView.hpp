//////////////////////////////////////////////////////////////////////////////////
// [ TreeView_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
//
// [::Last modi: 19.03.17 L.ey (µ~)::]
//
//
#ifndef _C_TREEVIEW_H_
 #define _C_TREEVIEW_H_

 #include <iostream>
 #include <fstream>
 #include <iomanip>
 
 using namespace std;
 
 #include <gtkmm/treeview.h>
 #include <gtkmm/liststore.h>
 #include <gtkmm/cellrenderertext.h>

 #include <gtkmm/filechooserdialog.h>
 #include <gtkmm/window.h>
 
//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
 const int C_TREEVIEW_ERROR = 0x00;
 const int C_TREEVIEW_READY = 0x01;
   
//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////
   
class C_TreeView : public Gtk::TreeView {
   
   public:
      C_TreeView();
      virtual ~C_TreeView(){};

      void setWindow(Gtk::Window* pWindow){this->pWindow = pWindow;};
      
      ////////////////////////
      //Tree model columns:
      class ModelColumns : public Gtk::TreeModel::ColumnRecord {
         public:

            ModelColumns(){add(nr); add(ip); add(mac);}

            Gtk::TreeModelColumn<unsigned int>  nr;
            Gtk::TreeModelColumn<Glib::ustring> ip;
            Gtk::TreeModelColumn<Glib::ustring> mac;
            Gtk::TreeModelColumn<int>           hidden;
      };

      ModelColumns Columns;
      
      /////////////////////////
      
      Glib::RefPtr<Gtk::ListStore> LStore;
      
      /////////////////////////

      Gtk::Menu Popup;
      Gtk::Menu Popup_Save;

      // Override Signal handler:
      // Alternatively, use signal_button_press_event().connect_notify()
      bool on_button_press_event(GdkEventButton* button_event) override;
      
      //Signal handler for popup menu items:
      void on_popup_save();
      
   private:
  
      Gtk::Window* pWindow; // for FileChooserDialog
};
 
#endif // _C_TREEVIEW_H_
