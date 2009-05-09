#include "rbxs_qname.h"
#include "rbxs_domelement.h"
#include "rbxs_pull.h"

/* -- */
//***********************************************************************************
// GC
//***********************************************************************************
void rbxs_qname_free(rbxs_qname *prbxs_qname) {
  if (prbxs_qname != NULL) {
    free(prbxs_qname);
  }  
}
  
void rbxs_qname_mark(rbxs_qname *prbxs_qname) { 
  if (prbxs_qname == NULL) return;
  if (!NIL_P(prbxs_qname->obj)) rb_gc_mark(prbxs_qname->obj);
}

//***********************************************************************************
// Methods
//***********************************************************************************
/* ++ */

/* 
 * Documentation
 */
VALUE rbxs_qname_inspect(VALUE self)
{
  rbxs_qname *prbxs_qname;
  rbxs_domelement *prbxs_domelement;
  rbxs_pull *prbxs_pull;
  unsigned char *ret;
  VALUE *argv;

  Data_Get_Struct(self, rbxs_qname, prbxs_qname);
  switch (prbxs_qname->type) {
    case RBXS_PARSER_TYPE_DOM:
      Data_Get_Struct(prbxs_qname->obj, rbxs_domelement, prbxs_domelement);
      if (prbxs_domelement->node->ns != NULL && prbxs_domelement->node->ns->prefix != NULL) {
        argv = ALLOCA_N(VALUE, 5);
        argv[0] = rb_str_new2("#<%s:0x%x \"%s:%s\">");
        argv[1] = CLASS_OF(self);
        argv[2] = rb_obj_id(self);
        argv[3] = rb_str_new2((char *)prbxs_domelement->node->ns->prefix);
        argv[4] = rb_str_new2((char *)prbxs_domelement->node->name);
        return(rb_f_sprintf(5, argv));
      } else {
        argv = ALLOCA_N(VALUE, 4);
        argv[0] = rb_str_new2("#<%s:0x%x \"%s\">");
        argv[1] = CLASS_OF(self);
        argv[2] = rb_obj_id(self);
        argv[3] = rb_str_new2((char *)prbxs_domelement->node->name);
        return(rb_f_sprintf(4, argv));
      }  
      break;
    case RBXS_PARSER_TYPE_READER:
      Data_Get_Struct(prbxs_qname->obj, rbxs_pull, prbxs_pull);
      ret = xmlTextReaderName(prbxs_pull->reader);
      if (ret) {
        argv = ALLOCA_N(VALUE, 4);
        argv[0] = rb_str_new2("#<%s:0x%x \"%s\">");
        argv[1] = CLASS_OF(self);
        argv[2] = rb_obj_id(self);
        argv[3] = rb_str_new2((char *)ret);
        xmlFree(ret);
        return(rb_f_sprintf(4, argv));
      } else
        return(Qnil);
  }   
  return(Qnil);
}

/* 
 * Documentation
 */
VALUE rbxs_qname_to_s(VALUE self)
{
  rbxs_qname *prbxs_qname;
  rbxs_domelement *prbxs_domelement;
  rbxs_pull *prbxs_pull;
  unsigned char *ret;
  VALUE *argv, val;

  Data_Get_Struct(self, rbxs_qname, prbxs_qname);
  switch (prbxs_qname->type) {
    case RBXS_PARSER_TYPE_DOM:
      Data_Get_Struct(prbxs_qname->obj, rbxs_domelement, prbxs_domelement);
      if (prbxs_domelement->node->ns != NULL && prbxs_domelement->node->ns->prefix != NULL) {
        argv = ALLOCA_N(VALUE, 3);
        argv[0] = rb_str_new2("%s:%s");
        argv[1] = rb_str_new2((char *)prbxs_domelement->node->ns->prefix);
        argv[2] = rb_str_new2((char *)prbxs_domelement->node->name);
        return(rb_f_sprintf(3, argv));
      } else {
        return rb_str_new2((char *)prbxs_domelement->node->name);
      }  
      break;
    case RBXS_PARSER_TYPE_READER:
      Data_Get_Struct(prbxs_qname->obj, rbxs_pull, prbxs_pull);
      ret = xmlTextReaderName(prbxs_pull->reader);
      if (ret) {
        val = rb_str_new2((char *)ret);
        xmlFree(ret);
        return(val);
      } else
        return(Qnil);
  }   
  return(Qnil);
}

/* 
 * Documentation
 */
VALUE rbxs_qname_name(VALUE self)
{
  rbxs_qname *prbxs_qname;
  rbxs_domelement *prbxs_domelement;
  rbxs_pull *prbxs_pull;
  xmlChar *ret;
  VALUE val;
  
  Data_Get_Struct(self, rbxs_qname, prbxs_qname);
  switch (prbxs_qname->type) {
    case RBXS_PARSER_TYPE_DOM:
      Data_Get_Struct(prbxs_qname->obj, rbxs_domelement, prbxs_domelement);
      return(rb_str_new2((char *)prbxs_domelement->node->name));
    case RBXS_PARSER_TYPE_READER:
      Data_Get_Struct(prbxs_qname->obj, rbxs_pull, prbxs_pull);
      ret = xmlTextReaderLocalName(prbxs_pull->reader);
      if (ret) {
        val = rb_str_new2((char *)ret);
        xmlFree(ret);
        return(val);
      } else
        return(Qnil);
  }   
  return(Qnil);
}

