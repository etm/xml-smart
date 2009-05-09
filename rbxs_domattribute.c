#include "rbxs_domattribute.h"
#include "rbxs_domelement.h"
#include "rbxs_dom.h"

/* -- */
//***********************************************************************************
// GC
//***********************************************************************************
void rbxs_domattribute_free(rbxs_domattribute *prbxs_domattribute) {
  if (prbxs_domattribute != NULL) {
    free(prbxs_domattribute);
  }  
}
  
void rbxs_domattribute_mark(rbxs_domattribute *prbxs_domattribute) { 
  if (prbxs_domattribute == NULL) return;
  if (!NIL_P(prbxs_domattribute->doc)) rb_gc_mark(prbxs_domattribute->doc);
}

//***********************************************************************************
// Methods
//***********************************************************************************
/* ++ */

/*
 *  Documentation
 */
VALUE rbxs_domattribute_inspect(VALUE self)
{
  rbxs_domattribute *prbxs_domattribute;
  xmlBufferPtr ret;
  VALUE *argv;

  argv = ALLOCA_N(VALUE, 4);
  argv[0] = rb_str_new2("#<%s:0x%x @%s>");
  argv[1] = CLASS_OF(self);
  argv[2] = rb_obj_id(self);
  
  Data_Get_Struct(self, rbxs_domattribute, prbxs_domattribute);
  ret = xmlBufferCreate();
  xmlNodeDump(ret, prbxs_domattribute->attribute->doc, (xmlNodePtr)prbxs_domattribute->attribute, 0, 1);
  argv[3] = rb_str_new2((char *)ret->content);
  argv[3] = rb_str_substr(argv[3],1,RSTRING_LEN(argv[3]));

  xmlBufferFree(ret);
  return(rb_f_sprintf(4, argv));
}

/*
 *  Documentation
 */
VALUE rbxs_domattribute_value_get(VALUE self)
{
  rbxs_domattribute *prbxs_domattribute;
  xmlChar *ret;
  VALUE val;

  Data_Get_Struct(self, rbxs_domattribute, prbxs_domattribute);
  ret = xmlGetProp(prbxs_domattribute->attribute->parent,prbxs_domattribute->attribute->name);
  val = rb_str_new2((char *)ret);
  xmlFree(ret);
  return(val);
}
  
/*
 *  Documentation
 */
VALUE rbxs_domattribute_path(VALUE self)
{
  rbxs_domattribute *prbxs_domattribute;
  xmlChar *ret;
  VALUE val;

  Data_Get_Struct(self, rbxs_domattribute, prbxs_domattribute);
  ret = xmlGetNodePath((xmlNodePtr)prbxs_domattribute->attribute);
  val = rb_str_new2((char *)ret);
  xmlFree(ret);
  return(val);
}

/*
 *  Documentation
 */
VALUE rbxs_domattribute_to_i(int argc, VALUE *argv, VALUE self)
{
    VALUE b;
    int base;

    rb_scan_args(argc, argv, "01", &b);
    if (argc == 0) base = 10;
    else base = NUM2INT(b);

    if (base < 0)
	    rb_raise(rb_eArgError, "illegal radix %d", base);
    return(rb_str_to_inum(rbxs_domattribute_value_get(self),base,Qfalse));
}

/*
 *  Documentation
 */
VALUE rbxs_domattribute_to_f(VALUE self)
{
  return rb_float_new(rb_str_to_dbl(rbxs_domattribute_value_get(self), Qfalse));
}

/*
 *  Documentation
 */
VALUE rbxs_domattribute_name_get(VALUE self)
{
  rbxs_domattribute *prbxs_domattribute;
  Data_Get_Struct(self, rbxs_domattribute, prbxs_domattribute);
  return(rb_str_new2((char *)prbxs_domattribute->attribute->name));
}  

/*
 *  Documentation
 */
