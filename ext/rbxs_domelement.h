/* Please see the COPYING file for copyright and distribution information */

#ifndef __RBXS_DOMELEMENT_H__
#define __RBXS_DOMELEMENT_H__

#include "rbxs.h"

RUBY_EXTERN VALUE cSmartDomElement;

typedef struct rbxs_domelement {
  VALUE      doc;
  xmlNodePtr node;
} rbxs_domelement;

RUBY_EXTERN VALUE rbxs_domelement_new(VALUE class, VALUE doc, xmlNodePtr node);
RUBY_EXTERN void  init_rbxs_domelement(void);
  
#endif
