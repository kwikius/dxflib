

enum class group_code_type {
    UNKNOWN,
    STRING,
    DOUBLE_3D,
    DOUBLE,
    INT16,
    INT32,
    STRING255,
    DOUBLE_SCALAR,
    INT64,
    BOOL,
    HEX_STRING,
    HEX_STRING_HANDLE,
    HEX_STRING_CHUNK,
    HEX_STRING_OBJECT_ID,
    LONG,
    COMMENT
};

group_code_type get_group_code_type(int gc)
{
   if ((gc >=0) && ( gc < 10)){
      return group_code_type::STRING;
   }
   if (gc < 40){
      return group_code_type::DOUBLE_3D;
   }
   if(gc < 60){
      return group_code_type::DOUBLE;
   }
   if(gc < 80){
      return group_code_type::INT16;
   }

   if(gc < 90){
      return group_code_type::UNKNOWN;
   }

   if(gc < 100){
      return group_code_type::INT32;
   }

   if(gc == 100){
      return group_code_type::STRING255;
   }

   if(gc == 101){
      return group_code_type::UNKNOWN;
   }

   if( gc == 102){
      return group_code_type::STRING255;
   }

   if( gc < 105 ){
      return group_code_type::UNKNOWN;
   }

   if(gc == 105){
      return group_code_type::HEX_STRING_HANDLE;
   }
   if(gc < 110){
      return group_code_type::UNKNOWN;
   }
   if(gc <140){
      return group_code_type::DOUBLE;
   }
   if(gc <150){
      return group_code_type::DOUBLE_SCALAR;
   }
   if(gc <160){
      return group_code_type::UNKNOWN;
   }
   if(gc < 170){
      return group_code_type::INT64;
   }
   if(gc <180){
      return group_code_type::INT16;
   }
   if(gc < 210){
      return group_code_type::UNKNOWN;
   }

   if(gc < 240){
      return group_code_type::DOUBLE;
   }

   if(gc < 270){
      return group_code_type::UNKNOWN;
   }

   if(gc < 290){
      return group_code_type::INT16;
   }

   if(gc < 300){
      return group_code_type::BOOL;
   }
   if(gc < 310){
      return group_code_type::STRING;
   }

   if(gc <320){
      return group_code_type::HEX_STRING_CHUNK;
   }

   if(gc < 330){
      return group_code_type::HEX_STRING_HANDLE;
   }

   if(gc < 370){
      return group_code_type::HEX_STRING_OBJECT_ID;
   }

   if(gc < 390){
      return group_code_type::INT16;
   }

   if(gc < 400){
      return group_code_type::HEX_STRING_HANDLE;
   }

   if(gc < 410){
      return group_code_type::INT16;
   }

   if(gc < 420){
      return group_code_type::STRING;
   }

   if(gc < 430){
      return group_code_type::INT32;
   }
   if(gc < 440){
      return group_code_type::STRING;
   }
   if(gc < 450){
      return group_code_type::INT32;
   }
   if(gc < 460){
      return group_code_type::LONG;
   }
   if(gc < 470){
      return group_code_type::DOUBLE;
   }
   if(gc < 480){
      return group_code_type::HEX_STRING_HANDLE;
   }

   if(gc < 999){
      return group_code_type::UNKNOWN;
   }

   if(gc == 999){
      return group_code_type::COMMENT;
   }
   if(gc < 1010){
      return group_code_type::STRING;
   }

   if(gc < 1060){
      return group_code_type::DOUBLE;
   }

   if(gc < 1070){
      return group_code_type::INT16;
   }

   if(gc == 1071){
      return group_code_type::INT32;
   }

   return group_code_type::UNKNOWN;
}