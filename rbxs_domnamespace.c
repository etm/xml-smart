#include "rbxs_dom.h"
#include "rbxs_domnamespace.h"

/* -- */
//***********************************************************************************
// GC
//***********************************************************************************
void rbxs_domnamespace_free(rbxs_domnamespace *prbxs_domnamespace) {
  if (prbxs_domnamespace != NULL) {
    prbxs_domnamespace->ns->_private = NULL;
    free(prbxs_domnamespace);
  }  
}
  
void rbxs_domnamespace_mark(rbxs_domnamespace *prbxs_domnamespace) { 
  if (prbxs_domnamespace == NULL) return;
  if (!NIL_P(prbxs_domnamespace->node)) rb_gc_mark(prbxs_domnamespace->node);
}

//***********************************************************************************
// Methods
//***********************************************************************************
/* ++ */

/*
 *  Documentation
 */
VALUE rbxs_domnamespace_inspect(VALUE self)
{
  rbxs_domnamespace *prbxs_domnamespace;
  VALUE *argv;

  Data_Get_Struct(self, rbxs_domnamespace, prbxs_domnamespace);
  if (prbxs_domnamespace->ns != NULL) {
    argv = ALLOCA_N(VALUE, 5);
    argv[0] = rb_str_new2("#<%s:0x%x %s=\"%s\">");
    argv[1] = CLASS_OF(self);
    argv[2] = rb_obj_id(self);
    argv[3] = rb_str_new2(prbxs_domnamespace->ns->prefix ? (char *)prbxs_domnamespace->ns->prefix : "");
    argv[4] = rb_str_new2((char *)prbxs_domnamespace->ns->href);
    return(rb_f_sprintf(5, argv));
  }  
  return(Qnil);
}

/*
 *  Documentation
 */
VALUE rbxs_domnamespace_to_s(VALUE self)
{
  rbxs_domnamespace *prbxs_domnamespace;
  
  Data_Get_Struct(self, rbxs_domnamespace, prbxs_domnamespace);
  if (prbxs_domnamespace->ns != NULL)
    return(rb_str_new2(prbxs_domnamespace->ns->prefix ? (char *)prbxs_domnamespace->ns->prefix : ""));
  return(Qnil);
}

/*
 *  Documentation
 */
VALUE rbxs_domnamespace_prefix(VALUE self)
{
  rbxs_domnamespace *prbxs_domnamespace;
  Data_Get_Struct(self, rbxs_domnamespace, prbxs_domnamespace);

  if (prbxs_domnamespace->ns && prbxs_domnamespace->ns->prefix)
	return rb_str_new2((char *)prbxs_domnamespace->ns->prefix);

  return Qnil;
}

/*
 *  Documentation
 */
VALUE rbxs_domnamespace_href(VALUE self)
{
	rbxs_domnamespace *prbxs_domnamespace;
	Data_Get_Struct(self, rbxs_domnamespace, prbxs_domnamespace);

	if (prbxs_domnamespace->ns && prbxs_domnamespace->ns->href)
		return rb_str_new2((char *)prbxs_domnamespace->ns->href);

	return Qnil;
}

//***********************************************************************************
// Constructors
//***********************************************************************************
VALUE rbxs_domnamespace_new(VALUE class, VALUE node, xmlNsPtr ns) {
  rbxs_domnamespace *prbxs_domnamespace;
  VALUE result;

  if (ns->_private) {
    return (VALUE) ns->_private;
  }

  prbxs_domnamespace = (rbxs_domnamespace *)malloc(sizeof(rbxs_domnamespace));
  if (prbxs_domnamespace == NULL )
    rb_raise(rb_eNoMemError, "No memory left for XML::Smart::domNamespace struct");

  prbxs_domnamespace->node = node;
  prbxs_domnamespace->ns = ns;

  result = Data_Wrap_Struct(class, rbxs_domnamespace_mark, rbxs_domnamespace_free, prbxs_domnamespace);
  prbxs_domnamespace->ns->_private = (void*) result;
  return result;
}

//***********************************************************************************
// Initialize class Node
//***********************************************************************************
#ifdef RDOC__
  mXML  = rb_define_module( "XML" );
  cSmart = rb_define_class_under( mXML, "Smart", rb_cObject );
  cSmartDom = rb_define_class_under( cSmart, "Dom", rb_cObject );
#endif
VALUE cSmartDomNamespace;

void init_rbxs_domnamespace(void) {
  cSmartDomNamespace = rb_define_class_under( cSmartDom, "Namespace", rb_cObject );

  rb_define_method(cSmartDomNamespace, "inspect",   rbxs_domnamespace_inspect, 0);
  rb_define_method(cSmartDomNamespace, "to_s",      rbxs_domnamespace_to_s,    0);
  rb_define_method(cSmartDomNamespace, "prefix",    rbxs_domnamespace_prefix,  0);
  rb_define_method(cSmartDomNamespace, "href",      rbxs_domnamespace_href,    0);
}
