//////////////////////////////////////////////////////////////////////////////////
// [ Edit_Mac_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
//
// [::Last modi: 31.05.17 L.ey (µ~)::]  
//
//
#ifndef _C_EDIT_MAC_H_
 #define _C_EDIT_MAC_H_

  #include <iostream>
  #include <sstream>
  #include <string> 
  #include <iomanip>

  using namespace std;

  #include <gtkmm/entry.h>
   
  typedef unsigned char UCHAR;
   
//////////////////////////////////////////////////////////////////////////////////
 
  const int C_EDIT_MAC_READY = 0x01;
  const int C_EDIT_MAC_ERROR = 0x00;

//////////////////////////////////////////////////////////////////////////////////

   class C_Edit_MAC {
     
      public:

         C_Edit_MAC(){};  
        ~C_Edit_MAC(){};  

         //////////////////////////////////
	 
         void setSize(int x, int y);
         void clear();

         //////////////////////////////////

         int setMAC(const char* psV1, const char* psV2, const char* psV3, const char* psV4, const char* psV5, const char* psV6);
         int setMAC(UCHAR ucV1, UCHAR ucV2, UCHAR ucV3, UCHAR ucV4, UCHAR ucV5, UCHAR ucV6);

         //////////////////////////////////

         int getMAC(string* psData); 
         int getMAC(UCHAR* pData);

         /////////////////////////////////
         // INLINE
         void hide(){aEdit[0].hide(); aEdit[1].hide(); aEdit[2].hide(); 
                     aEdit[3].hide(); aEdit[4].hide(); aEdit[5].hide();}

         void show(){aEdit[0].show(); aEdit[1].show(); aEdit[2].show(); 
                     aEdit[3].show(); aEdit[4].show(); aEdit[5].show();}
                     
	 /////////////////////////////////
	 
	 Gtk::Entry aEdit[6];
	 
      protected:

         int  isHEX(const char* psData);
         int  hexToDec(const char* psData);
   };

#endif // _C_EDIT_MAC_H_