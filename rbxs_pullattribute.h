/* Please see the COPYING file for copyright and distribution information */

#ifndef __RBXS_PULLATTRIBUTE_H__
#define __RBXS_PULLATTRIBUTE_H__

#include "rbxs.h"

RUBY_EXTERN VALUE cSmartPullAttribute;

typedef struct rbxs_pullattribute {
  VALUE node;
  VALUE which;
} rbxs_pullattribute;

RUBY_EXTERN VALUE rbxs_pullattribute_new(VALUE class, VALUE node, VALUE which);
RUBY_EXTERN void  init_rbxs_pullattribute(void);
  
#endif
