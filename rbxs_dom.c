#include "rbxs_dom.h"
#include "rbxs_domelement.h"
#include "rbxs_domnodeset.h"
#include "XSDtoRNG.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

const char XSDtoRNG[] = {
#include "XSDtoRNG.xsl"
};

/* -- */
// ***********************************************************************************
// GC
// ***********************************************************************************
void rbxs_dom_free(rbxs_dom *prbxs_dom) {
  if (prbxs_dom != NULL) {
    xmlFreeDoc(prbxs_dom->doc);
    xmlCleanupParser();
    free(prbxs_dom);
  }
}

void rbxs_dom_mark(rbxs_dom *prbxs_dom) {
  if (prbxs_dom == NULL) return;
  if (!NIL_P(prbxs_dom->changeHandlers)) rb_gc_mark(prbxs_dom->changeHandlers);
  if (!NIL_P(prbxs_dom->namespaces)) rb_gc_mark(prbxs_dom->namespaces);
}

//***********************************************************************************
// Methods
//***********************************************************************************
/* ++ */
static int nslist_each(VALUE key, VALUE value, xmlXPathContextPtr arg) {
  if (xmlXPathRegisterNs(arg,(unsigned char *)StringValuePtr(key),(unsigned char *)StringValuePtr(value)) != 0) {
    xmlXPathFreeContext(arg);
    rb_raise(rb_eRuntimeError, "Couldn't add namespace");
  }
  return ST_CONTINUE;
}

static int nslistset_each(VALUE key, VALUE value) {
  Check_Type(key, T_STRING);
  Check_Type(value, T_STRING);
  return ST_CONTINUE;
}

/*
 *  Documentation
 */
VALUE rbxs_dom_inspect(VALUE self) 
{
  VALUE *argv;

  argv = ALLOCA_N(VALUE, 3);
  argv[0] = rb_str_new2("#<%s:0x%x>");
  argv[1] = CLASS_OF(self);
  argv[2] = rb_obj_id(self);
  return(rb_f_sprintf(3, argv));
}

/*
 *  Documentation
 */
VALUE rbxs_dom_to_s(VALUE self)
{
  rbxs_dom *prbxs_dom;
  xmlChar *result;
  VALUE ret;
  int len;

  Data_Get_Struct(self, rbxs_dom, prbxs_dom);
  if (prbxs_dom->doc == NULL) {
    return(Qnil);
  } else if (prbxs_dom->doc->encoding != NULL) {
    xmlDocDumpFormatMemoryEnc(prbxs_dom->doc, &result, &len, (char *)prbxs_dom->doc->encoding, 1);
  } else {
    xmlDocDumpFormatMemory(prbxs_dom->doc, &result, &len, 1);
  }

  ret = rb_str_new2((char *)result);
  xmlFree(result);
  return(ret);
}

/*
 *  Documentation
 */
VALUE rbxs_dom_dup(VALUE self)
{
  return rbxs_dom_string(rbxs_dom_to_s(self));
}

/*
 *  Documentation
 */
VALUE rbxs_dom_root_set(VALUE self, VALUE element)
{
  rbxs_dom *prbxs_dom;
  rbxs_domelement *prbxs_domelement;
  xmlNodePtr root,newnode;

  if (rb_obj_is_kind_of(element, cSmartDomElement)) {
    Data_Get_Struct(self, rbxs_dom, prbxs_dom);
    Data_Get_Struct(element, rbxs_domelement, prbxs_domelement);
    newnode = xmlCopyNode(prbxs_domelement->node,1); // 1 == deep
    root = xmlDocGetRootElement(prbxs_dom->doc);
    xmlUnlinkNode(root);
    xmlFreeNode  (root);
    root = xmlDocSetRootElement(prbxs_dom->doc,newnode);
    if (root) {
      VALUE ret = rbxs_domelement_new(cSmartDomElement, self, root);  
      rbxs_dom_change_handlers_execute(prbxs_dom,RBXS_DOM_SIGNAL_CHANGE,ret);
      return(ret);
    }  
  } else
    rb_raise(rb_eArgError, "takes an element as argument");
  return(Qnil);
}

