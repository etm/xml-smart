/* Please see the COPYING file for copyright and distribution information */

#ifndef __RBXS_DOMOTHER_H__
#define __RBXS_DOMOTHER_H__

#include "rbxs.h"

RUBY_EXTERN VALUE cSmartDomOther;

typedef struct rbxs_domother {
  VALUE      doc;
  xmlNodePtr node;
} rbxs_domother;

RUBY_EXTERN VALUE rbxs_domother_new(VALUE class, VALUE doc, xmlNodePtr node);
RUBY_EXTERN void  init_rbxs_domother(void);
  
#endif
