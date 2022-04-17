
#include <list>
#include <dxf/file_image.hpp>
#include <dxf/entity/line.hpp>
#include <quan/two_d/box.hpp>

#include <quan/two_d/out/line.hpp>

/*
 input file holds lines representing parts of two_d boxes in x and y.
 Assemble the lines in the files into boxes and add to out list of boxes
 (originally for use with boxes exported to dxf from OpenSCAD squares)
*/
bool dxf_to_clip_boxes(const char *  input_file, std::list<quan::two_d::box<double> > & out)
{
  // open dxf file - throws exception if not successful
    dxf::file_image_t dxf_image(input_file);

    // see what polymorphic entities we got.
    auto entities = dxf_image.entities.get_entities();

    // extract 2d lines from dxf file image to these lists - after checking they are valid for box
    std::list<quan::two_d::line<quan::two_d::vect<double> > > horz_lines;
    std::list<quan::two_d::line<quan::two_d::vect<double> > > vert_lines;
    for (auto e: entities){
      dxf::line_t* line = dynamic_cast<dxf::line_t*>(e);
      if (line != nullptr){

         auto const start = line->m_start;
         auto const end = line->m_end;
         // lines must be same in z
         // Note arguable all lines should be in same z?
         if (start.z != end.z){
            std::cerr << "clip box lines not same in z\n";
            return false;
         }

         quan::two_d::vect<double> start_out{start.x,start.y};
         quan::two_d::vect<double> end_out{end.x,end.y};
         quan::two_d::line<quan::two_d::vect<double> > line_out{start_out, end_out};
         if ( start.x == end.x){
            vert_lines.push_back(line_out);
         }else{
            if (start.y == end.y){
              horz_lines.push_back(line_out);
            }else{
              std::cerr << "clip box lines not horizontal or vertical\n";
              return false;
            }
         }
      }
    }
    // both horz and vert should be same length and number should be divisible by 4
    if ( horz_lines.size() != vert_lines.size()){
       std::cerr << "not same no of vert and horz lines\n";
       return false;
    }
    if ( (horz_lines.size() % 2) !=0){
        std::cerr << "horz and vert lines not divisible by 2\n";
        return false;
    }
    // sort all horz lines so that horz lines have from.x < to.x
    // zero length lines where from.x == to.x are invalid
    for ( auto & hl : horz_lines){
       if ( hl.from.x == hl.to.x){
          std::cerr << "zero length horizontal line in dxf_to_clip_boxes\n";
          return false;
       }
       if ( hl.from.x > hl.to.x){
         std::swap(hl.from.x,hl.to.x);
       }
    }
    // sort all vertlines so that vert lines have from.y < to.y
    for ( auto & vl : vert_lines){
        // zero length lines where from.y == to.y are invalid
       if ( vl.from.y == vl.to.y){
          std::cerr << "zero length vertical line in dxf_to_clip_boxes\n";
          return false;
       }
       if ( vl.from.y > vl.to.y){
         std::swap(vl.from.y,vl.to.y);
       }
    }

    // look for pairs of vertical and horizontal lines making a box
    int num_boxes = 0;
    while (vert_lines.size() > 1) {
        auto const first = vert_lines.front();
        vert_lines.pop_front();
        // there must be a matching vertical line to first somewhere in vert_lines
        bool matching_vline = false;
        for (auto v_iter = vert_lines.begin();v_iter != vert_lines.end();){
            if( (v_iter->from.y == first.from.y) && (v_iter->to.y == first.to.y)){

                //possible vertical line match
                //ignore if the vertical lines coincide
                if ( first.from.x != v_iter->from.x){

                    // order the possible pair in increasing x
                    auto const vert_lhs = (first.from.x < v_iter->from.x)?first:*v_iter;
                    auto const vert_rhs = (first.from.x > v_iter->from.x)?first:*v_iter;
                    // now look for the matching horizontal lines at bottom in horz_lines
                    bool bottom_matching_hline = false;
                    for(auto hb_iter = horz_lines.begin();hb_iter != horz_lines.end();){
                        if ( (hb_iter->from.x == vert_lhs.from.x) && (hb_iter->to.x == vert_rhs.from.x)) {
                          // at this point we have a match if the data is valid
                          // Note then that all points are in vert lines vert_lhs vert_rhs above
                          bottom_matching_hline = true;
                          horz_lines.erase(hb_iter);
                          //n.b hb_iter now invalid!!!!!
                          // but look for top horizontal line for validity
                          bool top_matching_hline = false;
                          for(auto ht_iter = horz_lines.begin();ht_iter != horz_lines.end();){
                             if (( ht_iter->from.x == vert_lhs.to.x) && (ht_iter->to.x == vert_rhs.to.x)){
                                 // we found  the matching top horizontal line
                                 top_matching_hline = true;
                                 horz_lines.erase(ht_iter);
                                 // validated
                                 matching_vline = true;
                                 vert_lines.erase(v_iter);
                                 // construct the box
                                 auto const left = vert_lhs.from.x;
                                 auto const top = vert_lhs.to.y;
                                 auto const right = vert_rhs.from.x;
                                 auto const bottom = vert_lhs.from.y;
                                 // add the box to the list
                                 quan::two_d::box<double> box{left,top,right,bottom};
                                 out.push_back(box);
                                 ++num_boxes;
                                 std::cout << "box number " << num_boxes <<'\n';
                                 std::cout << "left = " << box.left <<'\n';
                                 std::cout << "top = " << box.top <<'\n';
                                 std::cout << "right = " << box.right <<'\n';
                                 std::cout << "bottom = " << box.bottom << "\n\n";
                                 break;
                             }else{
                                ++ht_iter;
                             }
                           }// ~ top matching horz line loop
                           // for validity we must have found a top matching line
                           if( top_matching_hline == true){
                              break;
                           }else{
                              std::cerr << "failed to find top matching horizontal line of box\n";
                              return false;
                           }
                        }
                        // exit this level if we found a bottom matching line
                        if(bottom_matching_hline == true){
                           break;
                        }else{
                          ++hb_iter;
                        }
                    }// ~bottom matching horz line loop
                    // for validity we must have find a bottom matching line
                    if(bottom_matching_hline == true){
                        break;
                    }else{
                       std::cerr << "failed to find bottom matching horizontal line of box\n";
                       return false;
                    }
                }//ignore corner case of coincident vertical lines
            }

            if(matching_vline == false){
               ++v_iter;
            }
        }//~matching vertical line loop
        // for validty we must have found the box
        if (matching_vline == false){
            std::cerr << "failed to find matching vertical line of box\n";
            return false;
        }
    }// repeat until all v_lines done
    return true;
}

//int main(int argc , const char* argv[])
//{
////   std::list<quan::two_d::box<double> > clipboxes;
////
////   dxf_to_clip_boxes("clip_boxes.dxf", clipboxes);
//
//
//}