/*
 *  Documentation
 */
VALUE rbxs_dom_encode_entities_set(VALUE self, VALUE value)
{
  rbxs_dom *prbxs_dom;
  Data_Get_Struct(self, rbxs_dom, prbxs_dom);
  switch (TYPE(value)) {
    case T_FALSE:
      prbxs_dom->encodeEntities = 0;
      return Qfalse; 
    case T_TRUE:
      prbxs_dom->encodeEntities = 1;
      return Qtrue; 
    default:
     rb_raise(rb_eArgError, "can be set to TRUE or FALSE");
  }  
}

/*
 *  Documentation
 */
VALUE rbxs_dom_xinclude(VALUE self)
{
  xmlNodePtr root = NULL;
  int err = 0;
  rbxs_dom *prbxs_dom;
	
  Data_Get_Struct(self, rbxs_dom, prbxs_dom);
  err = xmlXIncludeProcess(prbxs_dom->doc);

  root = (xmlNodePtr) prbxs_dom->doc->children;
  while(root && root->type != XML_ELEMENT_NODE && root->type != XML_XINCLUDE_START) {
    root = root->next;
  }
  if (root) {
    rbxs_dom_remove_xinclude_nodes(root);
  }

	if (err)
		return(Qtrue);
	return(Qfalse);
}

/*
 *  Documentation
 */
VALUE rbxs_dom_encode_entities_q(VALUE self)
{
  rbxs_dom *prbxs_dom;
  Data_Get_Struct(self, rbxs_dom, prbxs_dom);
  if (prbxs_dom->encodeEntities == 0) 
    return(Qfalse);  
  else
    return(Qtrue);
}

/*
 *  Documentation
 */
VALUE rbxs_dom_save_unformated_set(VALUE self, VALUE value)
{
  rbxs_dom *prbxs_dom;
  Data_Get_Struct(self, rbxs_dom, prbxs_dom);
  switch (TYPE(value)) {
    case T_FALSE:
      prbxs_dom->saveUnformated = 1;
      return Qfalse; 
    case T_TRUE:
      prbxs_dom->saveUnformated = 0;
      return Qtrue; 
    default:
     rb_raise(rb_eArgError, "can be set to TRUE or FALSE");
  }  
}

/*
 *  Documentation
 */
VALUE rbxs_dom_save_unformated_q(VALUE self)
{
  rbxs_dom *prbxs_dom;
  Data_Get_Struct(self, rbxs_dom, prbxs_dom);
  if (prbxs_dom->saveUnformated == 1) 
    return(Qfalse);  
  else
    return(Qtrue);
}
  
/*
 *  Documentation
 */
VALUE rbxs_dom_root(VALUE self)
{
  rbxs_dom *prbxs_dom;
  xmlNodePtr root;

  Data_Get_Struct(self, rbxs_dom, prbxs_dom);
  root = xmlDocGetRootElement(prbxs_dom->doc);
  if (root) 
    return(rbxs_domelement_new(cSmartDomElement, self, root));  
  else
    return(Qnil);
}

/*
 *  Documentation
 */
