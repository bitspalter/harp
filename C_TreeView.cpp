//////////////////////////////////////////////////////////////////////////////////
// [ TreeView_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_TreeView.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [ Konstructor ]
//////////////////////////////////////////////////////////////////////////////////
C_TreeView::C_TreeView(){
  
   //Create the Tree model:
   m_refTreeModel = Gtk::ListStore::create(m_Columns);
   set_model(m_refTreeModel);
   
   //Add the TreeView's view columns:
   //This number will be shown with the default numeric formatting.
   append_column("Nr",  m_Columns.m_col_nr);
   append_column("IP",  m_Columns.m_col_ip);
   append_column("MAC", m_Columns.m_col_mac);

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
   m_Popup.append(*item);
   
   m_Popup.accelerate(*this);
   m_Popup.show_all(); //Show all menu items when the menu pops up
}
//////////////////////////////////////////////////////////////////////////////////
// [ Destructor ]
//////////////////////////////////////////////////////////////////////////////////
C_TreeView::~C_TreeView(){
  
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
      m_Popup.popup(button_event->button, button_event->time);
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

   //Add response buttons the the dialog:
   dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
   dialog.add_button("_Open",   Gtk::RESPONSE_OK);
   
   //Show the dialog and wait for a user response:
   int result = dialog.run();
   
   //Handle the response:
   switch(result){
      case(Gtk::RESPONSE_OK): {
         //Notice that this is a std::string, not a Glib::ustring.
         string filename = dialog.get_filename();

         fOutput.open(filename.c_str(), ios_base::out | ios_base::binary); 

         if(!fOutput.is_open()){
	    cout << "Error: open file - " << filename << endl;
	    return;
	 }

         typedef Gtk::TreeModel::Children type_children; //minimise code length.
         type_children children = m_refTreeModel->children();
   
         int nRow = 0;
   
         for(type_children::iterator iter = children.begin(); iter != children.end(); ++iter){
            fOutput << setw(3) << nRow++ << "  " << left << setw(15) << (*iter)[m_Columns.m_col_ip]  << "  " << right << setw(17) << (*iter)[m_Columns.m_col_mac] << endl;
         }

         fOutput.close();
	 
         break;
      }
      case(Gtk::RESPONSE_CANCEL): break;
      default: break;
   }
}
