/* Please see the COPYING file for copyright and distribution information */

#ifndef __RBXS_H__
#define __RBXS_H__

#include <ruby.h>
#ifdef RUBY_VM
#include <ruby/io.h>
#include <ruby/st.h>
#else
#include <version.h>
#include <rubyio.h>
#include <st.h>
#if (RUBY_VERSION_MAJOR == 1 &&  RUBY_VERSION_MINOR == 8 && RUBY_VERSION_TEENY < 7)
#define RHASH_SIZE(h) (RHASH(h)->tbl ? RHASH(h)->tbl->num_entries : 0)
#endif
#define RHASH_EMPTY_P(h) (RHASH_SIZE(h) == 0)
#endif

#ifdef HAVE_RB_IO_T
#define RB_IO_T rb_io_t
#if (RUBY_VERSION_MAJOR == 1 &&  RUBY_VERSION_MINOR == 8 && RUBY_VERSION_TEENY < 7)
#define RB_IO_T_FD(o) o->fd
#else
#define RB_IO_T_FD(o) fileno(o->stdio_file)
#endif
#else
#define RB_IO_T OpenFile
#define RB_IO_T_FD(o) fileno(o->f)
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlsave.h>
#include <libxml/xinclude.h>

#include <libxslt/extra.h>
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>
#include <libxslt/imports.h>

#define RBXS_VERSION  "0.2.3"

#define RBXS_PARSER_TYPE_DOM    0
#define RBXS_PARSER_TYPE_READER 1

RUBY_EXTERN VALUE mXML;
RUBY_EXTERN VALUE cSmart;

#endif
