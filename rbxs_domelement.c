#include "rbxs_qname.h"
#include "rbxs_dom.h"
#include "rbxs_domelement.h"
#include "rbxs_domnodeset.h"
#include "rbxs_domattribute.h"
#include "rbxs_domattributeset.h"
#include "rbxs_domnamespace.h"
#include "rbxs_domnamespaceset.h"
#include "rbxs_domtext.h"

VALUE cSmartDomElement;

/* -- */
// ***********************************************************************************
// GC
// ***********************************************************************************
void rbxs_domelement_free(rbxs_domelement *prbxs_domelement) {
  if (prbxs_domelement != NULL) {
    free(prbxs_domelement);
  }  
}
  
void rbxs_domelement_mark(rbxs_domelement *prbxs_domelement) { 
  if (prbxs_domelement == NULL) return;
  if (!NIL_P(prbxs_domelement->doc)) rb_gc_mark(prbxs_domelement->doc);
}

// ***********************************************************************************
// Methods
// ***********************************************************************************
/* ++ */

/*
 *  Documentation
 */
VALUE rbxs_domelement_replace_by(VALUE self, VALUE element)
{
  rbxs_domelement *prbxs_domelementA, *prbxs_domelementB;
  xmlNodePtr node;

  if (rb_obj_is_kind_of(element, cSmartDomElement)) {
    Data_Get_Struct(self, rbxs_domelement, prbxs_domelementA);
    Data_Get_Struct(element, rbxs_domelement, prbxs_domelementB);
    node = xmlCopyNode(prbxs_domelementB->node,1); // 1 == deep
    node = xmlReplaceNode(prbxs_domelementA->node,node);
    xmlUnlinkNode(prbxs_domelementA->node);
    xmlFreeNode(prbxs_domelementA->node);
    prbxs_domelementA->node = node;
    if (node) 
      return(rbxs_domelement_new(cSmartDomElement, self, node));
  } else
    rb_raise(rb_eArgError, "takes an element as argument");
  return(Qnil);
}

/*
 *  Documentation
 */
VALUE rbxs_domelement_dump(VALUE self)
{
  rbxs_domelement *prbxs_domelement;
  xmlBufferPtr result;
  VALUE ret;

  Data_Get_Struct(self, rbxs_domelement, prbxs_domelement);
  result = xmlBufferCreate();
  xmlNodeDump(result, prbxs_domelement->node->doc, prbxs_domelement->node, 0, 1);
  ret = rb_str_new2((char *)result->content);
  
  xmlBufferFree(result);
  return(ret);
}

static int nslist_each(VALUE key, VALUE value, xmlXPathContextPtr arg) {
  if (xmlXPathRegisterNs(arg,(unsigned char *)StringValuePtr(key),(unsigned char *)StringValuePtr(value)) != 0) {
    xmlXPathFreeContext(arg);
    rb_raise(rb_eRuntimeError, "Couldn't add namespace");
  }
  return ST_CONTINUE;
}

/*
 *  Documentation
 */
