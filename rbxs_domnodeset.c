#include "rbxs_dom.h"
#include "rbxs_domelement.h"
#include "rbxs_domtext.h"
#include "rbxs_domother.h"
#include "rbxs_domnodeset.h"
#include "rbxs_domattribute.h"

/* -- */
//***********************************************************************************
// GC
//***********************************************************************************
void rbxs_domnodeset_free(rbxs_domnodeset *prbxs_domnodeset) {
  if (prbxs_domnodeset != NULL) {
    xmlXPathFreeObject(prbxs_domnodeset->obj);
    prbxs_domnodeset->obj = NULL;
    prbxs_domnodeset->nodeset = NULL;
    free(prbxs_domnodeset);
  }  
}
  
void rbxs_domnodeset_mark(rbxs_domnodeset *prbxs_domnodeset) { 
  if (prbxs_domnodeset == NULL) return;
  if (!NIL_P(prbxs_domnodeset->doc)) rb_gc_mark(prbxs_domnodeset->doc);
}

//***********************************************************************************
// Methods
//***********************************************************************************
/* ++ */
VALUE createFromType(rbxs_domnodeset *prbxs_domnodeset, long num) {
  if (prbxs_domnodeset->nodeset->nodeTab[num] == NULL) return (Qnil);
  switch(prbxs_domnodeset->nodeset->nodeTab[num]->type) {
    case XML_TEXT_NODE:
      return rbxs_domtext_new(cSmartDomText, prbxs_domnodeset->doc, prbxs_domnodeset->nodeset->nodeTab[num]);
    case XML_ATTRIBUTE_NODE:
      return rbxs_domattribute_new(cSmartDomAttribute, prbxs_domnodeset->doc, (xmlAttrPtr)prbxs_domnodeset->nodeset->nodeTab[num]);
    case XML_ELEMENT_NODE:
      return rbxs_domelement_new(cSmartDomElement, prbxs_domnodeset->doc, prbxs_domnodeset->nodeset->nodeTab[num]);
    default:
      return rbxs_domother_new(cSmartDomOther, prbxs_domnodeset->doc, prbxs_domnodeset->nodeset->nodeTab[num]);
  }
}

/*
 *  Documentation
 */
VALUE rbxs_domnodeset_length(VALUE self)
{
  rbxs_domnodeset *prbxs_domnodeset;
  Data_Get_Struct(self, rbxs_domnodeset, prbxs_domnodeset);
  if (xmlXPathNodeSetIsEmpty(prbxs_domnodeset->nodeset))
    return(INT2NUM(0));
  return(INT2NUM(prbxs_domnodeset->nodeset->nodeNr));
}

/*
 *  Documentation
 */
VALUE rbxs_domnodeset_nitems(VALUE self)
{
  rbxs_domnodeset *prbxs_domnodeset;
  int i,ret = 0;

  Data_Get_Struct(self, rbxs_domnodeset, prbxs_domnodeset);
  if (xmlXPathNodeSetIsEmpty(prbxs_domnodeset->nodeset))
    return(INT2NUM(0));

  for (i = 0; i < prbxs_domnodeset->nodeset->nodeNr; i++) {
    if (prbxs_domnodeset->nodeset->nodeTab[i] != NULL) ret += 1;
  }    
  return(INT2NUM(ret));
}

/*
 *  Documentation
 */
VALUE rbxs_domnodeset_empty_q(VALUE self)
{
  rbxs_domnodeset *prbxs_domnodeset;
  Data_Get_Struct(self, rbxs_domnodeset, prbxs_domnodeset);
  return (xmlXPathNodeSetIsEmpty(prbxs_domnodeset->nodeset) ? Qtrue : Qfalse);
}

/*
 *  Documentation
 */
