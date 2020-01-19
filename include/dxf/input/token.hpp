#ifndef DXF_PARSER_TOKEN_HPP_INCLUDED
#define DXF_PARSER_TOKEN_HPP_INCLUDED

#include <quan/lexer/filepos.hpp>
#include <typeinfo>
#include <cassert>

struct token{
private:
   template <typename T>
   static 
   void deleter_function( void* p)
   {
      T* t = reinterpret_cast<T*> (p);
      t->~T();
   }

   struct ptr_info_t{
      void * ptr;
      void (*deleter)(void * p);
      const char * typeident;
   };

   /*
      empty token is unused
      syntactic has no attribute
      semantic has an attribute which requires memory management
   */
   enum class token_style{empty = 0, syntactic = 1, semantic = 2};
public:
   token():m_token_style{token_style::empty}, m_id(0){}
   token(quan::lexer::filepos const & fp_in) : fp{fp_in},m_token_style{token_style::empty}, m_id(0){}
   ~token(){ 
      if (this->m_token_style == token_style::semantic) { ptr_info.deleter(ptr_info.ptr);} 
    }

   void clear_attribute()
   {
      if (this->m_token_style == token_style::semantic) { 
         ptr_info.deleter(ptr_info.ptr);
      }
      this->m_token_style = token_style::empty;
      this->m_id = 0;
   }

   void set_filepos(quan::lexer::filepos const & fp_in)
   {
       fp = fp_in;
   }

   template <typename P>
   void set_ptr( P* p, int id = -1)
   {
      assert( (m_token_style == token_style::empty) || (m_token_style == token_style::syntactic) );

      this->m_token_style = token_style::semantic;
      this->ptr_info.ptr = p;
      this->ptr_info.deleter = deleter_function<P>;
      this->ptr_info.typeident = typeid(P*).name();
      this->m_id = id;
   }
   
   // set the attribute and so change the token style to semantic
   template <typename P>
   void set_ptr(quan::lexer::filepos const & fp_in, P* p, int id = -1)
   {
      this->set_filepos(fp_in);
      this->template set_ptr<P>(p,id);
   }

   int get_id()const
   {
       return m_id;
   }

   void set_id(int id_in)
   {
      assert(m_token_style == token_style::empty);
      this->m_token_style = token_style::syntactic;
      this->m_id = id_in;
   }
   

   void set_id(quan::lexer::filepos const & fp_in,int id_in)
   {
      this->set_filepos(fp_in);
      this->set_id(id_in);
   }

   // check it is a pointer to P
   template <typename P>
   bool is_ptr() const
   {
      if (this->m_token_style != token_style::semantic) {
         return false;
      }
      return (this->ptr_info.typeident == typeid(P*).name());
   }

   // get the pointed type, asserts if not asked for correct type
   template <typename P> P* get_ptr()const
   {
     // assert( (this->m_token_style == token_style::semantic) && (this->ptr_info.typeident == typeid(P*).name()));
      assert( this->is_ptr<P>());
      return reinterpret_cast<P*>(ptr_info.ptr);
   }

   // n.b the filepos is not modified
   template <typename P> P* release_ptr()
   {
      P* temp = this->get_ptr<P>();
      this->m_id = 0;
      this->m_token_style = token_style::empty;
   
      return temp;
   }

   quan::lexer::filepos fp;
private:
   ptr_info_t ptr_info;  // deleter function
   token_style m_token_style;  // empty, syntactic, semantic
   int m_id;  // Id representing the tokens unique type. This is returned by yylex() to the parser
   // for parsing
};

// to release the attribute
 template <typename T>
 inline
 T* release_del(token * p)
 {
    assert (p != nullptr);
    auto temp = p->template release_ptr<T>();
    delete p;
    return temp;
 }

#endif // DXF_PARSER_TOKEN_HPP_INCLUDED
