#include "rbxs_dom.h"
#include "rbxs_domelement.h"
#include "rbxs_domattribute.h"
#include "rbxs_domattributeset.h"

/* -- */
//***********************************************************************************
// GC
//***********************************************************************************
void rbxs_domattributeset_free(rbxs_domattributeset *prbxs_domattributeset) {
  if (prbxs_domattributeset != NULL) {
    free(prbxs_domattributeset);
  }  
}
  
void rbxs_domattributeset_mark(rbxs_domattributeset *prbxs_domattributeset) { 
  if (prbxs_domattributeset == NULL) return;
  if (!NIL_P(prbxs_domattributeset->node)) rb_gc_mark(prbxs_domattributeset->node);
}

//***********************************************************************************
// Methods
//***********************************************************************************
/* ++ */

/*
 *  Documentation
 */
VALUE rbxs_domattributeset_include(VALUE self, VALUE key)
{
  rbxs_domattributeset *prbxs_domattributeset;
  rbxs_domelement *prbxs_domelement;
  Check_Type(key, T_STRING);
  Data_Get_Struct(self, rbxs_domattributeset, prbxs_domattributeset);
  Data_Get_Struct(prbxs_domattributeset->node, rbxs_domelement, prbxs_domelement);
  if (xmlHasProp(prbxs_domelement->node,(unsigned char *)StringValuePtr(key)))
    return(Qtrue);
  else
    return(Qfalse);
}

/*
 *  Documentation
 */
VALUE rbxs_domattributeset_get(VALUE self, VALUE key)
{
  rbxs_domattributeset *prbxs_domattributeset;
  rbxs_domelement *prbxs_domelement;
  xmlChar *ret;
  VALUE val;

  Check_Type(key, T_STRING);
  Data_Get_Struct(self, rbxs_domattributeset, prbxs_domattributeset);
  Data_Get_Struct(prbxs_domattributeset->node, rbxs_domelement, prbxs_domelement);

  ret = xmlGetProp(prbxs_domelement->node,(unsigned char *)StringValuePtr(key));
  if (ret) {
    val = rb_str_new2((char *)ret);
    xmlFree(ret);
    return(val);
  } else
    return(Qnil);
}

/*
 *  Documentation
 */
VALUE rbxs_domattributeset_set(VALUE self, VALUE key, VALUE value)
{
  rbxs_domattributeset *prbxs_domattributeset;
  rbxs_domelement *prbxs_domelement;
  rbxs_dom *prbxs_dom;
  xmlAttrPtr attr;
  unsigned char *tc;
  unsigned short int type;
  VALUE ret,str;

  Check_Type(key, T_STRING);
  Data_Get_Struct(self, rbxs_domattributeset, prbxs_domattributeset);
  Data_Get_Struct(prbxs_domattributeset->node, rbxs_domelement, prbxs_domelement);
  Data_Get_Struct(prbxs_domelement->doc, rbxs_dom, prbxs_dom);
  
  str = rb_obj_as_string(value);
  if (NIL_P(str) || TYPE(str) != T_STRING)
    rb_raise(rb_eTypeError, "cannot convert obj to string");
  tc = (unsigned char *)StringValuePtr(str);
  if (!xmlCheckUTF8((unsigned char *)tc))
    rb_raise(rb_eArgError, "text must be utf8 encoded");
  type = xmlHasProp(prbxs_domelement->node,(unsigned char *)StringValuePtr(key)) ? RBXS_DOM_SIGNAL_CHANGE : RBXS_DOM_SIGNAL_ADD;
    
  attr = xmlSetProp(prbxs_domelement->node,(unsigned char *)StringValuePtr(key),tc);
  if (attr == NULL)
    rb_raise(rb_eRuntimeError, "Couldn't set attribute");

  ret = rbxs_domattribute_new(cSmartDomAttribute, prbxs_domelement->doc, (xmlAttrPtr)attr);
  rbxs_dom_change_handlers_execute(prbxs_dom,type,ret);
  return(ret);
}  
  
/*
 *  Documentation
 */
