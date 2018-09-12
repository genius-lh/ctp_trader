
#ifndef __GLBPROFILE_H
#define __GLBPROFILE_H
#include "cupstype.h"

int glbPflGetInt(const char *section,
                 const char *entry,
                 const char *filename,
                 INT32 *value);
int glbPflGetLong(const char *section,
                  const char *entry,
                  const char *filename,
                  INT64 *value);
int glbPflGetString(const char *section,
                    const char *entry,
                    const char *filename,
                    char *value);

#endif
