#include "rbxs_pull.h"
#include "rbxs_qname.h"
#include "rbxs_pullattributeset.h"

/* -- */
//***********************************************************************************
// GC
//***********************************************************************************
void rbxs_pull_free(rbxs_pull *prbxs_pull) {
  if (prbxs_pull != NULL) {
    xmlFreeTextReader(prbxs_pull->reader);
    prbxs_pull->reader = NULL;
    free(prbxs_pull);
  }  
}
  
void rbxs_pull_mark(rbxs_pull *prbxs_pull) { }

//***********************************************************************************
// Methods
//***********************************************************************************
/* ++ */

/* 
 * Documentation
 */
VALUE rbxs_pull_next(VALUE self)
{
  rbxs_pull *prbxs_pull;
  Data_Get_Struct(self, rbxs_pull, prbxs_pull);
  if (xmlTextReaderRead(prbxs_pull->reader) >= 1)
    return self;
  else
    return Qnil;
}  
    
/* 
 * Documentation
 */
VALUE rbxs_pull_name(VALUE self)
{
  return(rbxs_qname_new(cSmartQName,self,RBXS_PARSER_TYPE_READER));
}

/* 
 * Documentation
 */
VALUE rbxs_pull_base(VALUE self)
{
  unsigned char *base_uri;
  rbxs_pull *prbxs_pull;

  Data_Get_Struct(self, rbxs_pull, prbxs_pull);
  base_uri = xmlTextReaderBaseUri(prbxs_pull->reader);
  if (base_uri)
    return(rb_str_new2((char *)base_uri));
  else
    return(Qnil);
}

/* 
 * Documentation
 */
VALUE rbxs_pull_xml_lang(VALUE self)
{
  unsigned char *xml_lang;
  rbxs_pull *prbxs_pull;

  Data_Get_Struct(self, rbxs_pull, prbxs_pull);
  xml_lang = xmlTextReaderXmlLang(prbxs_pull->reader);
  if (xml_lang)
    return(rb_str_new2((char *)xml_lang));
  else
    return(Qnil);
}

/* 
 * Documentation
 */
VALUE rbxs_pull_depth(VALUE self)
{
  int depth;
  rbxs_pull *prbxs_pull;

  Data_Get_Struct(self, rbxs_pull, prbxs_pull);
  depth = xmlTextReaderDepth(prbxs_pull->reader);
  if (depth >= 0)
    return(INT2FIX(depth));
  else
    return(Qnil);
}

/* 
 * Documentation
 */
VALUE rbxs_pull_attributes(VALUE self)
{
  return rbxs_pullattributeset_new(cSmartPullAttributeSet,self);
}

/* 
 * Documentation
 */
VALUE rbxs_pull_emptyp(VALUE self)
{
  rbxs_pull *prbxs_pull;

  Data_Get_Struct(self, rbxs_pull, prbxs_pull);  
  return (xmlTextReaderIsEmptyElement(prbxs_pull->reader) ? Qtrue : Qfalse);
}

/* 
 * Documentation
 */
VALUE rbxs_pull_valuep(VALUE self)
{
  rbxs_pull *prbxs_pull;

  Data_Get_Struct(self, rbxs_pull, prbxs_pull);  
  return (xmlTextReaderHasValue(prbxs_pull->reader) ? Qtrue : Qfalse);
}

/* 
 * Documentation
 */
VALUE rbxs_pull_value(VALUE self)
{
  rbxs_pull *prbxs_pull;
  unsigned char *value;

  Data_Get_Struct(self, rbxs_pull, prbxs_pull);  
  value = xmlTextReaderValue(prbxs_pull->reader);

  if (value) {
    VALUE retval = rb_str_new2((char *)value);
    xmlFree(value);
    return retval;
  } else
    return Qnil;
}

static VALUE type2symbol[18];

/* 
 * Documentation
 */
VALUE rbxs_pull_node_type(VALUE self)
{
  rbxs_pull *prbxs_pull;
  int node_type;

  Data_Get_Struct(self, rbxs_pull, prbxs_pull);  
  node_type = xmlTextReaderNodeType(prbxs_pull->reader);

  if (node_type < 0)
    return(Qnil);
  
  return ID2SYM(type2symbol[node_type]);
}