VALUE rbxs_domelement_find(int argc, VALUE *argv, VALUE self)
{
  rbxs_domelement    *prbxs_domelement;
  rbxs_dom           *prbxs_dom;
  xmlXPathContextPtr ctxt;
  xmlXPathObjectPtr  obj;
  VALUE              set;
  
  if (argc > 2 || argc < 1)
    rb_raise(rb_eArgError, "wrong number of arguments (need 1 or 2)");
  Check_Type(argv[0], T_STRING);
    
  Data_Get_Struct(self, rbxs_domelement, prbxs_domelement);
  Data_Get_Struct(prbxs_domelement->doc, rbxs_dom, prbxs_dom);

  ctxt = xmlXPathNewContext(prbxs_domelement->node->doc);
  if(ctxt == NULL)
    rb_raise(rb_eRuntimeError, "Couldn't create XPath context");
  ctxt->node = prbxs_domelement->node;

  if (argc == 2) {
    Check_Type(argv[1], T_HASH);
    if (!RHASH_EMPTY_P(argv[1]))
      rb_hash_foreach(argv[1], nslist_each, (st_data_t)ctxt);
  }
  if (!RHASH_EMPTY_P(prbxs_dom->namespaces))
    rb_hash_foreach(prbxs_dom->namespaces, nslist_each, (st_data_t)ctxt);
  
  obj = xmlXPathEvalExpression((unsigned char *)StringValuePtr(argv[0]), ctxt);
  if(obj == NULL) {
    xmlXPathFreeContext(ctxt);
    rb_raise(rb_eRuntimeError, "Error in xpath");
  }
  switch (obj->type) {
    case XPATH_NODESET:
      set = rbxs_domnodeset_new(cSmartDomNodeSet,prbxs_domelement->doc,obj);
      break;
    case XPATH_BOOLEAN:
      set = obj->boolval > 0 ? Qtrue : Qfalse; 
      xmlXPathFreeObject(obj);
      break;
    case XPATH_NUMBER:
      set = rb_float_new(obj->floatval);
      xmlXPathFreeObject(obj);
      break;
    case XPATH_STRING:
      set = rb_str_new2((char *)obj->stringval);
      xmlXPathFreeObject(obj);
      break;
    default:
      set = Qnil;
  }  
  xmlXPathFreeContext(ctxt);

  return set;
}  

/* 
 * Returns a XML::Smart::Dom::NodeSet containing all the
 * children of this node.
 */
/*
 *  Documentation
 */
VALUE rbxs_domelement_children(VALUE self)
{
  VALUE *vargv, obj;
  vargv = ALLOC_N(VALUE, 1);
  vargv[0] = rb_str_new2("*|text()");
  obj = rbxs_domelement_find(1,vargv,self);
  free(vargv);
  return(obj);
}

/*
 * Does this node have children? Return True or False.
 */
/*
 *  Documentation
 */
VALUE rbxs_domelement_children_q(VALUE self)
{
  rbxs_domelement *prbxs_domelement;
  
  Data_Get_Struct(self, rbxs_domelement, prbxs_domelement);
  if (prbxs_domelement->node->children)
    return(Qtrue);
  else
    return(Qfalse);
}
/*
 *  Documentation
 */
VALUE rbxs_domelement_children_empty_q(VALUE self)
{
  return(rbxs_domelement_children_q(self) == Qtrue ? Qfalse : Qtrue);
}
/*
 *  Documentation
 */
VALUE rbxs_domelement_children_mixed_q(VALUE self)
{
  rbxs_domelement *prbxs_domelement;
  xmlNodePtr node;
  
  Data_Get_Struct(self, rbxs_domelement, prbxs_domelement);
  node = prbxs_domelement->node->children;
  while (node != NULL) {
    if (node->type == XML_TEXT_NODE) return(Qtrue);
    node = node->next;  
  }  
  return(Qfalse);
}

/*
 *  Documentation
 */
VALUE rbxs_domelement_name(VALUE self)
{
  return(rbxs_qname_new(cSmartQName,self,RBXS_PARSER_TYPE_DOM));
}

/*
 *  Documentation
 */
VALUE rbxs_domelement_parent(VALUE self)
{
  rbxs_domelement *prbxs_domelement;
  Data_Get_Struct(self, rbxs_domelement, prbxs_domelement);

  if (prbxs_domelement->node != xmlDocGetRootElement(prbxs_domelement->node->doc))
    return(rbxs_domelement_new(cSmartDomElement, prbxs_domelement->doc, prbxs_domelement->node->parent));
  else
    return(Qnil);
}

/*
 *  Documentation
 */
VALUE rbxs_domelement_path(VALUE self)
{
  rbxs_domelement *prbxs_domelement;
  xmlChar *ret;
  VALUE val;

  Data_Get_Struct(self, rbxs_domelement, prbxs_domelement);
  ret = xmlGetNodePath(prbxs_domelement->node);
  val = rb_str_new2((char *)ret);
  xmlFree(ret);
  return(val);
}

/*
 *  Documentation
 */
