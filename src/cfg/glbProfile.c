
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cupstype.h"
#include "glbProfile.h"

#define PF_FALSE                 0
#define PF_TRUE                  1

#define PFL_MEMO_CHARACTER       '#'
#define PFL_LINE_BUFFER_SIZE     1024    /* before PFL_MEMO_CHARACTER or '\n' */
#define PFL_FILE_NAME_LEN        1024

static void ClearBlank(char *);
static int IsSection(char *);
static int IsThisSection(const char *, const char *);
static int IsThisEntry(const char *, const char *);
static void CutContent(char *, char *);


/*
 *  Function:  ClearBlank
 *
 *      To remove all comment and blank char from one line
 *
 *  Parameters:
 *
 *      line - point of a string of a line
 *
 *  Return Value:
 */

static void ClearBlank(char *line)
{
    INT32 i = 0, j, k;
    char buf[PFL_LINE_BUFFER_SIZE];

    while (line[i] != 0) {
        if (line[i] == PFL_MEMO_CHARACTER || line[i] == '\n') {
            line[i] = 0;
            if (i == 0) break;
            j = i - 1;
            do {
                if (j < 0) break;
                if ((line[j] != ' ') && (line[j] != '\t')) break;
                line[j--] = 0;
            } while(1);
            break;
        }  
        i++;
    }
 
    i = 0;
    memset(buf, 0, PFL_LINE_BUFFER_SIZE);

    while ((line[i] != '=') && (i < (INT32)strlen(line)))
        i++;
    if (i == (INT32)strlen(line)) return;

    for (j = i - 1; (line[j] == ' ') || (line[j] == '\t'); j--);
    for (k = i + 1; (line[k] == ' ') || (line[k] == '\t'); k++);

    memcpy(buf, line, j + 1);
    buf[j + 1] = '=';
    strcat(buf + j + 2, line + k);
 
    strcpy(line, buf);
}


/*
 *  Function:  IsSection
 *
 *      To confirm if a line is a section line
 *
 *  Parameters:
 *
 *      line - point of a string of a line
 *
 *  Return Value:
 *
 *       0 - is not a section line
 *       1 - is a section line
 */

static int IsSection(char * line)
{
    return line[0] == '[';
}


/*
 *  Function:  IsThisSection
 *
 *      To confirm if this line is a spec section
 *
 *  Parameters:
 *
 *      line    - point of a string of a line
 *      section - name of section
 *
 *  Return Value:
 *
 *       0 - is not this section
 *       1 - is this section
 */

static int IsThisSection(const char *line, const char *section)
{
    return !memcmp(line + 1, section, strlen(section));
}


/*
 *  Function:  IsThisEntry
 *
 *      To confire if this line is a spec entry
 *
 *  Parameters:
 *
 *      line  - point of a string of a line
 *      entry - name of entry
 *
 *  Return Value:
 *
 *       0 - is not this entry
 *       1 - is this entry
 */

static int IsThisEntry(const char *line, const char *entry)
{
    return (!memcmp(line, entry, strlen(entry)) &&
            line[strlen(entry)] == '=') ;
} 


/*
 *  Function:  CutContent
 *
 *      To get the value from a line
 *
 *  Parameters:
 *
 *      line  - point of a string of a line
 *      dest  - value of entry to put
 *
 *  Return Value:
 */

static void CutContent(char *line, char *dest)
{
    INT32 i = 0;

    while (line[i++] != '=');

    strcpy(dest, line + i);
} 


/*
 *  Function:  glbPflGetString
 *
 *      To get a string value from profile
 *
 *  Parameters:
 *
 *      section  - section name
 *      entry    - entry name
 *      filename - full filename of profile
 *      value    - pointer of string which value is returned
 *
 *  Return Value:
 *
 *      SUCCESS - success
 *      FAILURE - failure
 */

