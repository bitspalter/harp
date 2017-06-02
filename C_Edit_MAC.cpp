//////////////////////////////////////////////////////////////////////////////////
// [ Edit_Mac_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_Edit_MAC.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [clear] 
//////////////////////////////////////////////////////////////////////////////////
void C_Edit_MAC::clear(){
   for(int n = 0; n < 6; n++) aEdit[n].set_text("");
}
//////////////////////////////////////////////////////////////////////////////////
// [setSize]
//////////////////////////////////////////////////////////////////////////////////
void C_Edit_MAC::setSize(int x, int y){
   for(int n = 0; n < 6; n++){
      aEdit[n].set_width_chars(x);
      aEdit[n].set_size_request(-1, y);
   }
}
//////////////////////////////////////////////////////////////////////////////////
// [setMAC] ( char* )
//////////////////////////////////////////////////////////////////////////////////
int C_Edit_MAC::setMAC(const char* psV1, const char* psV2, const char* psV3, const char* psV4, const char* psV5, const char* psV6){
  
   if(!psV1 || !psV2 || !psV3 || !psV4 || !psV5 || !psV6) return(C_EDIT_MAC_ERROR);

   aEdit[0].set_text(psV1);
   aEdit[1].set_text(psV2);
   aEdit[2].set_text(psV3);
   aEdit[3].set_text(psV4);
   aEdit[4].set_text(psV5);
   aEdit[5].set_text(psV6);
   
   return(C_EDIT_MAC_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [setMAC] ( UCHAR )
//////////////////////////////////////////////////////////////////////////////////
int C_Edit_MAC::setMAC(UCHAR ucV1, UCHAR ucV2, UCHAR ucV3, UCHAR ucV4, UCHAR ucV5, UCHAR ucV6){
  
   ostringstream Str_1;
   Str_1 << uppercase << setw(2) << setfill('0') << (int) ucV1;
   aEdit[0].set_text(Str_1.str());

   ostringstream Str_2;
   Str_2 << uppercase << setw(2) << setfill('0') << (int) ucV2; 
   aEdit[1].set_text(Str_2.str());

   ostringstream Str_3;
   Str_3 << uppercase << setw(2) << setfill('0') << (int) ucV3;
   aEdit[2].set_text(Str_3.str());
   
   ostringstream Str_4;
   Str_4 << uppercase << setw(2) << setfill('0') << (int) ucV4;
   aEdit[3].set_text(Str_4.str());
   
   ostringstream Str_5;
   Str_5 << uppercase << setw(2) << setfill('0') << (int) ucV3;
   aEdit[4].set_text(Str_5.str());
   
   ostringstream Str_6;
   Str_6 << uppercase << setw(2) << setfill('0') << (int) ucV4;
   aEdit[5].set_text(Str_6.str());
   
   return(C_EDIT_MAC_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [getMAC] (char*)
//////////////////////////////////////////////////////////////////////////////////
int C_Edit_MAC::getMAC(string* psData){
  
   Glib::ustring SEdit;

   if(!psData) return(C_EDIT_MAC_ERROR);

   for(int i = 0; i < 6; i++){
      SEdit.clear();
      SEdit = aEdit[i].get_text();
      *psData += SEdit;
      if(i < 5) *psData += ":";
   }

   return(C_EDIT_MAC_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [getMAC] ( UCHAR* )
//////////////////////////////////////////////////////////////////////////////////
int C_Edit_MAC::getMAC(UCHAR* pData){
  
   Glib::ustring SEdit;
   unsigned int dwData = 0;

   if(!pData) return(C_EDIT_MAC_ERROR);

   for(int n = 0; n < 6; n++){
      SEdit.clear();
      SEdit = aEdit[n].get_text();

      if(isHEX(SEdit.c_str())){
         dwData   = hexToDec(SEdit.c_str());
         pData[n] = (UCHAR)dwData;
      }
   }
   
   return(C_EDIT_MAC_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [isHEX] (UCHAR*)
//////////////////////////////////////////////////////////////////////////////////
int C_Edit_MAC::isHEX(const char* psData){
  
   if(!psData) return(C_EDIT_MAC_ERROR);

   Glib::ustring T = psData;
   
   for(int i = 0; i < T.size(); i++){
      if((psData[i] < '0' || psData[i] > '9') && 
         (psData[i] < 'a' || psData[i] > 'f') && 
         (psData[i] < 'A' || psData[i] > 'F')) return(C_EDIT_MAC_ERROR);
   }
   
   return(C_EDIT_MAC_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [hexToDec] (UCHAR*)
//////////////////////////////////////////////////////////////////////////////////
int C_Edit_MAC::hexToDec(const char* psData){
  
   int result = 0;
   int Shift  = 0;

   if(!psData) return(0);

   Glib::ustring T = psData;
   
   for(int i = 0; i < T.size(); i++){
      if(i == 0) Shift = 16;
      if(i == 1) Shift =  1;

      if(T.size() == 1) Shift =  1;

      if(psData[i] > 47 && psData[i] < 58){  // "0" -> "9"
         result += (psData[i] - 48) * Shift;
      }else
      if(psData[i] > 64 && psData[i] < 71){  // "A" -> "F"
         result += (psData[i] - 55) * Shift;
      }else
      if(psData[i] > 96 && psData[i] < 103){ // "a" -> "f"
         result += (psData[i] - 87) * Shift;
      }
   }

   return(result);
}