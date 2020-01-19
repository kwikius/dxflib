#ifndef DXF_INPUT_BISON_PUBLIC_HPP_INCLUDED
#define DXF_INPUT_BISON_PUBLIC_HPP_INCLUDED

#include "bison/dxf_bison_fwd.hpp"
#include "bison/dxf_bison.hpp"
#include <dxf/file_image.hpp>

dxf::file_image_t * get_dxf_file_image();

bool send_dxf_input_to_image();
bool send_dxf_input_to_stdout();

#endif // DXF_INPUT_BISON_PUBLIC_HPP_INCLUDED