VALUE rbxs_dom_validate_relaxng(VALUE self, VALUE rdoc)
{
	xmlRelaxNGParserCtxtPtr parser;
	xmlRelaxNGPtr           sptr;
	xmlRelaxNGValidCtxtPtr  vptr;
	int                     is_valid;
  
  rbxs_dom *prbxs_dom;
  rbxs_dom *prbxs_rdom;

  Data_Get_Struct(self, rbxs_dom, prbxs_dom);
  Data_Get_Struct(rdoc, rbxs_dom, prbxs_rdom);

  if (rb_obj_is_kind_of(rdoc, cSmartDom)) {
    parser = xmlRelaxNGNewDocParserCtxt(prbxs_rdom->doc);

    xmlRelaxNGSetParserErrors(parser,
      (xmlRelaxNGValidityErrorFunc) fprintf,
      (xmlRelaxNGValidityWarningFunc) fprintf,
      stderr);
    sptr = xmlRelaxNGParse(parser);
    xmlRelaxNGFreeParserCtxt(parser);
    if (!sptr)
      rb_raise(rb_eRuntimeError, "Invalid RelaxNG");

    vptr = xmlRelaxNGNewValidCtxt(sptr);
    if (!vptr) {
      xmlRelaxNGFree(sptr);
      rb_raise(rb_eRuntimeError, "Invalid RelaxNG Validation Context");
    }

    xmlRelaxNGSetValidErrors(vptr, 
      (xmlRelaxNGValidityErrorFunc) fprintf, 
      (xmlRelaxNGValidityErrorFunc) fprintf, 
      stderr);
    is_valid = xmlRelaxNGValidateDoc(vptr, prbxs_dom->doc);
    xmlRelaxNGFree(sptr);
    xmlRelaxNGFreeValidCtxt(vptr);

    if (is_valid == 0)
      return(Qtrue);
    return(Qfalse);
  } else
    rb_raise(rb_eArgError, "takes a dom document as argument");
}

/*
 *  Documentation
 */
VALUE rbxs_dom_save_as(VALUE self, VALUE file)
{
  Check_Type(file, T_FILE);
  rb_io_write(file,rbxs_dom_to_s(self));
  return(Qtrue);
}

/*
 *  Documentation
 */
