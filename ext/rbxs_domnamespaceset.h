/* Please see the COPYING file for copyright and distribution information */

#ifndef __RBXS_DOMNAMESPACESET_H__
#define __RBXS_DOMNAMESPACESET_H__

#include "rbxs.h"

RUBY_EXTERN VALUE cSmartDomNamespaceSet;

typedef struct rbxs_domnamespaceset {
  VALUE node;
} rbxs_domnamespaceset;

RUBY_EXTERN VALUE rbxs_domnamespaceset_new(VALUE class, VALUE node);
RUBY_EXTERN void  init_rbxs_domnamespaceset(void);
  
#endif
