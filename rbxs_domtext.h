/* Please see the COPYING file for copyright and distribution information */

#ifndef __RBXS_DOMTEXT_H__
#define __RBXS_DOMTEXT_H__

#include "rbxs.h"

RUBY_EXTERN VALUE cSmartDomText;

typedef struct rbxs_domtext {
  VALUE      doc;
  xmlNodePtr node;
} rbxs_domtext;

RUBY_EXTERN VALUE rbxs_domtext_new(VALUE class, VALUE doc, xmlNodePtr node);
RUBY_EXTERN void  init_rbxs_domtext(void);
  
#endif