VALUE rbxs_domnodeset_get(VALUE self, VALUE num)
{
  rbxs_domnodeset *prbxs_domnodeset;

  Check_Type(num, T_FIXNUM);
  Data_Get_Struct(self, rbxs_domnodeset, prbxs_domnodeset);

  if (xmlXPathNodeSetIsEmpty(prbxs_domnodeset->nodeset))
    return(Qnil);

  if (NUM2INT(num) > -1) {
    if (prbxs_domnodeset->nodeset->nodeNr > NUM2INT(num))
      return createFromType(prbxs_domnodeset,NUM2INT(num));
  } else {
    if (prbxs_domnodeset->nodeset->nodeNr + NUM2INT(num) > -1)
      return createFromType(prbxs_domnodeset,prbxs_domnodeset->nodeset->nodeNr + NUM2INT(num));
  }
  
  return(Qnil);
}

/*
 *  Documentation
 */
VALUE rbxs_domnodeset_first(VALUE self)
{
  return(rbxs_domnodeset_get(self,INT2NUM(0)));
}

/*
 *  Documentation
 */
VALUE rbxs_domnodeset_last(VALUE self)
{
  return(rbxs_domnodeset_get(self,INT2NUM(-1)));
}

/*
 *  Documentation
 */
VALUE rbxs_domnodeset_delete_all(VALUE self)
{
  rbxs_domnodeset *prbxs_domnodeset;
  rbxs_dom *prbxs_dom;
  long i;

  Data_Get_Struct(self, rbxs_domnodeset, prbxs_domnodeset);
  Data_Get_Struct(prbxs_domnodeset->doc, rbxs_dom, prbxs_dom);

  if (xmlXPathNodeSetIsEmpty(prbxs_domnodeset->nodeset))
    return(Qfalse);

  for (i = 0; i < prbxs_domnodeset->nodeset->nodeNr; i++)
    if (prbxs_domnodeset->nodeset->nodeTab[i] != NULL) {
      rbxs_dom_change_handlers_execute(prbxs_dom,RBXS_DOM_SIGNAL_DELETE,createFromType(prbxs_domnodeset,i));
      xmlUnlinkNode(prbxs_domnodeset->nodeset->nodeTab[i]);
      xmlFreeNode  (prbxs_domnodeset->nodeset->nodeTab[i]);
      prbxs_domnodeset->nodeset->nodeTab[i] = NULL;
    }
  
  return(Qtrue);
}

/*
 *  Documentation
 */
VALUE rbxs_domnodeset_delete_if(VALUE self)
{
  rbxs_domnodeset *prbxs_domnodeset;
  rbxs_dom *prbxs_dom;
  VALUE obj;
  long i;
  unsigned short int success = 0;

  Data_Get_Struct(self, rbxs_domnodeset, prbxs_domnodeset);
  Data_Get_Struct(prbxs_domnodeset->doc, rbxs_dom, prbxs_dom);

  if (xmlXPathNodeSetIsEmpty(prbxs_domnodeset->nodeset))
    return(Qfalse);

  for (i = 0; i < prbxs_domnodeset->nodeset->nodeNr; i++) {
    obj = createFromType(prbxs_domnodeset,i);
    if (TYPE(rb_yield(obj)) == T_TRUE) {
      rbxs_dom_change_handlers_execute(prbxs_dom,RBXS_DOM_SIGNAL_DELETE,createFromType(prbxs_domnodeset,i));
      xmlUnlinkNode(prbxs_domnodeset->nodeset->nodeTab[i]);
      xmlFreeNode  (prbxs_domnodeset->nodeset->nodeTab[i]);
      prbxs_domnodeset->nodeset->nodeTab[i] = NULL;
      success = 1;
    }
  }

  return(success ? Qtrue : Qfalse);
}

/*
 *  Documentation
 */