//***********************************************************************************
// Constructors
//***********************************************************************************
VALUE rbxs_pull_new(VALUE class, VALUE what) {
  rbxs_pull *prbxs_pull;
  RB_IO_T *fptr;

  prbxs_pull = (rbxs_pull *)malloc(sizeof(rbxs_pull));
  if (prbxs_pull == NULL )
    rb_raise(rb_eNoMemError, "No memory left for XML::Smart::Pull struct");

  switch (TYPE(what)) {
    case T_STRING:
      prbxs_pull->reader = xmlReaderForMemory(RSTRING_PTR(what),
                   RSTRING_LEN(what),
               NULL, NULL, 0);
      break;
    case T_FILE:
      GetOpenFile(what,fptr);
      prbxs_pull->reader = xmlReaderForFd(RB_IO_T_FD(fptr),NULL,NULL,0);
      break;
    default:
      rb_raise(rb_eArgError, "argument must me String or File");
  }

  if (prbxs_pull->reader == NULL)
    rb_raise(rb_eRuntimeError, "Document could not be opend");

  if (rb_block_given_p()) {
    VALUE self;
    self = Data_Wrap_Struct(class, rbxs_pull_mark, rbxs_pull_free, prbxs_pull);
    while (xmlTextReaderRead(prbxs_pull->reader)) {
      rb_yield(self);
    }
    return(Qnil);
  }

  return(Data_Wrap_Struct(class, rbxs_pull_mark, rbxs_pull_free, prbxs_pull));
}

//***********************************************************************************
// Initialize class Pull
//***********************************************************************************
#ifdef RDOC__
  mXML  = rb_define_module( "XML" );
  cSmart = rb_define_class_under( mXML, "Smart", rb_cObject );
#endif
VALUE cSmartPull;

void init_rbxs_pull(void) {
  cSmartPull = rb_define_class_under( cSmart, "Pull", rb_cObject );
  
  type2symbol[XML_READER_TYPE_NONE]                   = rb_intern("none");
  type2symbol[XML_READER_TYPE_ELEMENT]                = rb_intern("element");
  type2symbol[XML_READER_TYPE_ATTRIBUTE]              = rb_intern("attribute");
  type2symbol[XML_READER_TYPE_TEXT]                   = rb_intern("text");
  type2symbol[XML_READER_TYPE_CDATA]                  = rb_intern("cdata");
  type2symbol[XML_READER_TYPE_ENTITY_REFERENCE]       = rb_intern("entity_reference");
  type2symbol[XML_READER_TYPE_ENTITY]                 = rb_intern("entity");
  type2symbol[XML_READER_TYPE_PROCESSING_INSTRUCTION] = rb_intern("processing_instruction");
  type2symbol[XML_READER_TYPE_COMMENT]                = rb_intern("comment");
  type2symbol[XML_READER_TYPE_DOCUMENT]               = rb_intern("document");
  type2symbol[XML_READER_TYPE_DOCUMENT_TYPE]          = rb_intern("document_type");
  type2symbol[XML_READER_TYPE_DOCUMENT_FRAGMENT]      = rb_intern("document_fragment");
  type2symbol[XML_READER_TYPE_NOTATION]               = rb_intern("notation");
  type2symbol[XML_READER_TYPE_WHITESPACE]             = rb_intern("whitespace");
  type2symbol[XML_READER_TYPE_SIGNIFICANT_WHITESPACE] = rb_intern("significant_whitespace");
  type2symbol[XML_READER_TYPE_END_ELEMENT]            = rb_intern("end_element");
  type2symbol[XML_READER_TYPE_END_ENTITY]             = rb_intern("end_entity");
  type2symbol[XML_READER_TYPE_XML_DECLARATION]        = rb_intern("xml_declaration");
  
  rb_define_method(cSmartPull, "to_s",       (VALUE(*)(ANYARGS))rbxs_pull_value,      0);
  rb_define_method(cSmartPull, "next",       (VALUE(*)(ANYARGS))rbxs_pull_next,       0);
  rb_define_method(cSmartPull, "name",       (VALUE(*)(ANYARGS))rbxs_pull_name,       0);

  rb_define_method(cSmartPull, "base",       (VALUE(*)(ANYARGS))rbxs_pull_base,       0);
  rb_define_method(cSmartPull, "depth",      (VALUE(*)(ANYARGS))rbxs_pull_depth,      0);
  rb_define_method(cSmartPull, "attributes", (VALUE(*)(ANYARGS))rbxs_pull_attributes, 0);
  rb_define_method(cSmartPull, "value?",     (VALUE(*)(ANYARGS))rbxs_pull_valuep,     0);
  rb_define_method(cSmartPull, "value",      (VALUE(*)(ANYARGS))rbxs_pull_value,      0);
  rb_define_method(cSmartPull, "empty?",     (VALUE(*)(ANYARGS))rbxs_pull_emptyp,     0);
  rb_define_method(cSmartPull, "lang",       (VALUE(*)(ANYARGS))rbxs_pull_xml_lang,   0);
  rb_define_method(cSmartPull, "node_type",  (VALUE(*)(ANYARGS))rbxs_pull_node_type,  0);
}
