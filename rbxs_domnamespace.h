/* Please see the COPYING file for copyright and distribution information */

#ifndef __RBXS_DOMNAMESPACE_H__
#define __RBXS_DOMNAMESPACE_H__

#include "rbxs.h"

RUBY_EXTERN VALUE cSmartDomNamespace;

typedef struct rbxs_domnamespace {
  VALUE node;
  xmlNsPtr ns;
} rbxs_domnamespace;

RUBY_EXTERN VALUE rbxs_domnamespace_new(VALUE class, VALUE node, xmlNsPtr ns);
RUBY_EXTERN void  init_rbxs_domnamespace(void);
  
#endif
