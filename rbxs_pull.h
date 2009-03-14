/* Please see the COPYING file for copyright and distribution information */

#ifndef __RBXS_PULL_H__
#define __RBXS_PULL_H__

#include "rbxs.h"

RUBY_EXTERN VALUE cSmartPull;

typedef struct rbxs_pull {
  xmlTextReaderPtr reader;
} rbxs_pull;

RUBY_EXTERN VALUE rbxs_pull_new(VALUE class, VALUE what);
RUBY_EXTERN void  init_rbxs_pull(void);
  
#endif
