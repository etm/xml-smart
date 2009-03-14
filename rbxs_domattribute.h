/* Please see the COPYING file for copyright and distribution information */

#ifndef __RBXS_DOMATTRIBUTE_H__
#define __RBXS_DOMATTRIBUTE_H__

#include "rbxs.h"

RUBY_EXTERN VALUE cSmartDomAttribute;

typedef struct rbxs_domattribute {
  VALUE      doc;
  xmlAttrPtr attribute;
} rbxs_domattribute;

RUBY_EXTERN VALUE rbxs_domattribute_new(VALUE class, VALUE doc, xmlAttrPtr attribute);
RUBY_EXTERN void  init_rbxs_domattribute(void);
  
#endif
