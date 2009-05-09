#include "rbxs_pullattributeset.h"
#include "rbxs_pullattribute.h"
#include "rbxs_qname.h"
#include "rbxs_pull.h"

/* -- */
//***********************************************************************************
// GC
//***********************************************************************************
void rbxs_pullattributeset_free(rbxs_pullattributeset *prbxs_pullattributeset)
{
  if (prbxs_pullattributeset != NULL) {
    free(prbxs_pullattributeset);
  }  
}
  
void rbxs_pullattributeset_mark(rbxs_pullattributeset *prbxs_pullattributeset)
{ 
  if (prbxs_pullattributeset == NULL) return;
  if (!NIL_P(prbxs_pullattributeset->node)) rb_gc_mark(prbxs_pullattributeset->node);
}

//***********************************************************************************
// Methods
//***********************************************************************************
/* ++ */

/* 
 * call-seq:
 *    include? => (true|false)
 *
 * Documentation
 */
VALUE rbxs_pullattributeset_include(VALUE self, VALUE which)
{
  rbxs_pullattributeset *prbxs_pullattributeset;
  rbxs_pull *prbxs_pull;
  Data_Get_Struct(self, rbxs_pullattributeset, prbxs_pullattributeset);
  Data_Get_Struct(prbxs_pullattributeset->node, rbxs_pull, prbxs_pull);

  Check_Type(which, T_STRING);

  if (xmlTextReaderMoveToAttribute(prbxs_pull->reader,(unsigned char *)StringValuePtr(which)))
    return(Qtrue);
  else
    return(Qfalse);
}  

/* 
 * Documentation
 */
VALUE rbxs_pullattributeset_empty(VALUE self)
{
  rbxs_pullattributeset *prbxs_pullattributeset;
  rbxs_pull *prbxs_pull;
  Data_Get_Struct(self, rbxs_pullattributeset, prbxs_pullattributeset);
  Data_Get_Struct(prbxs_pullattributeset->node, rbxs_pull, prbxs_pull);
  return(xmlTextReaderHasAttributes(prbxs_pull->reader) ? Qfalse : Qtrue);
}

/* 
 * Documentation
 */
VALUE rbxs_pullattributeset_length(VALUE self)
{
  rbxs_pullattributeset *prbxs_pullattributeset;
  rbxs_pull *prbxs_pull;
  Data_Get_Struct(self, rbxs_pullattributeset, prbxs_pullattributeset);
  Data_Get_Struct(prbxs_pullattributeset->node, rbxs_pull, prbxs_pull);
  return(NUM2INT(xmlTextReaderAttributeCount(prbxs_pull->reader)));
}

/* 
 * Documentation
 */
VALUE rbxs_pullattributeset_each(VALUE self)
{
  rbxs_pullattributeset *prbxs_pullattributeset;
  rbxs_pull *prbxs_pull;
  Data_Get_Struct(self, rbxs_pullattributeset, prbxs_pullattributeset);
  Data_Get_Struct(prbxs_pullattributeset->node, rbxs_pull, prbxs_pull);
  while (xmlTextReaderMoveToNextAttribute(prbxs_pull->reader)) {
    xmlChar *ret;
    VALUE val;
    ret = xmlTextReaderValue(prbxs_pull->reader);
    val = rb_str_new2((char *)ret);
    xmlFree(ret);
    rb_yield_values(2,rbxs_qname_new(cSmartQName,prbxs_pullattributeset->node,RBXS_PARSER_TYPE_READER),val);
  }  
  xmlTextReaderMoveToElement(prbxs_pull->reader);
  return(self);
}

/* 
 * Documentation
 */
VALUE rbxs_pullattributeset_get(VALUE self, VALUE which)
{
  rbxs_pullattributeset *prbxs_pullattributeset;
  Data_Get_Struct(self, rbxs_pullattributeset, prbxs_pullattributeset);
  return(rbxs_pullattribute_new(cSmartPullAttribute,prbxs_pullattributeset->node,which));
}
  
/* 
 * Documentation
 */
VALUE rbxs_pullattributeset_first(VALUE self)
{
  return rbxs_pullattributeset_get(self,INT2NUM(0)); 
}  

/* 
 * Documentation
 */
VALUE rbxs_pullattributeset_last(VALUE self)
{
  return rbxs_pullattributeset_get(self,INT2NUM(-1));
}  

//***********************************************************************************
// Constructors
//***********************************************************************************
VALUE rbxs_pullattributeset_new(VALUE class, VALUE node) {
  rbxs_pullattributeset *prbxs_pullattributeset;

  prbxs_pullattributeset = (rbxs_pullattributeset *)malloc(sizeof(rbxs_pullattributeset));
  if (prbxs_pullattributeset == NULL )
    rb_raise(rb_eNoMemError, "No memory left for XML::Smart::Pull::AttributeSet struct");

  prbxs_pullattributeset->node = node;

  return(Data_Wrap_Struct(class, rbxs_pullattributeset_mark, rbxs_pullattributeset_free, prbxs_pullattributeset));
}

//***********************************************************************************
// Initialize class Node
//***********************************************************************************
#ifdef RDOC__
  mXML  = rb_define_module( "XML" );
  cSmart = rb_define_class_under( mXML, "Smart", rb_cObject );
  cSmartPull = rb_define_class_under( cSmart, "Pull", rb_cObject );
#endif
VALUE cSmartPullAttributeSet;

void init_rbxs_pullattributeset(void) {
  cSmartPullAttributeSet = rb_define_class_under( cSmartPull, "AttributeSet", rb_cObject );
  rb_include_module(cSmartPullAttributeSet, rb_mEnumerable);

  rb_define_method(cSmartPullAttributeSet, "length",   (VALUE(*)(ANYARGS))rbxs_pullattributeset_length,  0);
  rb_define_method(cSmartPullAttributeSet, "empty?",   (VALUE(*)(ANYARGS))rbxs_pullattributeset_empty,   0);
  rb_define_method(cSmartPullAttributeSet, "each",     (VALUE(*)(ANYARGS))rbxs_pullattributeset_each,    0);
  rb_define_method(cSmartPullAttributeSet, "[]",       (VALUE(*)(ANYARGS))rbxs_pullattributeset_get,     1);
  rb_define_method(cSmartPullAttributeSet, "first",    (VALUE(*)(ANYARGS))rbxs_pullattributeset_first,   0);
  rb_define_method(cSmartPullAttributeSet, "last",     (VALUE(*)(ANYARGS))rbxs_pullattributeset_last,    0);
  rb_define_method(cSmartPullAttributeSet, "include?", (VALUE(*)(ANYARGS))rbxs_pullattributeset_include, 1);
}