VALUE rbxs_domelement_text_set(VALUE self, VALUE text)
{
  rbxs_domelement *prbxs_domelement;
  rbxs_dom *prbxs_dom;
  unsigned char *tc;
  VALUE str;

  Data_Get_Struct(self, rbxs_domelement, prbxs_domelement);
  Data_Get_Struct(prbxs_domelement->doc, rbxs_dom, prbxs_dom);
  
  str = rb_obj_as_string(text);
  if (NIL_P(str) || TYPE(str) != T_STRING)
    rb_raise(rb_eTypeError, "cannot convert obj to string");
  tc = (unsigned char *)StringValuePtr(str);
  if (!xmlCheckUTF8(tc))
    rb_raise(rb_eArgError, "text must be utf8 encoded");

  if (prbxs_dom->encodeEntities == 0)
    xmlNodeSetContent(prbxs_domelement->node, tc);
  else {
    xmlChar  *c;
    c = xmlEncodeEntitiesReentrant(prbxs_domelement->node->doc,tc);
    xmlNodeSetContent(prbxs_domelement->node, c);
    xmlFree(c);
  }  

  rbxs_dom_change_handlers_execute(prbxs_dom,RBXS_DOM_SIGNAL_CHANGE,rbxs_domtext_new(cSmartDomText, prbxs_domelement->doc, prbxs_domelement->node->children));
  return(text);
} 

/*
 *  Documentation
 */
VALUE rbxs_domelement_text_get(VALUE self)
{
  rbxs_domelement *prbxs_domelement;
  
  Data_Get_Struct(self, rbxs_domelement, prbxs_domelement);
  if (prbxs_domelement->node->children != NULL)
    if (prbxs_domelement->node->children->content != NULL)
      return(rb_str_new2((char *)prbxs_domelement->node->children->content));
  return(rb_str_new2(""));
}

/*
 *  Documentation
 */
VALUE rbxs_domelement_inspect(VALUE self)
{
  VALUE *argv;

  rbxs_domelement *prbxs_domelement;
  Data_Get_Struct(self, rbxs_domelement, prbxs_domelement);

  argv = ALLOCA_N(VALUE, 4);
  argv[0] = rb_str_new2("#<%s:0x%x \"%s\">");
  argv[1] = CLASS_OF(self);
  argv[2] = rb_obj_id(self);
  argv[3] = rb_str_new2((char *)prbxs_domelement->node->name);
  return(rb_f_sprintf(4, argv));
}

/*
 *  Documentation
 */
VALUE rbxs_domelement_to_i(int argc, VALUE *argv, VALUE self)
{
    VALUE b;
    int base;

    rb_scan_args(argc, argv, "01", &b);
    if (argc == 0) base = 10;
    else base = NUM2INT(b);

    if (base < 0)
	    rb_raise(rb_eArgError, "illegal radix %d", base);
    return(rb_str_to_inum(rbxs_domelement_text_get(self),base,Qfalse));
}

/*
 *  Documentation
 */
VALUE rbxs_domelement_to_f(VALUE self)
{
  return rb_float_new(rb_str_to_dbl(rbxs_domelement_text_get(self), Qfalse));
}

/*
 *  Documentation
 */
VALUE rbxs_domelement_attributes(VALUE self)
{
  return rbxs_domattributeset_new(cSmartDomAttributeSet,self);
}
  
