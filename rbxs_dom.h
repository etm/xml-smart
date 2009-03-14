/* Please see the COPYING file for copyright and distribution information */

#ifndef __RBXS_DOM_H__
#define __RBXS_DOM_H__

#include "rbxs.h"

#define RBXS_DOM_TYPE_STRING 0
#define RBXS_DOM_TYPE_FILE   1

#define RBXS_DOM_SIGNAL_ADD    0
#define RBXS_DOM_SIGNAL_CHANGE 1
#define RBXS_DOM_SIGNAL_DELETE 2

RUBY_EXTERN VALUE cSmartDom;

typedef struct rbxs_dom {
  unsigned short int encodeEntities;
  unsigned short int saveUnformated;
  int                type;
  unsigned short int lock;
  xmlDocPtr          doc;
  VALUE              changeHandlers;
  VALUE              namespaces;
} rbxs_dom;

void rbxs_dom_change_handlers_execute(rbxs_dom *prbxs_dom, unsigned short int type, VALUE node);
xmlNodePtr rbxs_dom_free_xinclude_node(xmlNodePtr cur);
void rbxs_dom_remove_xinclude_nodes(xmlNodePtr cur);

RUBY_EXTERN VALUE rbxs_dom_open(VALUE name, char *root, unsigned short int lock, unsigned short int save);
RUBY_EXTERN VALUE rbxs_dom_string(VALUE string);
RUBY_EXTERN void  init_rbxs_dom(void);

#endif
