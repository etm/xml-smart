/* Please see the COPYING file for copyright and distribution information */

#ifndef __RBXS_DOMNODESET_H__
#define __RBXS_DOMNODESET_H__

#include "rbxs.h"

RUBY_EXTERN VALUE cSmartDomNodeSet;

typedef struct rbxs_domnodeset {
  VALUE              doc;
  xmlXPathObjectPtr  obj;
  xmlNodeSetPtr      nodeset;
} rbxs_domnodeset;

RUBY_EXTERN VALUE rbxs_domnodeset_new(VALUE class, VALUE doc, xmlXPathObjectPtr obj);
RUBY_EXTERN void  init_rbxs_domnodeset(void);
  
#endif