static unsigned short checkArguments(int argc, VALUE *argv, VALUE *content, VALUE *attributes) {
  if (argc < 1) rb_raise(rb_eArgError, "at least 1 argument is mandatory");
  if (TYPE(argv[0]) == T_STRING) {
    switch (argc) {
      case 3:
        if ((TYPE(argv[2]) == T_HASH) && (TYPE(argv[1]) != T_HASH)) {
          *content = rb_obj_as_string(argv[1]);
          *attributes = argv[2];
        } else if ((TYPE(argv[1]) == T_HASH) && (TYPE(argv[2]) != T_HASH)) {
          *content = rb_obj_as_string(argv[2]);
          *attributes = argv[1];
        } else  
          rb_raise(rb_eArgError, "argument 2 or 3 has to be a Hash");
        break; 
      case 2:
        if (TYPE(argv[1]) != T_HASH) 
          *content = rb_obj_as_string(argv[1]);
        else if (TYPE(argv[1]) == T_HASH) 
          *attributes = argv[1];
        else  
          rb_raise(rb_eArgError, "argument 2 has to be a String or a Hash");
        break;
      case 1:
        break;
      default:  
        rb_raise(rb_eArgError, "wrong number of arguments (needs 1 to 3)");
    }
    return(1);
  } else if (rb_obj_is_kind_of(argv[0], cSmartDomElement) || rb_obj_is_kind_of(argv[0], cSmartDomNodeSet)) {
    switch (argc) {
      case 2:
        if ((TYPE(argv[1]) != T_FIXNUM) || (NUM2INT(argv[1]) > 1) || (NUM2INT(argv[1]) < 0))
          rb_raise(rb_eArgError, "argument 2 has to be a either ::MOVE or ::COPY");
        break;
      case 1:
        break;
      default:  
        rb_raise(rb_eArgError, "wrong number of arguments (needs 2)");
    }
    if (rb_obj_is_kind_of(argv[0], cSmartDomElement)) return(2);
    if (rb_obj_is_kind_of(argv[0], cSmartDomNodeSet)) return(3);
  } else {
      rb_raise(rb_eArgError, "wrong type of first argument");
  }
  return 0;
}
  
static int addAttributes_each(VALUE key, VALUE value, xmlNodePtr arg) {
  xmlAttrPtr attr;
  unsigned char *tc;
  VALUE str;
  
  str = rb_obj_as_string(value);
  if (NIL_P(str) || TYPE(str) != T_STRING)
    rb_raise(rb_eTypeError, "cannot convert obj to string");
  tc = (unsigned char *)StringValuePtr(str);
  if (!xmlCheckUTF8(tc))
    rb_raise(rb_eArgError, "text must be utf8 encoded");

  str = rb_obj_as_string(key);
  if (NIL_P(str) || TYPE(str) != T_STRING)
    rb_raise(rb_eTypeError, "cannot convert obj to string");
  attr = xmlNewProp(arg, (unsigned char *)StringValuePtr(str), tc);

  if (attr == NULL)
    rb_raise(rb_eRuntimeError, "Couldn't not add attribute");
  return ST_CONTINUE;
}

static VALUE moveOrCopyElement(xmlNodePtr domelement, VALUE argA, VALUE argB, rbxs_domelement *prbxs_domelement, int where, int ret) {
  xmlNodePtr new_domelement = NULL;
  if ((argB == Qnil &&  domelement->doc == prbxs_domelement->node->doc) || (argB != Qnil && NUM2INT(argB) == 0)) {
    if (domelement->doc == domelement->parent->doc && domelement->parent != NULL)
      xmlUnlinkNode(domelement);
    switch (where) {
      case 1: new_domelement = xmlAddChild(prbxs_domelement->node,domelement); break;
      case 2: new_domelement = xmlAddPrevSibling(prbxs_domelement->node,domelement); break;
      case 3: new_domelement = xmlAddNextSibling(prbxs_domelement->node,domelement); break;
    }          
    if (ret != 0)
      return rbxs_domelement_new(cSmartDomElement,prbxs_domelement->doc,new_domelement);
  } else if ((argB == Qnil &&  domelement->doc != prbxs_domelement->node->doc) || (argB != Qnil && NUM2INT(argB) == 1)) {
    new_domelement = xmlCopyNode(domelement,1); // 1 == deep
    switch (where) {
      case 1: new_domelement = xmlAddChild(prbxs_domelement->node,new_domelement); break;
      case 2: new_domelement = xmlAddPrevSibling(prbxs_domelement->node,new_domelement); break;
      case 3: new_domelement = xmlAddNextSibling(prbxs_domelement->node,new_domelement); break;
    }          
    if (ret != 0) return(rbxs_domelement_new(cSmartDomElement,prbxs_domelement->doc,new_domelement));
  }
  return(Qnil);
}  

