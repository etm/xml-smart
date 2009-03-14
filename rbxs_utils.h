/* Please see the COPYING file for copyright and distribution information */

#ifndef __RBXS_UTILS_H__
#define __RBXS_UTILS_H__

#include "rbxs.h"

//***********************************************************************************
// shamlessly copied from xmllint.c
//***********************************************************************************
xmlExternalEntityLoader defaultEntityLoader = NULL;
static xmlParserInputPtr xmllintExternalEntityLoader(const char *URL, const char *ID, xmlParserCtxtPtr ctxt) {
  xmlParserInputPtr ret;
  warningSAXFunc warning = NULL;

  if ((ctxt != NULL) && (ctxt->sax != NULL)) {
    warning = ctxt->sax->warning;
    ctxt->sax->warning = NULL;
  }

  if (defaultEntityLoader != NULL) {
    ret = defaultEntityLoader(URL, ID, ctxt);
    if (ret != NULL) {
      if (warning != NULL)
        ctxt->sax->warning = warning;
      return(ret);
    }
  }
  if (warning != NULL) {
    ctxt->sax->warning = warning;
    if (URL != NULL)
      warning(ctxt, "failed to load external entity \"%s\"\n", URL);
    else if (ID != NULL)
      warning(ctxt, "failed to load external entity \"%s\"\n", ID);
  }
  return(NULL);
}

#endif
