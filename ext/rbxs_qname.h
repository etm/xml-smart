/* Please see the COPYING file for copyright and distribution information */

#ifndef __RBXS_QNAME_H__
#define __RBXS_QNAME_H__

#include "rbxs.h"

RUBY_EXTERN VALUE cSmartQName;

typedef struct rbxs_qname {
  VALUE obj;
  int   type;
} rbxs_qname;

RUBY_EXTERN VALUE rbxs_qname_new(VALUE class, VALUE obj, int type);
RUBY_EXTERN void  init_rbxs_qname(void);
  
#endif