VALUE rbxs_domnodeset_delete_at(VALUE self, VALUE num)
{
  rbxs_domnodeset *prbxs_domnodeset;
  long i, j;
  
  Check_Type(num, T_FIXNUM);
  Data_Get_Struct(self, rbxs_domnodeset, prbxs_domnodeset);

  if (xmlXPathNodeSetIsEmpty(prbxs_domnodeset->nodeset))
    return(Qfalse);

  j = NUM2INT(num);
  if (j > -1) {
    if (prbxs_domnodeset->nodeset->nodeNr <= j)
      return(Qfalse);
  } else {
    if (prbxs_domnodeset->nodeset->nodeNr + j < 0)
      return(Qfalse);
    else
      j += prbxs_domnodeset->nodeset->nodeNr;
  }
  for (i = 0; i < prbxs_domnodeset->nodeset->nodeNr; i++)
    if ((j == i) && (prbxs_domnodeset->nodeset->nodeTab[i] != NULL)) {
      rbxs_dom *prbxs_dom;
      Data_Get_Struct(prbxs_domnodeset->doc, rbxs_dom, prbxs_dom);
      rbxs_dom_change_handlers_execute(prbxs_dom,RBXS_DOM_SIGNAL_DELETE,createFromType(prbxs_domnodeset,i));

      xmlUnlinkNode(prbxs_domnodeset->nodeset->nodeTab[i]);
      xmlFreeNode  (prbxs_domnodeset->nodeset->nodeTab[i]);
      prbxs_domnodeset->nodeset->nodeTab[i] = NULL;
      return(Qtrue);
    }
  
  return(Qfalse);
}

/*
 *  Documentation
 */
VALUE rbxs_domnodeset_each(VALUE self)
{
  rbxs_domnodeset *prbxs_domnodeset;
  VALUE obj;
  int i;

  Data_Get_Struct(self, rbxs_domnodeset, prbxs_domnodeset);

  if (xmlXPathNodeSetIsEmpty(prbxs_domnodeset->nodeset))
    return(Qnil);

  for (i = 0; i < prbxs_domnodeset->nodeset->nodeNr; i++) {
    obj = createFromType(prbxs_domnodeset,i);
    rb_yield(obj);
  }

  return(self);
}

//***********************************************************************************
// Constructors
//***********************************************************************************
VALUE rbxs_domnodeset_new(VALUE class, VALUE doc, xmlXPathObjectPtr obj) {
  rbxs_domnodeset *prbxs_domnodeset;

  prbxs_domnodeset = (rbxs_domnodeset *)malloc(sizeof(rbxs_domnodeset));
  if (prbxs_domnodeset == NULL )
    rb_raise(rb_eNoMemError, "No memory left for XML::Smart::NodeSet struct");

  prbxs_domnodeset->doc     = doc;
  prbxs_domnodeset->obj     = obj;
  prbxs_domnodeset->nodeset = obj->nodesetval;

  return(Data_Wrap_Struct(class, rbxs_domnodeset_mark, rbxs_domnodeset_free, prbxs_domnodeset));
}

//***********************************************************************************
// Initialize class Node
//***********************************************************************************
#ifdef RDOC__
  mXML  = rb_define_module( "XML" );
  cSmart = rb_define_class_under( mXML, "Smart", rb_cObject );
  cSmartDom = rb_define_class_under( cSmart, "Dom", rb_cObject );
#endif
VALUE cSmartDomNodeSet;

void init_rbxs_domnodeset(void) {
  cSmartDomNodeSet = rb_define_class_under( cSmartDom, "NodeSet", rb_cObject );
  rb_include_module(cSmartDomNodeSet, rb_mEnumerable);

  rb_define_method(cSmartDomNodeSet, "length",      rbxs_domnodeset_length,     0);
  rb_define_method(cSmartDomNodeSet, "nitems",      rbxs_domnodeset_nitems,     0);
  rb_define_method(cSmartDomNodeSet, "empty?",      rbxs_domnodeset_empty_q,    0);
  rb_define_method(cSmartDomNodeSet, "[]",          rbxs_domnodeset_get,        1);
  rb_define_method(cSmartDomNodeSet, "at",          rbxs_domnodeset_get,        1);
  rb_define_method(cSmartDomNodeSet, "first",       rbxs_domnodeset_first,      0);
  rb_define_method(cSmartDomNodeSet, "last",        rbxs_domnodeset_last,       0);
  rb_define_method(cSmartDomNodeSet, "delete_all!", rbxs_domnodeset_delete_all, 0);
  rb_define_method(cSmartDomNodeSet, "delete_if!",  rbxs_domnodeset_delete_if,  0);
  rb_define_method(cSmartDomNodeSet, "delete_at!",  rbxs_domnodeset_delete_at,  1);
  rb_define_method(cSmartDomNodeSet, "each",        rbxs_domnodeset_each,       0);
}
