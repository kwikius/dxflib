#ifndef DXFLIB_EXAMPLES_AEROFOIL_HPP_INCLUDED
#define DXFLIB_EXAMPLES_AEROFOIL_HPP_INCLUDED

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

#include <quan/length.hpp>
#include <quan/two_d/vect.hpp>

/*
 load aerofoil from file
 aerofoil is in standard format. nominal chord of 1.
  points go from trailing edge upper side, through le to trailing edge lower side
*/

struct  aerofoil{

   aerofoil(std::string const & filename)
   {
      if (! load(filename)){
         std::string msg = "\"" + filename + "\" : invalid aerofoil file";
         throw std::runtime_error(msg.c_str());
      }
   }
   std::string get_name() const {return m_name;}
   std::size_t get_num_coords() const
   {
      return this->m_coords.size();
   }

   bool output_dxf(std::ostream & out,quan::length::mm const & chord, quan::length::mm const & te_thickness)const;

   quan::two_d::vect<double> get_coord(int i) const
   {
      return this->m_coords.at(i);
   }

   std::vector<quan::two_d::vect<double> > const &  get_coords_vect() const { return m_coords;}

   void  add_coord(quan::two_d::vect<double> const & cd)
   {
      this->m_coords.push_back(cd);
   }
   void set_name( const char* const name) { m_name = name;}
   private:
   std::string m_name;
   std::vector<quan::two_d::vect<double> > m_coords;
   bool load(std::string const & filename);
};

#endif // DXFLIB_EXAMPLES_AEROFOIL_HPP_INCLUDED
