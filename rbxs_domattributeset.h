/* Please see the COPYING file for copyright and distribution information */

#ifndef __RBXS_DOMATTRIBUTESET_H__
#define __RBXS_DOMATTRIBUTESET_H__

#include "rbxs.h"

RUBY_EXTERN VALUE cSmartDomAttributeSet;

typedef struct rbxs_domattributeset {
  VALUE node;
} rbxs_domattributeset;

RUBY_EXTERN VALUE rbxs_domattributeset_new(VALUE class, VALUE node);
RUBY_EXTERN void  init_rbxs_domattributeset(void);
  
#endif
