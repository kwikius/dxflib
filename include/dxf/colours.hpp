#ifndef DXF_COLOURS_HPP_INCLUDED
#define DXF_COLOURS_HPP_INCLUDED


/*
How the default dxf colour numbers relate to rgb colours
https://www.globalmapperforum.com/discussion/4028/dxf-color
N.B.  Colours indices may be changed
*/
namespace dxf {
      struct colour{
         static constexpr int16_t white = 0; // rgb( 255, 255, 255 ) 
         static constexpr int16_t red = 1;  // rgb( 255, 0, 0 ) ;
         static constexpr int16_t yellow =2; // rgb( 255, 255, 0 )
         static constexpr int16_t green = 3; //rgb( 0, 255, 0 ) 
         static constexpr int16_t cyan = 4; // rgb( 0, 255, 255 )
         static constexpr int16_t blue = 5; // rgb( 0, 0, 255 ) 
         static constexpr int16_t magenta = 6; // rgb( 255, 0, 255 )
         static constexpr int16_t black = 7;   // rgb( 0, 0, 0 )
         static constexpr int16_t gray = 8;  // rgb( 128, 128, 128 )
         static constexpr int16_t silver = 9; // rgb( 192, 192, 192 )
    
      };
/*
// maybe names for other colours
colour_index : 11 = rgb( 255, 123, 123 ) ; // salmon

colour_index : 21 = rgb( 255, 156, 123 ) ; //light_salmon

colour_index : 31 = rgb( 255, 189, 123 ) ; //topaz

colour_index : 41 = rgb( 255, 222, 183 ) ; //peach puff

colour_index : 51 = rgb( 255, 255, 123 ) ; //yellow

colour_index : 61 = rgb( 222, 255, 123 ) ; // light khaki

colour_index : 71 = rgb( 189, 255, 123 ) ; //

colour_index : 81 = rgb( 156, 255, 123 ) ; // mint green

colour_index : 91 = rgb( 123, 255, 123 ) ; //pale green

colour_index : 101 = rgb( 122, 255, 156 ) ; // mint green

colour_index : 111 = rgb( 123, 255, 189 ) ; //aquamarine

colour_index : 121 = rgb( 123, 255, 222 ) ;  //aquamarine v2
colour_index : 131 = rgb( 123, 255, 255 ) ;  //electric blue
colour_index : 141 = rgb( 123, 222, 255 ) ;  //light sky blue
colour_index : 151 = rgb( 123, 189, 255 ) ;
colour_index : 161 = rgb( 123, 156, 255 ) ;
colour_index : 171 = rgb( 123, 123, 255 ) ;
colour_index : 181 = rgb( 156, 123, 255 ) ;
colour_index : 191 = rgb( 189, 123, 255 ) ;
colour_index : 201 = rgb( 222, 123, 255 ) ;
colour_index : 211 = rgb( 255, 123, 255 ) ;
colour_index : 221 = rgb( 255, 123, 222 ) ;
colour_index : 231 = rgb( 255, 123, 189 ) ;
colour_index : 241 = rgb( 255, 123, 156 ) ;

colour_index : 251 = rgb( 41, 41, 41 ) ;
colour_index : 252 = rgb( 90, 90, 90 ) ;
colour_index : 253 = rgb( 136, 136, 136 ) ;
colour_index : 254 = rgb( 181, 181, 181 ) ;
*/

} //dxf

#endif // DXF_COLOURS_HPP_INCLUDED
