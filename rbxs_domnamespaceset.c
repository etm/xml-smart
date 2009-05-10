#include "rbxs_dom.h"
#include "rbxs_domelement.h"
#include "rbxs_domnamespace.h"
#include "rbxs_domnamespaceset.h"

#define XML_XMLNS_NAMESPACE ((xmlChar *) "http://www.w3.org/2000/xmlns/")

/* -- */
//***********************************************************************************
// GC
//***********************************************************************************
void rbxs_domnamespaceset_free(rbxs_domnamespaceset *prbxs_domnamespaceset) {
  if (prbxs_domnamespaceset != NULL) {
    free(prbxs_domnamespaceset);
  }  
}
  
void rbxs_domnamespaceset_mark(rbxs_domnamespaceset *prbxs_domnamespaceset) { 
  if (prbxs_domnamespaceset == NULL) return;
  if (!NIL_P(prbxs_domnamespaceset->node)) rb_gc_mark(prbxs_domnamespaceset->node);
}

//***********************************************************************************
// Methods
//***********************************************************************************
/* ++ */

/*
 *  Documentation
 */
VALUE rbxs_domnamespaceset_include(VALUE self, VALUE prefix)
{
  rbxs_domnamespaceset *prbxs_domnamespaceset;
  rbxs_domelement *prbxs_domelement;
  xmlNsPtr ns;
  unsigned char *cmp;

  Check_Type(prefix, T_STRING);
  Data_Get_Struct(self, rbxs_domnamespaceset, prbxs_domnamespaceset);
  Data_Get_Struct(prbxs_domnamespaceset->node, rbxs_domelement, prbxs_domelement);

  ns = prbxs_domelement->node->nsDef;
  cmp = (unsigned char *)StringValuePtr(prefix);
  while (ns != NULL) {
    if (xmlStrEqual(ns->prefix,cmp)) {
      return Qtrue;
    } 
    ns = ns->next;  
  }  
  return(Qfalse);
}

/*
 *  Documentation
 */
VALUE rbxs_domnamespaceset_length(VALUE self)
{
  rbxs_domnamespaceset *prbxs_domnamespaceset;
  rbxs_domelement *prbxs_domelement;
  int counter = 0;
  xmlNsPtr ns;

  Data_Get_Struct(self, rbxs_domnamespaceset, prbxs_domnamespaceset);
  Data_Get_Struct(prbxs_domnamespaceset->node, rbxs_domelement, prbxs_domelement);

  ns = prbxs_domelement->node->nsDef;
  while (ns != NULL) {
    counter++;
    ns = ns->next;  
  }  
  return(INT2NUM(counter));
}

/*
 *  Documentation
 */
VALUE rbxs_domnamespaceset_find(VALUE self,VALUE value)
{
  rbxs_domnamespaceset *prbxs_domnamespaceset;
  rbxs_domelement *prbxs_domelement;
  xmlNsPtr ns;
  
  Check_Type(value,T_STRING);
  Data_Get_Struct(self, rbxs_domnamespaceset, prbxs_domnamespaceset);
  Data_Get_Struct(prbxs_domnamespaceset->node, rbxs_domelement, prbxs_domelement);
  
  ns = xmlSearchNs(prbxs_domelement->node->doc, prbxs_domelement->node, (unsigned char *)StringValuePtr(value));
  if (ns)
    return rbxs_domnamespace_new(cSmartDomNamespace,self,ns);
  else {
    ns = xmlSearchNsByHref(prbxs_domelement->node->doc, prbxs_domelement->node, (unsigned char *)StringValuePtr(value));
    if (ns) return rbxs_domnamespace_new(cSmartDomNamespace,self,ns);
  }  
    
  return Qnil;
}

/*
 *  Documentation
 */
