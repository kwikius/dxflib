#ifndef DXF_POLYLINE_VERTEX_HPP_INCLUDED
#define DXF_POLYLINE_VERTEX_HPP_INCLUDED

#include <quan/three_d/vect.hpp>

namespace dxf{

   struct vertex_t : entity_t{

      typedef quan::three_d::vect<double> point_t;

      std::ostream & output_derived(std::ostream & os)const final
      {
          os << "   10\n" << m_point.x << "\n   20\n" << m_point.y << "\n   30\n" << m_point.z << "\n";
          os << "   40\n" << m_starting_width << "\n   41\n" << m_ending_width << "\n";
          os << "   70\n" << m_vertex_flags.value << "\n";
          return os;
      }

      struct flags{
          static constexpr uint16_t extra_curve_fit_vertex     = 0b0000'0001;
          static constexpr uint16_t curve_fit_tangent_defined  = 0b0000'0010;
          static constexpr uint16_t spline_fit_vertex          = 0b0000'1000;
          static constexpr uint16_t spline_frame_control_point = 0b0001'0000;
          static constexpr uint16_t polyline_vertex            = 0b0010'0000;
          static constexpr uint16_t polygon_mesh               = 0b0100'0000;
          static constexpr uint16_t polyface_mesh              = 0b1000'0000;
      };
    
      vertex_t(point_t const & pt_in, int16_t flags_in)
      :entity_t{entity_type_t::vertex}
      ,m_point(pt_in)
      ,m_starting_width(0)
      ,m_ending_width(0)
      ,m_vertex_flags{flags_in}
      {}

      private:
        point_t m_point;
        int32_t m_starting_width;
        int32_t m_ending_width;
        union{
           struct {
               bool     extra_curve_fit_vertex      :1;   //   1
               bool     curve_fit_tangent_defined   :1;   //   2
               bool     unused1                     :1;   //   4
               bool     spline_fitting_vertex       :1;   //   8
               bool     spline_frame_control_point  :1;   //  16
               bool     polyline_vertex             :1;   //  32
               bool     polygon_mesh                :1;   //  64
               bool     polyface_mesh               :1;   // 128
               uint8_t  unused2                     :8;
           };
           int16_t  value;
        }m_vertex_flags;
   };

} // dxf

#endif // DXF_POLYLINE_VERTEX_HPP_INCLUDED