VALUE rbxs_domattribute_value_set(VALUE self, VALUE value)
{
  rbxs_domattribute *prbxs_domattribute;
  rbxs_dom *prbxs_dom;
  unsigned char *tc;
  VALUE str;

  Data_Get_Struct(self, rbxs_domattribute, prbxs_domattribute);
  Data_Get_Struct(prbxs_domattribute->doc, rbxs_dom, prbxs_dom);

  str = rb_obj_as_string(value);
  if (NIL_P(str) || TYPE(str) != T_STRING)
    rb_raise(rb_eTypeError, "cannot convert obj to string");
  tc = (unsigned char *)StringValuePtr(str);
  if (!xmlCheckUTF8(tc))
    rb_raise(rb_eArgError, "text must be utf8 encoded");
  xmlNodeSetContent((xmlNodePtr)prbxs_domattribute->attribute,tc);
  
  rbxs_dom_change_handlers_execute(prbxs_dom,RBXS_DOM_SIGNAL_CHANGE,self);
  return(value);
}

/*
 *  Documentation
 */
VALUE rbxs_domattribute_element(VALUE self) 
{
  rbxs_domattribute *prbxs_domattribute;
  Data_Get_Struct(self, rbxs_domattribute, prbxs_domattribute);
  return(rbxs_domelement_new(cSmartDomElement,prbxs_domattribute->doc,prbxs_domattribute->attribute->parent));
}

//***********************************************************************************
// Constructors
//***********************************************************************************
VALUE rbxs_domattribute_new(VALUE class, VALUE doc, xmlAttrPtr attribute) {
  rbxs_domattribute *prbxs_domattribute;

  prbxs_domattribute = (rbxs_domattribute *)malloc(sizeof(rbxs_domattribute));
  if (prbxs_domattribute == NULL )
    rb_raise(rb_eNoMemError, "No memory left for XML::Smart::Dom::Attribute struct");

  prbxs_domattribute->doc       = doc;
  prbxs_domattribute->attribute = attribute;

  return(Data_Wrap_Struct(class, rbxs_domattribute_mark, rbxs_domattribute_free, prbxs_domattribute));
}

//***********************************************************************************
// Initialize class Node
//***********************************************************************************
#ifdef RDOC__
  mXML  = rb_define_module( "XML" );
  cSmart = rb_define_class_under( mXML, "Smart", rb_cObject );
  cSmartDom = rb_define_class_under( cSmart, "Dom", rb_cObject );
#endif
VALUE cSmartDomAttribute;
 
void init_rbxs_domattribute(void) {
  cSmartDomAttribute = rb_define_class_under( cSmartDom, "Attribute", rb_cObject );

  rb_define_method(cSmartDomAttribute, "inspect", (VALUE(*)(ANYARGS))rbxs_domattribute_inspect,    0);
  rb_define_method(cSmartDomAttribute, "to_s",    (VALUE(*)(ANYARGS))rbxs_domattribute_value_get,  0);
  rb_define_method(cSmartDomAttribute, "to_i",    (VALUE(*)(ANYARGS))rbxs_domattribute_to_i,      -1);
  rb_define_method(cSmartDomAttribute, "to_f",    (VALUE(*)(ANYARGS))rbxs_domattribute_to_f,       0);
  rb_define_method(cSmartDomAttribute, "name",    (VALUE(*)(ANYARGS))rbxs_domattribute_name_get,   0);
  rb_define_method(cSmartDomAttribute, "path",    (VALUE(*)(ANYARGS))rbxs_domattribute_path,       0);
  rb_define_method(cSmartDomAttribute, "value",   (VALUE(*)(ANYARGS))rbxs_domattribute_value_get,  0);
  rb_define_method(cSmartDomAttribute, "value=",  (VALUE(*)(ANYARGS))rbxs_domattribute_value_set,  1);
  rb_define_method(cSmartDomAttribute, "element", (VALUE(*)(ANYARGS))rbxs_domattribute_element,    0);
}