VALUE rbxs_domnamespaceset_get_ns(VALUE self, VALUE prefix)
{
  rbxs_domnamespaceset *prbxs_domnamespaceset;
  rbxs_domelement *prbxs_domelement;
  xmlNsPtr ns;
  unsigned char *cmp;

  Check_Type(prefix, T_STRING);
  Data_Get_Struct(self, rbxs_domnamespaceset, prbxs_domnamespaceset);
  Data_Get_Struct(prbxs_domnamespaceset->node, rbxs_domelement, prbxs_domelement);

  ns = prbxs_domelement->node->nsDef;
  cmp = (unsigned char *)StringValuePtr(prefix);
  while (ns != NULL) {
    if (xmlStrEqual(ns->prefix,cmp)) {
      return rbxs_domnamespace_new(cSmartDomNamespace, prbxs_domnamespaceset->node, ns);
    } 
    ns = ns->next;  
  }  
  return(Qnil);
}

/*
 *  Documentation
 */
VALUE rbxs_domnamespaceset_get(VALUE self, VALUE prefix)
{
  rbxs_domnamespaceset *prbxs_domnamespaceset;
  rbxs_domelement *prbxs_domelement;
  xmlNsPtr ns;
  unsigned char *cmp;

  Check_Type(prefix, T_STRING);
  Data_Get_Struct(self, rbxs_domnamespaceset, prbxs_domnamespaceset);
  Data_Get_Struct(prbxs_domnamespaceset->node, rbxs_domelement, prbxs_domelement);

  ns = prbxs_domelement->node->nsDef;
  cmp = (unsigned char *)StringValuePtr(prefix);
  while (ns != NULL) {
    if (xmlStrEqual(ns->prefix,cmp))
      return(rb_str_new2((char *)ns->href));
    ns = ns->next;  
  }  
  return(Qnil);
}

/*
 *  Documentation
 */
VALUE rbxs_domnamespaceset_set(VALUE self, VALUE prefix, VALUE href)
{
  rbxs_domnamespaceset *prbxs_domnamespaceset;
  rbxs_domelement *prbxs_domelement;
  rbxs_dom *prbxs_dom;
  unsigned char *t_href, *t_prefix;
  xmlNsPtr ns = NULL;
  unsigned short int type;
  VALUE ret,str;

  Check_Type(prefix, T_STRING);
  Data_Get_Struct(self, rbxs_domnamespaceset, prbxs_domnamespaceset);
  Data_Get_Struct(prbxs_domnamespaceset->node, rbxs_domelement, prbxs_domelement);
  Data_Get_Struct(prbxs_domelement->doc, rbxs_dom, prbxs_dom);
  
  str = rb_obj_as_string(href);
  if (NIL_P(str) || TYPE(str) != T_STRING)
    rb_raise(rb_eTypeError, "cannot convert href to string");
  t_href = (unsigned char *)StringValuePtr(str);
  if (!xmlCheckUTF8(t_href))
    rb_raise(rb_eArgError, "href must be utf8 encoded");
  
  str = rb_obj_as_string(prefix);
  if (NIL_P(str) || TYPE(str) != T_STRING)
    rb_raise(rb_eTypeError, "cannot convert prefix to string");
  t_prefix = (unsigned char *)StringValuePtr(str);
  if (!xmlCheckUTF8(t_prefix))
    rb_raise(rb_eArgError, "prefix must be utf8 encoded");

  type = RBXS_DOM_SIGNAL_CHANGE;
  if (t_href == NULL ||
    (!xmlStrEqual(t_prefix, (unsigned char *)"xml") && xmlStrEqual(t_href, XML_XML_NAMESPACE)) ||
    (prbxs_domelement->node->type == XML_ATTRIBUTE_NODE && !xmlStrEqual(t_prefix, (unsigned char *)"xmlns") && xmlStrEqual(t_href, XML_XMLNS_NAMESPACE)) ||
    (prbxs_domelement->node->type == XML_ATTRIBUTE_NODE && !xmlStrEqual(prbxs_domelement->node->name, (unsigned char *)"xmlns"))) {
    ns = NULL;
  } else {
    xmlNsPtr cmp;
    cmp = prbxs_domelement->node->nsDef;
    while (cmp != NULL) {
      if (xmlStrEqual(t_prefix, cmp->prefix)) {
        if (xmlStrEqual(t_href, cmp->href)) {
          ns = cmp;
          break;
        } else {
          xmlFree((char *) cmp->href);
          cmp->href = xmlStrdup(t_href);
          ns = cmp;
          break;
        }
      }
      cmp = cmp->next;
    }
    if (ns == NULL) {
      ns = xmlNewNs(prbxs_domelement->node,t_href,t_prefix);
      type = RBXS_DOM_SIGNAL_ADD;
    }
  }
  if (ns == NULL)
    rb_raise(rb_eRuntimeError, "Couldn't add namespace");
  xmlSetNs(prbxs_domelement->node, ns);

  ret = rbxs_domnamespace_new(cSmartDomNamespace,prbxs_domnamespaceset->node,ns);
  rbxs_dom_change_handlers_execute(prbxs_dom,type,ret);
  return(ret);
}  

