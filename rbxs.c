#include "rbxs.h"
#include "rbxs_qname.h"
#include "rbxs_dom.h"
#include "rbxs_domnamespace.h"
#include "rbxs_domnamespaceset.h"
#include "rbxs_domelement.h"
#include "rbxs_domtext.h"
#include "rbxs_domother.h"
#include "rbxs_domnodeset.h"
#include "rbxs_domattribute.h"
#include "rbxs_domattributeset.h"
#include "rbxs_pull.h"
#include "rbxs_pullattribute.h"
#include "rbxs_pullattributeset.h"
#include "rbxs_utils.h"

/* -- */
//***********************************************************************************
// Constructors
//***********************************************************************************
/* ++ */

/*
 *  Documentation
 */
VALUE rbxs_main_modify(int argc, VALUE *argv, VALUE class)
{
  char *root;
  unsigned short int lock;

  Check_Type(argv[0], T_STRING);
  
  root = NULL;
  lock = 7;
  switch (argc) {
    case 3:
      if ((TYPE(argv[2]) == T_FIXNUM) && (TYPE(argv[1]) != T_STRING)) {
        Check_Type(argv[2], T_FIXNUM);
        lock = NUM2INT(argv[2]);
        root = StringValuePtr(argv[1]);
      } else if ((TYPE(argv[1]) == T_FIXNUM) && (TYPE(argv[2]) != T_STRING)) {
        Check_Type(argv[1], T_FIXNUM);
        lock = NUM2INT(argv[1]);
        root = StringValuePtr(argv[2]);
      } else  
        rb_raise(rb_eArgError, "argument 2 and 3 have to be Hash and Integer");
      break; 
    case 2:
      if (TYPE(argv[1]) == T_STRING) 
        root = StringValuePtr(argv[1]);
      else if (TYPE(argv[1]) == T_FIXNUM) {
        Check_Type(argv[2], T_FIXNUM);
        lock = NUM2INT(argv[2]);
      } else  
        rb_raise(rb_eArgError, "argument 2 has to be a String or an Integer");
      break;
    case 1:
      break;
    default:  
      rb_raise(rb_eArgError, "wrong number of arguments (need 1, 2 or 3)");
  }
  return(rbxs_dom_open(argv[0],root,lock,1));
}

/*
 *  Documentation
 */
VALUE rbxs_main_open(VALUE class, VALUE name)
{
  Check_Type(name, T_STRING);
  return(rbxs_dom_open(name,NULL,0,0));
}
/*
 *  Documentation
 */
VALUE rbxs_main_open_b(VALUE class, VALUE name, VALUE str)
{
  Check_Type(name, T_STRING);
  Check_Type(str, T_STRING);
  return(rbxs_dom_open(name,StringValuePtr(str),0,0));
}
/*
 *  Documentation
 */
VALUE rbxs_main_string(VALUE class, VALUE string)
{
  return(rbxs_dom_string(string));
}
/*
 *  Documentation
 */
VALUE rbxs_main_pull(VALUE class, VALUE what)
{
  return(rbxs_pull_new(cSmartPull,what));
}

//***********************************************************************************
// Initialize module and class
//***********************************************************************************
VALUE mXML;
VALUE cSmart;

void Init_smart( void ) {
  mXML  = rb_define_module( "XML" );
  cSmart = rb_define_class_under( mXML, "Smart", rb_cObject );

  // libxml init
  xmlKeepBlanksDefault(0);
  xmlLineNumbersDefault(0);
  defaultEntityLoader = xmlGetExternalEntityLoader();
  xmlSetExternalEntityLoader(xmllintExternalEntityLoader);

  rb_define_const(cSmart, "LIBXML_VERSION", rb_str_new2(LIBXML_DOTTED_VERSION));
  rb_define_const(cSmart, "VERSION", rb_str_new2(RBXS_VERSION));

  rb_define_singleton_method(cSmart, "modify", (VALUE(*)(ANYARGS))rbxs_main_modify, -1);
  rb_define_singleton_method(cSmart, "open!",  (VALUE(*)(ANYARGS))rbxs_main_open_b,  2);
  rb_define_singleton_method(cSmart, "open",   (VALUE(*)(ANYARGS))rbxs_main_open,    1);
  rb_define_singleton_method(cSmart, "new",    (VALUE(*)(ANYARGS))rbxs_main_string,  1);
  rb_define_singleton_method(cSmart, "string", (VALUE(*)(ANYARGS))rbxs_main_string,  1);
  rb_define_singleton_method(cSmart, "pull",   (VALUE(*)(ANYARGS))rbxs_main_pull,    1);
  
  init_rbxs_qname();
  init_rbxs_dom();
  init_rbxs_domnamespaceset();
  init_rbxs_domnamespace();
  init_rbxs_domelement();
  init_rbxs_domtext();
  init_rbxs_domother();
  init_rbxs_domnodeset();
  init_rbxs_domattribute();
  init_rbxs_domattributeset();
  init_rbxs_pull();
  init_rbxs_pullattributeset();
  init_rbxs_pullattribute();
}
