#include "rbxs_pullattribute.h"
#include "rbxs_pullattributeset.h"
#include "rbxs_qname.h"
#include "rbxs_pull.h"

/* -- */
//***********************************************************************************
// GC
//***********************************************************************************
void rbxs_pullattribute_free(rbxs_pullattribute *prbxs_pullattribute) {
  if (prbxs_pullattribute != NULL) {
    free(prbxs_pullattribute);
  }  
}
  
void rbxs_pullattribute_mark(rbxs_pullattribute *prbxs_pullattribute) { 
  if (prbxs_pullattribute == NULL) return;
  if (!NIL_P(prbxs_pullattribute->node)) rb_gc_mark(prbxs_pullattribute->node);
}

//***********************************************************************************
// Methods
//***********************************************************************************
/* ++ */
void searchPosition(xmlTextReaderPtr reader, VALUE which)
{
  int position,length;

  switch (TYPE(which)) {
    case T_STRING:
      if (!xmlTextReaderMoveToAttribute(reader,(unsigned char *)StringValuePtr(which))) {
        xmlTextReaderMoveToElement(reader);
        rb_raise(rb_eRuntimeError, "attribute could not be found");
      }  
      break;
    case T_FIXNUM:
      position = NUM2INT(which);
      length   = xmlTextReaderAttributeCount(reader);
       if (position < 0)
         position = length + position; 
       if ((position >= 0) && (position < length)) { 
         if (!xmlTextReaderMoveToAttributeNo(reader,position)) {
           xmlTextReaderMoveToElement(reader);
           rb_raise(rb_eRuntimeError, "weird things occured");
         }  
       } else 
         rb_raise(rb_eRuntimeError, "index out of range");
      break;
    default:
      rb_raise(rb_eArgError, "argument must me String or a Fixnum");
  }    
}

/* 
 * Documentation
 */
VALUE rbxs_pullattribute_name(VALUE self)
{
  rbxs_pullattribute *prbxs_pullattribute;
  rbxs_pull *prbxs_pull;

  Data_Get_Struct(self, rbxs_pullattribute, prbxs_pullattribute);
  Data_Get_Struct(prbxs_pullattribute->node, rbxs_pull, prbxs_pull);

  searchPosition(prbxs_pull->reader,prbxs_pullattribute->which);
  return(rbxs_qname_new(cSmartQName,prbxs_pullattribute->node,RBXS_PARSER_TYPE_READER));
}  

/* 
 * Documentation
 */
VALUE rbxs_pullattribute_value(VALUE self)
{
  rbxs_pullattribute *prbxs_pullattribute;
  rbxs_pull *prbxs_pull;
  unsigned char *value;

  Data_Get_Struct(self, rbxs_pullattribute, prbxs_pullattribute);
  Data_Get_Struct(prbxs_pullattribute->node, rbxs_pull, prbxs_pull);

  searchPosition(prbxs_pull->reader,prbxs_pullattribute->which);

  value = xmlTextReaderValue(prbxs_pull->reader);
  if (value) {
    VALUE retval = rb_str_new2((char *)value);
    xmlFree(value);
    return retval;
  } else
    return Qnil;
  return Qnil;
}

//***********************************************************************************
// Constructors
//***********************************************************************************
VALUE rbxs_pullattribute_new(VALUE class, VALUE node, VALUE which) {
  rbxs_pullattribute *prbxs_pullattribute;

  prbxs_pullattribute = (rbxs_pullattribute *)malloc(sizeof(rbxs_pullattribute));
  if (prbxs_pullattribute == NULL )
    rb_raise(rb_eNoMemError, "No memory left for XML::Smart::Pull::AttributeSet::Attribute struct");

  prbxs_pullattribute->node = node;
  prbxs_pullattribute->which = which;

  return(Data_Wrap_Struct(class, rbxs_pullattribute_mark, rbxs_pullattribute_free, prbxs_pullattribute));
}

//***********************************************************************************
// Initialize class Node
//***********************************************************************************
#ifdef RDOC__
  mXML  = rb_define_module( "XML" );
  cSmart = rb_define_class_under( mXML, "Smart", rb_cObject );
  cSmartPull = rb_define_class_under( cSmart, "Pull", rb_cObject );
  cSmartPullAttributeSet = rb_define_class_under( cSmartPull, "AttributeSet", rb_cObject );
#endif
VALUE cSmartPullAttribute;
void init_rbxs_pullattribute(void) {
  cSmartPullAttribute = rb_define_class_under( cSmartPullAttributeSet, "Attribute", rb_cObject );

  rb_define_method(cSmartPullAttribute, "name",  (VALUE(*)(ANYARGS))rbxs_pullattribute_name,  0);
  rb_define_method(cSmartPullAttribute, "value", (VALUE(*)(ANYARGS))rbxs_pullattribute_value, 0);
}