static VALUE rbxs_domelement_add_worker(int argc, VALUE *argv, VALUE self, int where) {
  rbxs_domelement *prbxs_domelement, *prbxs_cdomelement;
  rbxs_domnodeset *prbxs_cdomnodeset;
  rbxs_dom *prbxs_dom;
  xmlNodePtr new_domelement = NULL;
  VALUE content;
  VALUE attributes;
  VALUE ret,str;
  int i;
  xmlChar *c = NULL;

  content    = Qnil;
  attributes = rb_hash_new();
    
  Data_Get_Struct(self, rbxs_domelement, prbxs_domelement);

  switch (checkArguments(argc,argv,&content,&attributes)) {
    case 1:
      Data_Get_Struct(prbxs_domelement->doc, rbxs_dom, prbxs_dom);
      if (content != Qnil) {
        char *tc = StringValuePtr(content);
        if (!xmlCheckUTF8((unsigned char *)tc))
          rb_raise(rb_eArgError, "text must be utf8 encoded");
        if (prbxs_dom->encodeEntities == 0)
          c = xmlCharStrdup(tc);
        else
          c = xmlEncodeEntitiesReentrant(prbxs_domelement->node->doc,(unsigned char *)tc);
      }  
      switch (where) {
        case 1:
          str = rb_obj_as_string(argv[0]);
          if (NIL_P(str) || TYPE(str) != T_STRING)
            rb_raise(rb_eTypeError, "cannot convert obj to string");
          new_domelement = xmlNewChild(prbxs_domelement->node, NULL, (unsigned char *)StringValuePtr(str), c);
          break;
        case 2:
          str = rb_obj_as_string(argv[0]);
          if (NIL_P(str) || TYPE(str) != T_STRING)
            rb_raise(rb_eTypeError, "cannot convert obj to string");
          new_domelement = xmlNewDocNode(prbxs_domelement->node->doc, NULL, (unsigned char *)StringValuePtr(argv[0]), c);
          new_domelement = xmlAddPrevSibling(prbxs_domelement->node,new_domelement);
          break;
        case 3:
          str = rb_obj_as_string(argv[0]);
          if (NIL_P(str) || TYPE(str) != T_STRING)
            rb_raise(rb_eTypeError, "cannot convert obj to string");
          new_domelement = xmlNewDocNode(prbxs_domelement->node->doc, NULL, (unsigned char *)StringValuePtr(argv[0]), c);
          new_domelement = xmlAddNextSibling(prbxs_domelement->node,new_domelement);
          break;
      }    
      xmlFree(c);
      if (new_domelement == NULL)
        rb_raise(rb_eRuntimeError, "Couldn't add element");
      if (!RHASH_EMPTY_P(attributes))
        rb_hash_foreach(attributes, addAttributes_each, (st_data_t)new_domelement);
      ret = rbxs_domelement_new(cSmartDomElement,prbxs_domelement->doc,new_domelement);
      rbxs_dom_change_handlers_execute(prbxs_dom,RBXS_DOM_SIGNAL_ADD,ret);
      return(ret);
      break;
    case 2:
      Data_Get_Struct(argv[0], rbxs_domelement, prbxs_cdomelement);
      ret = moveOrCopyElement(prbxs_cdomelement->node,argv[0],argc == 1 ? Qnil : argv[1],prbxs_domelement,where,1);
      if (ret != Qnil) {
        Data_Get_Struct(prbxs_domelement->doc, rbxs_dom, prbxs_dom);
        rbxs_dom_change_handlers_execute(prbxs_dom,RBXS_DOM_SIGNAL_ADD,ret);
      }  
      return(ret);
      break;
    case 3:  
      Data_Get_Struct(argv[0], rbxs_domnodeset, prbxs_cdomnodeset);
      if (xmlXPathNodeSetIsEmpty(prbxs_cdomnodeset->nodeset)) return(Qnil);
      for (i = 0; i < prbxs_cdomnodeset->nodeset->nodeNr; i++) {
        if (prbxs_cdomnodeset->nodeset->nodeTab[i] == NULL) return (Qnil);
        if (prbxs_cdomnodeset->nodeset->nodeTab[i]->type == XML_ELEMENT_NODE) {
          Data_Get_Struct(prbxs_domelement->doc, rbxs_dom, prbxs_dom);
          ret = moveOrCopyElement(prbxs_cdomnodeset->nodeset->nodeTab[i],argv[0],argc == 1 ? Qnil : argv[1],prbxs_domelement,where,0);
          rbxs_dom_change_handlers_execute(prbxs_dom,RBXS_DOM_SIGNAL_ADD,ret);
        }    
      }
      return(argv[0]);
  }  
  return(Qnil); 
}