/*
 *  Documentation
 */
VALUE rbxs_domnamespaceset_each(VALUE self)
{
  rbxs_domnamespaceset *prbxs_domnamespaceset;
  rbxs_domelement *prbxs_domelement;
  VALUE obj;
  xmlNsPtr ns;

  Data_Get_Struct(self, rbxs_domnamespaceset, prbxs_domnamespaceset);
  Data_Get_Struct(prbxs_domnamespaceset->node, rbxs_domelement, prbxs_domelement);
  
  ns = prbxs_domelement->node->nsDef;
  while (ns != NULL) {
    obj = rbxs_domnamespace_new(cSmartDomNamespace, prbxs_domnamespaceset->node, ns);
    rb_yield(obj);
    ns = ns->next;  
  }  
  return(self);
}

//***********************************************************************************
// Constructors
//***********************************************************************************
VALUE rbxs_domnamespaceset_new(VALUE class, VALUE node) {
  rbxs_domnamespaceset *prbxs_domnamespaceset;

  prbxs_domnamespaceset = (rbxs_domnamespaceset *)malloc(sizeof(rbxs_domnamespaceset));
  if (prbxs_domnamespaceset == NULL )
    rb_raise(rb_eNoMemError, "No memory left for XML::Smart::Dom::NamespaceSet struct");

  prbxs_domnamespaceset->node = node;

  return(Data_Wrap_Struct(class, rbxs_domnamespaceset_mark, rbxs_domnamespaceset_free, prbxs_domnamespaceset));
}

//***********************************************************************************
// Initialize class Node
//***********************************************************************************
#ifdef RDOC__
  mXML  = rb_define_module( "XML" );
  cSmart = rb_define_class_under( mXML, "Smart", rb_cObject );
  cSmartDom = rb_define_class_under( cSmart, "Dom", rb_cObject );
#endif
VALUE cSmartDomNamespaceSet;

void init_rbxs_domnamespaceset(void) {
  cSmartDomNamespaceSet = rb_define_class_under( cSmartDom, "NamespaceSet", rb_cObject );
  rb_include_module(cSmartDomNamespaceSet, rb_mEnumerable);

  rb_define_method(cSmartDomNamespaceSet, "has_ns?",  (VALUE(*)(ANYARGS))rbxs_domnamespaceset_include,   1);
  rb_define_method(cSmartDomNamespaceSet, "get_ns",   (VALUE(*)(ANYARGS))rbxs_domnamespaceset_get_ns,    1);
  rb_define_method(cSmartDomNamespaceSet, "include?", (VALUE(*)(ANYARGS))rbxs_domnamespaceset_include,   1);
  rb_define_method(cSmartDomNamespaceSet, "[]",       (VALUE(*)(ANYARGS))rbxs_domnamespaceset_get,       1);
  rb_define_method(cSmartDomNamespaceSet, "[]=",      (VALUE(*)(ANYARGS))rbxs_domnamespaceset_set,       2);
  rb_define_method(cSmartDomNamespaceSet, "add",      (VALUE(*)(ANYARGS))rbxs_domnamespaceset_set,       2);
  rb_define_method(cSmartDomNamespaceSet, "length",   (VALUE(*)(ANYARGS))rbxs_domnamespaceset_length,    0);
  rb_define_method(cSmartDomNamespaceSet, "each",     (VALUE(*)(ANYARGS))rbxs_domnamespaceset_each,      0);
  rb_define_method(cSmartDomNamespaceSet, "find",     (VALUE(*)(ANYARGS))rbxs_domnamespaceset_find,      1);
}