VALUE rbxs_dom_find(int argc, VALUE *argv, VALUE self)
{
  rbxs_dom           *prbxs_dom;
  xmlXPathContextPtr  ctxt;
  xmlXPathObjectPtr   obj;
  xmlNodePtr          root;
  VALUE               set;
  
  if (argc > 2 || argc < 1)
    rb_raise(rb_eArgError, "wrong number of arguments (need 1 or 2)");
  Check_Type(argv[0], T_STRING);

  Data_Get_Struct(self, rbxs_dom, prbxs_dom);

  ctxt = xmlXPathNewContext(prbxs_dom->doc);
  if(ctxt == NULL)
    rb_raise(rb_eRuntimeError, "Couldn't create XPath context");
  root = xmlDocGetRootElement(prbxs_dom->doc);
  ctxt->node = root->parent;

  if (argc == 2) {
    Check_Type(argv[1], T_HASH);
    if (!RHASH_EMPTY_P(argv[1]))
      rb_hash_foreach(argv[1], (int(*)(ANYARGS))nslist_each, (st_data_t)ctxt);
  }
  if (!RHASH_EMPTY_P(prbxs_dom->namespaces))
    rb_hash_foreach(prbxs_dom->namespaces, (int(*)(ANYARGS))nslist_each, (st_data_t)ctxt);

  obj = xmlXPathEvalExpression((unsigned char *)StringValuePtr(argv[0]), ctxt);
  ctxt->node = NULL;
  if(obj == NULL) {
    xmlXPathFreeContext(ctxt);
    rb_raise(rb_eRuntimeError, "Error in xpath");
  }
  switch (obj->type) {
    case XPATH_NODESET:
      set = rbxs_domnodeset_new(cSmartDomNodeSet,self,obj);
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
 *  Documentation
 */
VALUE rbxs_dom_change_handler_set(VALUE self)
{
  rbxs_dom *prbxs_dom;

  Data_Get_Struct(self, rbxs_dom, prbxs_dom);
  if (!rb_block_given_p())
    rb_raise(rb_eArgError, "you need to supply a block with #on_change");

  rb_ary_push(prbxs_dom->changeHandlers,rb_block_proc());
  return(Qtrue); 
}  


/*
 *  Documentation
 */
VALUE rbxs_dom_change_handlers(VALUE self)
{
  rbxs_dom *prbxs_dom;
  Data_Get_Struct(self, rbxs_dom, prbxs_dom);
  return(prbxs_dom->changeHandlers);
}

/*
 *  Documentation
 */
VALUE rbxs_dom_namespaces_get(VALUE self)
{
  rbxs_dom *prbxs_dom;
  Data_Get_Struct(self, rbxs_dom, prbxs_dom);
  return(prbxs_dom->namespaces);
}
/*
 *  Documentation
 */
VALUE rbxs_dom_namespaces_set(VALUE self, VALUE ns)
{
  rbxs_dom *prbxs_dom;
  Data_Get_Struct(self, rbxs_dom, prbxs_dom);
  Check_Type(ns, T_HASH);
  if (!RHASH_EMPTY_P(ns))
    rb_hash_foreach(ns, (int(*)(ANYARGS))nslistset_each, Qnil);
  prbxs_dom->namespaces = ns;
  return(Qnil);
}
    
void rbxs_dom_change_handlers_execute(rbxs_dom *prbxs_dom, unsigned short int type, VALUE node) {
  long i;
  for (i = 0; i < RARRAY_LEN(prbxs_dom->changeHandlers); i++) {
    VALUE args = rb_ary_new2(2);

    VALUE e = RARRAY_PTR(prbxs_dom->changeHandlers)[i];
    rb_ary_push(args,INT2FIX(type));
    rb_ary_push(args,node);
    rb_eval_cmd(e,args,1);
  }
}

xmlNodePtr rbxs_dom_free_xinclude_node(xmlNodePtr cur) {
  xmlNodePtr xincnode;
  xincnode = cur;
  cur = cur->next;
  xmlUnlinkNode(xincnode);
  xmlFreeNode(xincnode);
  return cur;
}

void rbxs_dom_remove_xinclude_nodes(xmlNodePtr cur) {
  while(cur) {
    if (cur->type == XML_XINCLUDE_START) {
      rbxs_dom_free_xinclude_node(cur);

      while(cur && cur->type != XML_XINCLUDE_END) {
        if (cur->type == XML_ELEMENT_NODE) {
           rbxs_dom_remove_xinclude_nodes(cur->children);
        }
        cur = cur->next;
      }

      if (cur && cur->type == XML_XINCLUDE_END) {
        cur = rbxs_dom_free_xinclude_node(cur);
      }
    } else {
      if (cur->type == XML_ELEMENT_NODE) {
        rbxs_dom_remove_xinclude_nodes(cur->children);
      }
      cur = cur->next;
    }
  }
}

//***********************************************************************************
// Constructors
//***********************************************************************************
VALUE rbxs_dom_string(VALUE string) {
  rbxs_dom *prbxs_dom;
  xmlParserCtxtPtr ctxt;

  Check_Type(string, T_STRING);
  
  prbxs_dom = (rbxs_dom *)malloc(sizeof(rbxs_dom));
  if (prbxs_dom == NULL)
    rb_raise(rb_eNoMemError, "No memory left for XML::Smart::Dom struct");

  xmlInitParser();

  ctxt = xmlCreateMemoryParserCtxt(StringValuePtr(string),strlen(StringValuePtr(string)));
  if (ctxt == NULL)
   rb_sys_fail(StringValuePtr(string));

  if (xmlParseDocument(ctxt) == -1) {
    xmlFreeDoc(ctxt->myDoc);
    xmlFreeParserCtxt(ctxt);
    rb_raise(rb_eRuntimeError, "Document didn't parse");
  }

  if (!ctxt->wellFormed) {
    xmlFreeDoc(ctxt->myDoc);
    xmlFreeParserCtxt(ctxt);
    ctxt = NULL;
    rb_raise(rb_eRuntimeError, "Document is not wellformed");
  }

  prbxs_dom->type = RBXS_DOM_TYPE_STRING;
  prbxs_dom->lock = 0;
  prbxs_dom->changeHandlers = rb_ary_new2(0);
  prbxs_dom->namespaces = rb_hash_new();
  prbxs_dom->encodeEntities = 1;
  prbxs_dom->saveUnformated = 1;
  prbxs_dom->doc  = ctxt->myDoc;
  
  xmlFreeParserCtxt(ctxt);

  return(Data_Wrap_Struct(cSmartDom, rbxs_dom_mark, rbxs_dom_free, prbxs_dom));
}

VALUE rbxs_dom_open(VALUE name, char *root, unsigned short int lock, unsigned short int save) {
  rbxs_dom *prbxs_dom;
  xmlParserCtxtPtr ctxt;
  char *lockfile;

  ctxt = NULL;
  lockfile = NULL;

  if ((!rb_block_given_p()) && save>0)
    rb_raise(rb_eArgError, "you need to supply a block with #modify");
    
  prbxs_dom = (rbxs_dom *)malloc(sizeof(rbxs_dom));
  if (prbxs_dom == NULL)
    rb_raise(rb_eNoMemError, "No memory left for XML::Smart::Dom struct");

  if (root != NULL) {
    int fd;
    fd = open(StringValuePtr(name), O_CREAT|O_EXCL|O_WRONLY, S_IRUSR|S_IWUSR);
    if (fd < 0) {
      if (errno != EEXIST)
        rb_raise(rb_eRuntimeError, "Failed to create new file");
    } else {
      char *wr;
      wr = (char *)calloc(strlen("<?xml version='1.0'?>\n") + strlen(root) + 1,sizeof(char));
      strncat(wr,"<?xml version='1.0'?>\n",strlen("<?xml version='1.0'?>\n"));
      strncat(wr,root,strlen(root));
      if (!(write(fd,wr,strlen(wr)) > 0))
        rb_raise(rb_eRuntimeError, "Failed to write new file");
      free(wr);
    }  
    close(fd);
  }
    
  if (save > 0 && lock > 0) {
    struct stat buffer;
    lockfile = (char *)calloc(strlen(StringValuePtr(name)) + 6,sizeof(char));
    strncat(lockfile,StringValuePtr(name),strlen(StringValuePtr(name)));
    strncat(lockfile,".lock",5);
    int fd;
    do {
      fd = open(lockfile, O_CREAT|O_EXCL, S_IRUSR|S_IWUSR);
      if (fd < 0) {
        if (errno != EEXIST) {
          xmlFreeDoc(ctxt->myDoc);
          ctxt = NULL;
          free(lockfile);
          rb_raise(rb_eRuntimeError, "Failed to create lockfile");
        } else {
          stat(lockfile, &buffer);
          if (buffer.st_mtime < time(NULL) - lock)
            unlink(lockfile); 
        }  
      } 
      rb_thread_polling();
    } while (fd < 0);
    close(fd);
  }

  xmlInitParser();
  ctxt = xmlCreateFileParserCtxt(StringValuePtr(name));

  prbxs_dom->type = RBXS_DOM_TYPE_FILE;
  prbxs_dom->encodeEntities = 1;
  prbxs_dom->saveUnformated = 1;
  prbxs_dom->changeHandlers = rb_ary_new2(0);
  prbxs_dom->namespaces = rb_hash_new();
  prbxs_dom->lock = lock;

  if (ctxt == NULL) {
    if (save>0 && prbxs_dom->lock>0) {
      unlink(lockfile);
      free(lockfile);
    }  
    rb_sys_fail(StringValuePtr(name));
  } 

  if (xmlParseDocument(ctxt) == -1) {
    if (save>0 && prbxs_dom->lock>0) {
      unlink(lockfile);
      free(lockfile);
    }  
    xmlFreeDoc(ctxt->myDoc);
    xmlFreeParserCtxt(ctxt);
    rb_raise(rb_eRuntimeError, "Document didn't parse");
  }

  if (!ctxt->wellFormed) {
    if (save>0 && prbxs_dom->lock>0) {
      unlink(lockfile);
      free(lockfile);
    }  
    xmlFreeDoc(ctxt->myDoc);
    xmlFreeParserCtxt(ctxt);
    ctxt = NULL;
    rb_raise(rb_eRuntimeError, "Document is not wellformed");
  }

  prbxs_dom->doc = ctxt->myDoc;
  xmlFreeParserCtxt(ctxt);
  xmlCleanupParser(); // Hmmm, no problem to call it as this point
    
  if (rb_block_given_p()) {
    rb_yield(Data_Wrap_Struct(cSmartDom, rbxs_dom_mark, rbxs_dom_free, prbxs_dom));
    if (save > 0) {
      int status = -1;
      status = xmlSaveFormatFile(StringValuePtr(name), prbxs_dom->doc, prbxs_dom->saveUnformated);
      if (prbxs_dom->lock > 0) {
        unlink(lockfile);
        free(lockfile);
      }  
      if (status == -1)
        rb_raise(rb_eRuntimeError, "Unable to write file back to disk");
    }  
    return(Qnil);
  }

  return(Data_Wrap_Struct(cSmartDom, rbxs_dom_mark, rbxs_dom_free, prbxs_dom));
}

//***********************************************************************************
// Initialize module and class
//***********************************************************************************
#ifdef RDOC__
  mXML  = rb_define_module( "XML" );
  cSmart = rb_define_class_under( mXML, "Smart", rb_cObject );
#endif
VALUE cSmartDom;

void init_rbxs_dom( void ) {
  cSmartDom = rb_define_class_under( cSmart, "Dom", rb_cObject );
  
  rb_define_const(cSmartDom, "SIGNAL_ADD",    INT2NUM(RBXS_DOM_SIGNAL_ADD   ));
  rb_define_const(cSmartDom, "SIGNAL_CHANGE", INT2NUM(RBXS_DOM_SIGNAL_CHANGE));
  rb_define_const(cSmartDom, "SIGNAL_DELETE", INT2NUM(RBXS_DOM_SIGNAL_DELETE));
  
  rb_define_method(cSmartDom, "inspect",          (VALUE(*)(ANYARGS))rbxs_dom_inspect,              0);
  rb_define_method(cSmartDom, "dup",              (VALUE(*)(ANYARGS))rbxs_dom_dup,                  0);
  rb_define_method(cSmartDom, "clone",            (VALUE(*)(ANYARGS))rbxs_dom_dup,                  0);
  rb_define_method(cSmartDom, "to_s",             (VALUE(*)(ANYARGS))rbxs_dom_to_s,                 0);
  rb_define_method(cSmartDom, "root",             (VALUE(*)(ANYARGS))rbxs_dom_root,                 0);
  rb_define_method(cSmartDom, "root=",            (VALUE(*)(ANYARGS))rbxs_dom_root_set,             1);
  rb_define_method(cSmartDom, "find",             (VALUE(*)(ANYARGS))rbxs_dom_find,                -1);
  rb_define_method(cSmartDom, "on_change",        (VALUE(*)(ANYARGS))rbxs_dom_change_handler_set,   0);
  rb_define_method(cSmartDom, "change_handlers",  (VALUE(*)(ANYARGS))rbxs_dom_change_handlers,      0);
  rb_define_method(cSmartDom, "save_as",          (VALUE(*)(ANYARGS))rbxs_dom_save_as,              1);
  rb_define_method(cSmartDom, "namespaces=",      (VALUE(*)(ANYARGS))rbxs_dom_namespaces_set,       1);
  rb_define_method(cSmartDom, "namespaces",       (VALUE(*)(ANYARGS))rbxs_dom_namespaces_get,       0);
  rb_define_method(cSmartDom, "encode_entities=", (VALUE(*)(ANYARGS))rbxs_dom_encode_entities_set,  1);
  rb_define_method(cSmartDom, "encode_entities?", (VALUE(*)(ANYARGS))rbxs_dom_encode_entities_q,    0);
  rb_define_method(cSmartDom, "save_unformated=", (VALUE(*)(ANYARGS))rbxs_dom_save_unformated_set,  1);
  rb_define_method(cSmartDom, "save_unformated?", (VALUE(*)(ANYARGS))rbxs_dom_save_unformated_q,    0);
  rb_define_method(cSmartDom, "xinclude!",        (VALUE(*)(ANYARGS))rbxs_dom_xinclude,             0);
  rb_define_method(cSmartDom, "validate_against", (VALUE(*)(ANYARGS))rbxs_dom_validate_relaxng,     1);
}