/*
 *  Documentation
 */
VALUE rbxs_domelement_add(int argc, VALUE *argv, VALUE self)
{
  return(rbxs_domelement_add_worker(argc,argv,self,1));
}  
/*
 *  Documentation
 */
VALUE rbxs_domelement_add_before(int argc, VALUE *argv, VALUE self)
{
  return(rbxs_domelement_add_worker(argc,argv,self,2));
}    
/*
 *  Documentation
 */
VALUE rbxs_domelement_add_after(int argc, VALUE *argv, VALUE self)
{
  return(rbxs_domelement_add_worker(argc,argv,self,3));
}

/*
 *  Documentation
 */
VALUE rbxs_domelement_namespaces(VALUE self)
{
  return rbxs_domnamespaceset_new(cSmartDomNamespaceSet,self);
}

/*
 *  Documentation
 */
VALUE rbxs_domelement_namespace(VALUE self)
{
	rbxs_domelement *prbxs_domelement;
	
	Data_Get_Struct(self, rbxs_domelement, prbxs_domelement);
	if (prbxs_domelement->node->ns) {
		return(rbxs_domnamespace_new(cSmartDomNamespace,self,prbxs_domelement->node->ns));
	}
	return(Qnil);
}

/*
 *  Documentation
 */
VALUE rbxs_domelement_namespace_q(VALUE self)
{
	rbxs_domelement *prbxs_domelement;
	
	Data_Get_Struct(self, rbxs_domelement, prbxs_domelement);
	if (prbxs_domelement->node->ns)
		return(Qtrue);
	return(Qfalse);
}

/*
 *  Documentation
 */
VALUE rbxs_domelement_xinclude(VALUE self)
{
  xmlNodePtr node = NULL;
  int err = 0;
	rbxs_domelement *prbxs_domelement;
	
	Data_Get_Struct(self, rbxs_domelement, prbxs_domelement);

  node = (xmlNodePtr) prbxs_domelement->node;
  if (node) {
    err = xmlXIncludeProcessTree(node);
    rbxs_dom_remove_xinclude_nodes(node);
  }

	if (err)
		return(Qtrue);
	return(Qfalse);
}

/*
 *  Documentation
 */
VALUE rbxs_domelement_namespace_set(VALUE self,VALUE nsobj)
{
	rbxs_domelement *prbxs_domelement;
  rbxs_dom *prbxs_dom;
	
  Data_Get_Struct(self, rbxs_domelement, prbxs_domelement);
  Data_Get_Struct(prbxs_domelement->doc, rbxs_dom, prbxs_dom);
	
	if (rb_obj_is_kind_of(nsobj,cSmartDomNamespace)) {
	  rbxs_domnamespace *prbxs_domnamespace;
    Data_Get_Struct(nsobj, rbxs_domnamespace, prbxs_domnamespace);
    xmlSetNs(prbxs_domelement->node,prbxs_domnamespace->ns);
    rbxs_dom_change_handlers_execute(prbxs_dom,RBXS_DOM_SIGNAL_CHANGE,nsobj);
    return(Qnil);
  }  
  if (TYPE(nsobj) == T_STRING) {
    xmlNsPtr ns;
    ns = xmlSearchNs(prbxs_domelement->node->doc, prbxs_domelement->node, (unsigned char *)StringValuePtr(nsobj));
    if (!ns) {
      ns = xmlSearchNsByHref(prbxs_domelement->node->doc, prbxs_domelement->node, (unsigned char *)StringValuePtr(nsobj));
      if (!ns) rb_raise(rb_eArgError, "String identifies no valid namespace");
    }  
    xmlSetNs(prbxs_domelement->node,ns);
    rbxs_dom_change_handlers_execute(prbxs_dom,RBXS_DOM_SIGNAL_CHANGE,rbxs_domnamespace_new(cSmartDomNamespace, prbxs_domelement->doc, ns));
    return(Qnil);
  }  
  rb_raise(rb_eArgError, "Expecting XML::Smart::Dom::Namespace");
}