/* 
 * Documentation
 */
VALUE rbxs_qname_streq(VALUE self,VALUE str)
{
  rbxs_qname *prbxs_qname;
  rbxs_domelement *prbxs_domelement;
  rbxs_pull *prbxs_pull;
  
  Data_Get_Struct(self, rbxs_qname, prbxs_qname);
  Check_Type(str, T_STRING);
  switch (prbxs_qname->type) {
    case RBXS_PARSER_TYPE_DOM:
      Data_Get_Struct(prbxs_qname->obj, rbxs_domelement, prbxs_domelement);
      if (strcmp((char *)prbxs_domelement->node->name,StringValuePtr(str)) == 0)
        return(Qtrue);
      else
        return(Qfalse);
    case RBXS_PARSER_TYPE_READER:
      Data_Get_Struct(prbxs_qname->obj, rbxs_pull, prbxs_pull);
      if (strcmp((char *)xmlTextReaderLocalName(prbxs_pull->reader),StringValuePtr(str)) == 0)
        return(Qtrue);
      else
        return(Qfalse);
  }   
  return(Qnil);
}

/* 
 * Documentation
 */
VALUE rbxs_qname_prefix(VALUE self)
{
  rbxs_qname *prbxs_qname;
  rbxs_domelement *prbxs_domelement;
  rbxs_pull *prbxs_pull;
  xmlChar *ret;
  VALUE val;
  
  Data_Get_Struct(self, rbxs_qname, prbxs_qname);
  switch (prbxs_qname->type) {
    case RBXS_PARSER_TYPE_DOM:
      Data_Get_Struct(prbxs_qname->obj, rbxs_domelement, prbxs_domelement);
      if (prbxs_domelement->node->ns != NULL && prbxs_domelement->node->ns->prefix != NULL)
        return(rb_str_new2((char *)prbxs_domelement->node->ns->prefix));
      else
        return(Qnil);
    case RBXS_PARSER_TYPE_READER:
      Data_Get_Struct(prbxs_qname->obj, rbxs_pull, prbxs_pull);
      ret = xmlTextReaderPrefix(prbxs_pull->reader);
      if (ret) {
        val = rb_str_new2((char *)ret);
        xmlFree(ret);
        return(val);
      } else
        return(Qnil);
  }   
  return Qnil;
}

/* 
 * Documentation
 */
VALUE rbxs_qname_namespace(VALUE self)
{
  rbxs_qname *prbxs_qname;
  rbxs_domelement *prbxs_domelement;
  rbxs_pull *prbxs_pull;
  xmlChar *ret;
  VALUE val;
  
  Data_Get_Struct(self, rbxs_qname, prbxs_qname);
  switch (prbxs_qname->type) {
    case RBXS_PARSER_TYPE_DOM:
      Data_Get_Struct(prbxs_qname->obj, rbxs_domelement, prbxs_domelement);
      if (prbxs_domelement->node->ns != NULL && prbxs_domelement->node->ns->href != NULL)
        return(rb_str_new2((char *)prbxs_domelement->node->ns->href));
      else  
        return(Qnil);
    case RBXS_PARSER_TYPE_READER:
      Data_Get_Struct(prbxs_qname->obj, rbxs_pull, prbxs_pull);
      ret = xmlTextReaderNamespaceUri(prbxs_pull->reader);
      if (ret) {
        val = rb_str_new2((char *)ret);
        xmlFree(ret);
        return(val);
      } else
        return(Qnil);
  }   
  return(Qnil);
}

//***********************************************************************************
// Constructors
//***********************************************************************************
VALUE rbxs_qname_new(VALUE class, VALUE obj, int type) {
  rbxs_qname *prbxs_qname;

  prbxs_qname = (rbxs_qname *)malloc(sizeof(rbxs_qname));
  if (prbxs_qname == NULL )
    rb_raise(rb_eNoMemError, "No memory left for XML::Smart::QName struct");

  prbxs_qname->obj  = obj;
  prbxs_qname->type = type;

  return(Data_Wrap_Struct(class, rbxs_qname_mark, rbxs_qname_free, prbxs_qname));
}

//***********************************************************************************
// Initialize class Node
//***********************************************************************************
#ifdef RDOC__
  mXML  = rb_define_module( "XML" );
  cSmart = rb_define_class_under( mXML, "Smart", rb_cObject );
#endif
VALUE cSmartQName;

void init_rbxs_qname(void) {
  cSmartQName = rb_define_class_under( cSmart, "QName", rb_cObject );

  rb_define_method(cSmartQName, "inspect",   (VALUE(*)(ANYARGS))rbxs_qname_inspect,  0);
  rb_define_method(cSmartQName, "to_s",      (VALUE(*)(ANYARGS))rbxs_qname_to_s,     0);
  rb_define_method(cSmartQName, "==",        (VALUE(*)(ANYARGS))rbxs_qname_streq,    1);
  rb_define_method(cSmartQName, "name",      (VALUE(*)(ANYARGS))rbxs_qname_name,     0);
  rb_define_method(cSmartQName, "prefix",    (VALUE(*)(ANYARGS))rbxs_qname_prefix,   0);
  rb_define_method(cSmartQName, "namespace", (VALUE(*)(ANYARGS))rbxs_qname_namespace,0);
}
