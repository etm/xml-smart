#include "rbxs_dom.h"
#include "rbxs_domother.h"

/* -- */
//***********************************************************************************
// GC
//***********************************************************************************
void rbxs_domother_free(rbxs_domother *prbxs_domother) {
  if (prbxs_domother != NULL) {
    free(prbxs_domother);
  }  
}
  
void rbxs_domother_mark(rbxs_domother *prbxs_domother) { 
  if (prbxs_domother == NULL) return;
  if (!NIL_P(prbxs_domother->doc)) rb_gc_mark(prbxs_domother->doc);
}

//***********************************************************************************
// Methods
//***********************************************************************************
/* ++ */

/* 
 * Documentation
 */
VALUE rbxs_domother_parent(VALUE self)
{
  rbxs_domother *prbxs_domother;
  Data_Get_Struct(self, rbxs_domother, prbxs_domother);

  if (prbxs_domother->node != xmlDocGetRootElement(prbxs_domother->node->doc))
    return(rbxs_domother_new(cSmartDomOther, self, prbxs_domother->node->parent));
  else
    return(Qnil);
}

/* 
 * Documentation
 */
VALUE rbxs_domother_path(VALUE self)
{
  rbxs_domother *prbxs_domother;
  xmlChar *ret;
  VALUE val;

  Data_Get_Struct(self, rbxs_domother, prbxs_domother);
  ret = xmlGetNodePath(prbxs_domother->node);
  val = rb_str_new2((char *)ret);
  xmlFree(ret);
  return(val);
}

/* 
 * Documentation
 */
VALUE rbxs_domother_text_get(VALUE self)
{
  rbxs_domother *prbxs_domother;
  xmlBufferPtr result;
  VALUE ret;

  Data_Get_Struct(self, rbxs_domother, prbxs_domother);
  result = xmlBufferCreate();
  xmlNodeDump(result, prbxs_domother->node->doc, prbxs_domother->node, 0, 1);
  ret = rb_str_new2((char *)result->content);
  
  xmlBufferFree(result);
  return(ret);
}

/* 
 * Documentation
 */
VALUE rbxs_domother_inspect(VALUE self)
{
  VALUE *argv;

  argv = ALLOCA_N(VALUE, 4);
  argv[0] = rb_str_new2("#<%s:0x%x \"%s\">");
  argv[1] = CLASS_OF(self);
  argv[2] = rb_obj_id(self);
  argv[3] = rbxs_domother_text_get(self);
  return(rb_f_sprintf(4, argv));
}

//***********************************************************************************
// Constructors
//***********************************************************************************
VALUE rbxs_domother_new(VALUE class, VALUE doc, xmlNodePtr node) {
  rbxs_domother *prbxs_domother;

  prbxs_domother = (rbxs_domother *)malloc(sizeof(rbxs_domother));
  if (prbxs_domother == NULL )
    rb_raise(rb_eNoMemError, "No memory left for XML::Smart::Dom::Other struct");

  prbxs_domother->doc  = doc;
  prbxs_domother->node = node;

  return(Data_Wrap_Struct(class, rbxs_domother_mark, rbxs_domother_free, prbxs_domother));
}

//***********************************************************************************
// Initialize class Node
//***********************************************************************************
#ifdef RDOC__
  mXML  = rb_define_module( "XML" );
  cSmart = rb_define_class_under( mXML, "Smart", rb_cObject );
  cSmartDom = rb_define_class_under( cSmart, "Dom", rb_cObject );
#endif
VALUE cSmartDomOther;

void init_rbxs_domother(void) {
  cSmartDomOther = rb_define_class_under( cSmartDom, "Other", rb_cObject );

  rb_define_method(cSmartDomOther, "inspect", (VALUE(*)(ANYARGS))rbxs_domother_inspect,   0);
  rb_define_method(cSmartDomOther, "to_s",    (VALUE(*)(ANYARGS))rbxs_domother_text_get,  0);
  rb_define_method(cSmartDomOther, "text",    (VALUE(*)(ANYARGS))rbxs_domother_text_get,  0);
  rb_define_method(cSmartDomOther, "parent",  (VALUE(*)(ANYARGS))rbxs_domother_parent,    0);
  rb_define_method(cSmartDomOther, "path",    (VALUE(*)(ANYARGS))rbxs_domother_path,      0);
}
