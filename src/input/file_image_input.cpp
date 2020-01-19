
#include <dxf/file_image.hpp>

int dxf_bison_parse();
int dxf_parser_init(const char* filename);

namespace {

// image to pasrse into
   dxf::file_image_t* the_image = nullptr;

   bool m_send_input_to_image = false;
   bool m_send_input_to_stdout = true;
}

// debug
bool send_dxf_input_to_image()
{
   return m_send_input_to_image;
}

//debug
bool send_dxf_input_to_stdout()
{
   return m_send_input_to_stdout;
}

dxf::file_image_t * get_dxf_file_image()
{
   return the_image;
}

namespace {

   bool dxf_image_init(dxf::file_image_t & image,const char* filename)
   {
      the_image = &image;
      m_send_input_to_image = true;
      bool result = false;
      if ( dxf_parser_init(filename) >=0){
         int ret = dxf_bison_parse();
         if( send_dxf_input_to_stdout()){
            std::cout << "dxf_parse returned " << ret;
         }
         result = ret >= 0;
      }
      the_image = nullptr;
      return result;
   };
}

dxf::file_image_t::file_image_t(const char * filename)
: header{*this}
,tables{*this}
,blocks{*this}
,entities{*this}
{
   auto layer = new layer_t{"0"};
   auto layer_table = new layer_table_t(); 
   tables.add(layer_table);
   layer_table->add_layer(layer);
   if ( ! dxf_image_init(*this,filename)){
      throw std::runtime_error("failed to load dxf image");
   }
}