extern int glbPflGetString(const char *section,
                           const char *entry,
                           const char *filename,
                           char *value)
{
    FILE * fp;
    char line[PFL_LINE_BUFFER_SIZE];
    INT32 cbNum = PF_FALSE;
    INT32 stNum = PF_FALSE;
    INT32 InThisSection = PF_FALSE;
  
    if ((fp = fopen(filename, "r")) == NULL) {
        return FAILURE;
    }

    while (NULL != fgets(line, PFL_LINE_BUFFER_SIZE, fp)) {
        ClearBlank(line);

        if (IsSection(line)) {
            InThisSection = IsThisSection(line, section);
            continue;
        }

        if (InThisSection == PF_FALSE) continue;

        stNum = PF_TRUE;
        if (IsThisEntry(line, entry)) {
            CutContent(line, value);
            cbNum = PF_TRUE;
            break;
        }
    }

    fclose(fp);

    if (stNum == PF_FALSE) {
      return -2;
    }

    if (cbNum == PF_FALSE) {
        return FAILURE;
    }

    return SUCCESS;
}


/*
 *  Function:  glbPflGetInt
 *
 *      To get a int value from profile
 *
 *  Parameters:
 *
 *      section  - section name
 *      entry    - entry name
 *      filename - full filename of profile
 *      value    - pointer of int which value is returned
 *
 *  Return Value:
 *
 *      SUCCESS - success
 *      FAILURE - failure
 */

extern int glbPflGetInt(const char *section,
                        const char *entry,
                        const char *filename,
                        INT32 *value)
{
    char buf[PFL_LINE_BUFFER_SIZE];

    if (glbPflGetString(section, entry, filename, buf) != SUCCESS) {
        *value = 0;
        return FAILURE;
    }
    else {
        *value = atoi(buf);
  return SUCCESS;
    }
}


/*
 *  Function:  glbPflGetLong
 *
 *      To get a long value from profile
 *
 *  Parameters:
 *
 *      section  - section name
 *      entry    - entry name
 *      filename - full filename of profile
 *      value    - pointer of long which value is returned
 *
 *  Return Value:
 *
 *      SUCCESS - success
 *      FAILURE - failure
 */

extern int glbPflGetLong(const char *section,
                         const char *entry,
                         const char *filename,
                         INT64 *value)
{
    char buf[PFL_LINE_BUFFER_SIZE];

    if (glbPflGetString(section, entry, filename, buf) != SUCCESS) {
        *value = 0L;
        return FAILURE;
    }
    else {
        *value = atol(buf);
        return SUCCESS;
    }
}

extern int glbPflSetString(const char *section,
                           const char *entry,
                           const char *filename,
                           char *value)
{
    FILE * fp;
    FILE * fnew;
    char line[PFL_LINE_BUFFER_SIZE];
    char useln[PFL_LINE_BUFFER_SIZE];
    char newfile[300];
    INT32 cbNum = PF_FALSE;
    INT32 stNum = PF_FALSE;
    INT32 InThisSection = PF_FALSE;
    int ipos=0;
  
    if ((fp = fopen(filename, "rt")) == NULL) {
        return FAILURE;
    }
    memset(newfile,0,sizeof(newfile));
    strcpy(newfile,filename);
    strcat(newfile,".new");
    if ((fnew = fopen(newfile, "wt")) == NULL) {
        return FAILURE;
    }

    while (NULL != fgets(line, PFL_LINE_BUFFER_SIZE, fp)) {
        fflush(fnew);
        memcpy(useln,line,sizeof(line));
        if(cbNum==PF_TRUE){
          fprintf(fnew,"%s",useln);
          continue;
        }
        ClearBlank(line);

        if (IsSection(line)) {
            InThisSection = IsThisSection(line, section);
            fprintf(fnew,"%s",useln);
            continue;
        }

        if (InThisSection == PF_FALSE){
            fprintf(fnew,"%s",useln);
          continue;
        }

        stNum = PF_TRUE;
        if (IsThisEntry(line, entry)) {
            memset(&line[strlen(entry)+1],0,sizeof(line)-strlen(entry)-1);
            memcpy(&line[strlen(entry)+1],value,strlen(value));
            memcpy(&line[strlen(line)],"\n",1);
            fprintf(fnew,"%s",line);

            cbNum = PF_TRUE;
            continue;
        }
         fprintf(fnew,"%s",useln);
    }

    fclose(fp);
    fflush(fnew);
    fclose(fnew);

    remove(filename);
    rename(newfile,filename);
    
    if (stNum == PF_FALSE) {
      return -2;
    }

    if (cbNum == PF_FALSE) {
        return FAILURE;
    }

    return SUCCESS;
}


