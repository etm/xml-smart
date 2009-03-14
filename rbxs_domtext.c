#include "rbxs_dom.h"
#include "rbxs_domtext.h"

/* -- */
//***********************************************************************************
// GC
//***********************************************************************************
void rbxs_domtext_free(rbxs_domtext *prbxs_domtext) {
  if (prbxs_domtext != NULL) {
    free(prbxs_domtext);
  }  
}
  
void rbxs_domtext_mark(rbxs_domtext *prbxs_domtext) { 
  if (prbxs_domtext == NULL) return;
  if (!NIL_P(prbxs_domtext->doc)) rb_gc_mark(prbxs_domtext->doc);
}

//***********************************************************************************
// Methods
//***********************************************************************************
/* ++ */

/*
 *  Documentation
 */
VALUE rbxs_domtext_parent(VALUE self)
{
  rbxs_domtext *prbxs_domtext;
  Data_Get_Struct(self, rbxs_domtext, prbxs_domtext);

  if (prbxs_domtext->node != xmlDocGetRootElement(prbxs_domtext->node->doc))
    return(rbxs_domtext_new(cSmartDomText, self, prbxs_domtext->node->parent));
  else
    return(Qnil);
}

/*
 *  Documentation
 */
VALUE rbxs_domtext_path(VALUE self)
{
  rbxs_domtext *prbxs_domtext;
  xmlChar *ret;
  VALUE val;

  Data_Get_Struct(self, rbxs_domtext, prbxs_domtext);
  ret = xmlGetNodePath(prbxs_domtext->node);
  val = rb_str_new2((char *)ret);
  xmlFree(ret);
  return(val);
}

/*
 *  Documentation
 */
VALUE rbxs_domtext_text_set(VALUE self, VALUE text)
{
  rbxs_domtext *prbxs_domtext;
  rbxs_dom *prbxs_dom;
  VALUE str;

  Data_Get_Struct(self, rbxs_domtext, prbxs_domtext);
  Data_Get_Struct(prbxs_domtext->doc, rbxs_dom, prbxs_dom);

  rbxs_dom_change_handlers_execute(prbxs_dom,RBXS_DOM_SIGNAL_CHANGE,self);
  str = rb_obj_as_string(text);
  if (NIL_P(str) || TYPE(str) != T_STRING)
    rb_raise(rb_eTypeError, "cannot convert obj to string");
  xmlNodeSetContent(prbxs_domtext->node, (unsigned char *)StringValuePtr(str));

  return(text);
}

/*
 *  Documentation
 */
VALUE rbxs_domtext_text_get(VALUE self)
{
  rbxs_domtext *prbxs_domtext;
  
  Data_Get_Struct(self, rbxs_domtext, prbxs_domtext);
  if (prbxs_domtext->node->type == XML_TEXT_NODE)
    return(rb_str_new2((char *)prbxs_domtext->node->content));
  return(rb_str_new2(""));
}

/*
 *  Documentation
 */
VALUE rbxs_domtext_inspect(VALUE self)
{
  VALUE *argv;

  argv = ALLOCA_N(VALUE, 4);
  argv[0] = rb_str_new2("#<%s:0x%x \"%s\">");
  argv[1] = CLASS_OF(self);
  argv[2] = rb_obj_id(self);
  argv[3] = rbxs_domtext_text_get(self);
  return(rb_f_sprintf(4, argv));
}

/*
 *  Documentation
 */
VALUE rbxs_domtext_to_i(int argc, VALUE *argv, VALUE self)
{
    VALUE b;
    int base;

    rb_scan_args(argc, argv, "01", &b);
    if (argc == 0) base = 10;
    else base = NUM2INT(b);

    if (base < 0)
	    rb_raise(rb_eArgError, "illegal radix %d", base);
    return(rb_str_to_inum(rbxs_domtext_text_get(self),base,Qfalse));
}

/*
 *  Documentation
 */
VALUE rbxs_domtext_to_f(VALUE self)
{
  return rb_float_new(rb_str_to_dbl(rbxs_domtext_text_get(self), Qfalse));
}

//***********************************************************************************
// Constructors
//***********************************************************************************
VALUE rbxs_domtext_new(VALUE class, VALUE doc, xmlNodePtr node) {
  rbxs_domtext *prbxs_domtext;

  prbxs_domtext = (rbxs_domtext *)malloc(sizeof(rbxs_domtext));
  if (prbxs_domtext == NULL )
    rb_raise(rb_eNoMemError, "No memory left for XML::Smart::Dom::Text struct");

  prbxs_domtext->doc  = doc;
  prbxs_domtext->node = node;

  return(Data_Wrap_Struct(class, rbxs_domtext_mark, rbxs_domtext_free, prbxs_domtext));
}

//***********************************************************************************
// Initialize class Node
//***********************************************************************************
#ifdef RDOC__
  mXML  = rb_define_module( "XML" );
  cSmart = rb_define_class_under( mXML, "Smart", rb_cObject );
  cSmartDom = rb_define_class_under( cSmart, "Dom", rb_cObject );
#endif
VALUE cSmartDomText;

void init_rbxs_domtext(void) {
  cSmartDomText = rb_define_class_under( cSmartDom, "Text", rb_cObject );

  rb_define_method(cSmartDomText, "inspect", rbxs_domtext_inspect,   0);
  rb_define_method(cSmartDomText, "to_s",    rbxs_domtext_text_get,  0);
  rb_define_method(cSmartDomText, "to_i",    rbxs_domtext_to_i,     -1);
  rb_define_method(cSmartDomText, "to_f",    rbxs_domtext_to_f,      0);
  rb_define_method(cSmartDomText, "text",    rbxs_domtext_text_get,  0);
  rb_define_method(cSmartDomText, "text=",   rbxs_domtext_text_set,  1);
  rb_define_method(cSmartDomText, "parent",  rbxs_domtext_parent,    0);
  rb_define_method(cSmartDomText, "path",    rbxs_domtext_path,      0);
}
