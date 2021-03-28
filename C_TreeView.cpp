//////////////////////////////////////////////////////////////////////////////////
// [ TreeView_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_TreeView.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [ Konstructor ]
//////////////////////////////////////////////////////////////////////////////////
C_TreeView::C_TreeView(){
  
   //Create the Tree model:
   LStore = Gtk::ListStore::create(Columns);
   set_model(LStore);

   Pango::FontDescription fdsc("Courier New 12");
   
   override_font(fdsc);
   
   //Add the TreeView's view columns:
   //This number will be shown with the default numeric formatting.
   append_column("Nr",  Columns.nr);
   append_column("IP",  Columns.ip);
   append_column("MAC", Columns.mac);

   ////////////////////////////////////////
   
   //Make all the columns reorderable:
   //This is not necessary, but it's nice to show the feature.
   //You can use TreeView::set_column_drag_function() to more
   //finely control column drag and drop.
   for(guint i = 0; i < 3; i++){
      auto column = get_column(i);
      column->set_reorderable();
      column->set_resizable(true);
      
      column->set_expand(true);
   }
   
   ////////////////////////
   //Fill popup menu:
   
   auto item = Gtk::manage(new Gtk::MenuItem("_save", true));
   item->signal_activate().connect(sigc::mem_fun(*this, &C_TreeView::on_popup_save));
   Popup.append(*item);
   
   Popup.accelerate(*this);
   Popup.show_all(); //Show all menu items when the menu pops up
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_button_press_event ]
//////////////////////////////////////////////////////////////////////////////////
bool C_TreeView::on_button_press_event(GdkEventButton* button_event){
   bool bReturn = false;

   //Call base class, to allow normal handling,
   //such as allowing the row to be selected by the right-click:
   bReturn = TreeView::on_button_press_event(button_event);

   //Then do our custom stuff:
   if((button_event->type == GDK_BUTTON_PRESS) && (button_event->button == 3)){
      Popup.popup(button_event->button, button_event->time);
   }

   return(bReturn);
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_popup_save ]
//////////////////////////////////////////////////////////////////////////////////
void C_TreeView::on_popup_save(){

   fstream fOutput;

   ////////////////////////////////////7
   
   Gtk::FileChooserDialog dialog("Please choose a file", Gtk::FILE_CHOOSER_ACTION_SAVE);
   dialog.set_transient_for(*this->pWindow);

   dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
   dialog.add_button("_Open",   Gtk::RESPONSE_OK);

   int result = dialog.run();

   switch(result){
      case(Gtk::RESPONSE_OK): {

         string filename = dialog.get_filename();

         fOutput.open(filename.c_str(), ios_base::out | ios_base::binary); 

         if(!fOutput.is_open()){
            cout << "Error: open file - " << filename << endl;
            return;
         }

         typedef Gtk::TreeModel::Children type_children; //minimise code length.
         type_children children = LStore->children();
   
         int nRow = 0;
   
         for(type_children::iterator iter = children.begin(); iter != children.end(); ++iter){
            fOutput << setw(3)  << nRow++ << "  " << left 
                    << setw(15) << (*iter)[Columns.ip]  << "  " << right 
                    << setw(17) << (*iter)[Columns.mac] << endl;
         }

         fOutput.close();

         break;
      }
      case(Gtk::RESPONSE_CANCEL): break;
      default: break;
   }
}