VALUE rbxs_domattributeset_get_attr(VALUE self, VALUE key)
{
  rbxs_domattributeset *prbxs_domattributeset;
  rbxs_domelement *prbxs_domelement;
  xmlAttrPtr attr;

  Check_Type(key, T_STRING);
  Data_Get_Struct(self, rbxs_domattributeset, prbxs_domattributeset);
  Data_Get_Struct(prbxs_domattributeset->node, rbxs_domelement, prbxs_domelement);

  attr = prbxs_domelement->node->properties;
  while (attr != NULL) {
    if (xmlStrEqual(attr->name,BAD_CAST(StringValuePtr(key)))) {
      return rbxs_domattribute_new(cSmartDomAttribute, prbxs_domelement->doc, (xmlAttrPtr)attr);
    }
    attr = attr->next;  
  }  
  rb_raise(rb_eRuntimeError, "Couldn't get attribute");
  return(Qnil);
}

/*
 *  Documentation
 */
VALUE rbxs_domattributeset_each(VALUE self)
{
  rbxs_domattributeset *prbxs_domattributeset;
  rbxs_domelement *prbxs_domelement;
  VALUE obj;
  xmlAttrPtr attr;

  Data_Get_Struct(self, rbxs_domattributeset, prbxs_domattributeset);
  Data_Get_Struct(prbxs_domattributeset->node, rbxs_domelement, prbxs_domelement);

  attr = prbxs_domelement->node->properties;
  while (attr != NULL) {
    obj = rbxs_domattribute_new(cSmartDomAttribute, prbxs_domelement->doc, (xmlAttrPtr)attr);
    rb_yield(obj);
    attr = attr->next;  
  }  
  return(self);
}

//***********************************************************************************
// Constructors
//***********************************************************************************
VALUE rbxs_domattributeset_new(VALUE class, VALUE node) {
  rbxs_domattributeset *prbxs_domattributeset;

  prbxs_domattributeset = (rbxs_domattributeset *)malloc(sizeof(rbxs_domattributeset));
  if (prbxs_domattributeset == NULL )
    rb_raise(rb_eNoMemError, "No memory left for XML::Smart::Dom::AttributeSet struct");

  prbxs_domattributeset->node = node;

  return(Data_Wrap_Struct(class, rbxs_domattributeset_mark, rbxs_domattributeset_free, prbxs_domattributeset));
}

//***********************************************************************************
// Initialize class Node
//***********************************************************************************
#ifdef RDOC__
  mXML  = rb_define_module( "XML" );
  cSmart = rb_define_class_under( mXML, "Smart", rb_cObject );
  cSmartDom = rb_define_class_under( cSmart, "Dom", rb_cObject );
#endif
VALUE cSmartDomAttributeSet;

void init_rbxs_domattributeset(void) {
  cSmartDomAttributeSet = rb_define_class_under( cSmartDom, "AttributeSet", rb_cObject );
  rb_include_module(cSmartDomAttributeSet, rb_mEnumerable);

  rb_define_method(cSmartDomAttributeSet, "has_attr?", (VALUE(*)(ANYARGS))rbxs_domattributeset_include,       1);
  rb_define_method(cSmartDomAttributeSet, "get_attr",  (VALUE(*)(ANYARGS))rbxs_domattributeset_get_attr,      1);
  rb_define_method(cSmartDomAttributeSet, "include?",  (VALUE(*)(ANYARGS))rbxs_domattributeset_include,       1);
  rb_define_method(cSmartDomAttributeSet, "[]",        (VALUE(*)(ANYARGS))rbxs_domattributeset_get,           1);
  rb_define_method(cSmartDomAttributeSet, "[]=",       (VALUE(*)(ANYARGS))rbxs_domattributeset_set,           2);
  rb_define_method(cSmartDomAttributeSet, "add",       (VALUE(*)(ANYARGS))rbxs_domattributeset_set,           2);
  rb_define_method(cSmartDomAttributeSet, "each",      (VALUE(*)(ANYARGS))rbxs_domattributeset_each,          0);
}