// ***********************************************************************************
// Constructors
// ***********************************************************************************
VALUE rbxs_domelement_new(VALUE class, VALUE doc, xmlNodePtr node) {
  rbxs_domelement *prbxs_domelement;

  prbxs_domelement = (rbxs_domelement *)malloc(sizeof(rbxs_domelement));
  if (prbxs_domelement == NULL )
    rb_raise(rb_eNoMemError, "No memory left for XML::Smart::Dom::Element struct");

  prbxs_domelement->doc  = doc;
  prbxs_domelement->node = node;

  return(Data_Wrap_Struct(class, rbxs_domelement_mark, rbxs_domelement_free, prbxs_domelement));
}

// ***********************************************************************************
// Initialize class Node
// ***********************************************************************************
#ifdef RDOC__
  mXML  = rb_define_module( "XML" );
  cSmart = rb_define_class_under( mXML, "Smart", rb_cObject );
  cSmartDom = rb_define_class_under( cSmart, "Dom", rb_cObject );
#endif

void init_rbxs_domelement(void) {
  cSmartDomElement = rb_define_class_under( cSmartDom, "Element", rb_cObject );
  
  rb_define_const(cSmartDomElement, "MOVE", INT2NUM(0));
  rb_define_const(cSmartDomElement, "COPY", INT2NUM(1));

  rb_define_method(cSmartDomElement, "dump",            rbxs_domelement_dump,              0);
  rb_define_method(cSmartDomElement, "inspect",         rbxs_domelement_inspect,           0);
  rb_define_method(cSmartDomElement, "find",            rbxs_domelement_find,             -1);
  rb_define_method(cSmartDomElement, "to_s",            rbxs_domelement_text_get,          0);
  rb_define_method(cSmartDomElement, "to_i",            rbxs_domelement_to_i,             -1);
  rb_define_method(cSmartDomElement, "to_f",            rbxs_domelement_to_f,              0);
  rb_define_method(cSmartDomElement, "name",            rbxs_domelement_name,              0);
  rb_define_method(cSmartDomElement, "text",            rbxs_domelement_text_get,          0);
  rb_define_method(cSmartDomElement, "text=",           rbxs_domelement_text_set,          1);
  rb_define_method(cSmartDomElement, "children",        rbxs_domelement_children,          0);
  rb_define_method(cSmartDomElement, "children?",       rbxs_domelement_children_q,        0);
  rb_define_method(cSmartDomElement, "empty?",          rbxs_domelement_children_empty_q,  0);
  rb_define_method(cSmartDomElement, "mixed?",          rbxs_domelement_children_mixed_q,  0);
  rb_define_method(cSmartDomElement, "parent",          rbxs_domelement_parent,            0);
  rb_define_method(cSmartDomElement, "path",            rbxs_domelement_path,              0);
  rb_define_method(cSmartDomElement, "attributes",      rbxs_domelement_attributes,        0);
  rb_define_method(cSmartDomElement, "add",             rbxs_domelement_add,              -1);
  rb_define_method(cSmartDomElement, "add_before",      rbxs_domelement_add_before,       -1);
  rb_define_method(cSmartDomElement, "add_after",       rbxs_domelement_add_after,        -1);
  rb_define_method(cSmartDomElement, "replace_by",      rbxs_domelement_replace_by,        1);
  rb_define_method(cSmartDomElement, "namespaces",      rbxs_domelement_namespaces,        0);
  rb_define_method(cSmartDomElement, "namespace",       rbxs_domelement_namespace,         0);
  rb_define_method(cSmartDomElement, "namespace?",      rbxs_domelement_namespace_q,       0);
  rb_define_method(cSmartDomElement, "namespace=",      rbxs_domelement_namespace_set,     1);
  rb_define_method(cSmartDomElement, "xinclude!",       rbxs_domelement_xinclude,          0);
}
