/* Please see the COPYING file for copyright and distribution information */

#ifndef __RBXS_PULLATTRIBUTESET_H__
#define __RBXS_PULLATTRIBUTESET_H__

#include "rbxs.h"

RUBY_EXTERN VALUE cSmartPullAttributeSet;

typedef struct rbxs_pullattributeset {
  VALUE node;
} rbxs_pullattributeset;

RUBY_EXTERN VALUE rbxs_pullattributeset_new(VALUE class, VALUE node);
RUBY_EXTERN void  init_rbxs_pullattributeset(void);
  
#endif
