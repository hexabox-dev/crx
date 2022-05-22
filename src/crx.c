/*
 * name: crx
 * description: Regular Expression Engine (light weight version) for C Language, using double-recursion and function pointers.
 * author: ken seto (hexabox)
 * copyright: 2009~now
 * license: GPLv3
 *
 * some abbreviations i use:
 *	 sam		sample string
 *	 pat		pattern string
 *	 cur		current pointer of string i.e. char[]
 *	 endp		end of pattern string '\0'
 */
#include "crx.h"

#include <string.h>
//#include <stdlib.h>   // for atoi
int atoi(const char *s) {
    int v=0;
    while (s && *s >= '0' && *s <='9')
	    v = v*10 + *s++ - '0';
    return v;
}

#ifdef _MSC_VER
#define inline __inline
#endif

#define TYPE_CHAR   1
#define TYPE_CMD    2
#define TYPE_PREFIX 4
#define TYPE_SUFFIX 8
#define TYPE_OPEN   16
#define TYPE_CLOSE  32

// function pointers
#define CMD2    (int(*)(char*,char*))
#define CMD3    (int(*)(char*,char*,char*))

// command handlers short-hand
#define _CMD2(str)  int c_##str(char* pat, char* sam)
#define _CMD3(str)  int c_##str(char* pat, char* sam, char* endp)


// declare function signatures
// the main one ...
int match(char*, char*, char*);
/* 
 * ... and those cmd handlers
 * rules of commands:
 * 1. return number of consumed characters in sample string
 * 2. TYPE_CLOSE follows TYPE_OPEN immediately in command table
 */
inline _CMD2(achar);
inline _CMD2(any);
inline _CMD2(escape);
_CMD2(group);
_CMD3(multi);
_CMD2(option);


// Cmd struct
typedef struct {
    char    id;
    int     span;
    void*   fcn;
    int     type;
} Cmd;

Cmd cmd_tbl[] = {
    '(',    0,    (void*)c_group,    TYPE_OPEN,   
    ')',    1,    (void*)NULL,       TYPE_CLOSE, 
    '[',    0,    (void*)c_option,   TYPE_OPEN,  
    ']',    1,    (void*)NULL,       TYPE_CLOSE, 
    '{',    0,    (void*)c_multi,    TYPE_SUFFIX|TYPE_OPEN,
    '}',    1,    (void*)NULL,       TYPE_CLOSE, 
    '*',    1,    (void*)c_multi,    TYPE_SUFFIX,
    '+',    1,    (void*)c_multi,    TYPE_SUFFIX,
    '?',    1,    (void*)c_multi,    TYPE_SUFFIX,
    '.',    1,    (void*)c_any,      TYPE_CMD,   
   '\\',    2,    (void*)c_escape,   TYPE_PREFIX,
      0,    1,    (void*)c_achar,    TYPE_CHAR,  
};

Cmd* get_cmd(char id)
{
    Cmd* cmd = &cmd_tbl[0];

    while (cmd->id != 0)
    {
        if (id == cmd->id)
            break;
        cmd++;
    }
    return cmd;
}

static inline bool is_suffix(char id)
{
    Cmd* cmd = get_cmd(id);
    return (cmd ? (cmd->type & TYPE_SUFFIX) == TYPE_SUFFIX : false);
}

char* find_close(char* init, char stop)
{
    int cnt = 0;
    char* ptr = init;

    while (*ptr)
    {
        if (*ptr == *init)
            cnt++;
        else if (*ptr == stop)
            cnt--;

        if (cnt == 0)
            return ptr;

        ptr++;
    }
    return NULL;
}

char* get_next_pat(char* cur)   // find next unit of pattern
{
    Cmd* cmd = get_cmd(*cur);

    if (cmd->type & TYPE_OPEN)
    {
        cur = find_close(cur, (cmd+1)->id);
        if (cur)
            return (cur + 1);
    }
    return (cur + cmd->span);
}


// -------------- command handlers ----------------

inline _CMD2(any) {return 1;}
inline _CMD2(achar) {return *pat == *sam ? 1 : EOF;}

_CMD2(group)    // sub pattern, imagine movie 'inception'
{
    char *close = find_close(pat, ')');
    if (!close) return EOF;	// wrong pattern error? // false;

    return match(pat+1, sam, close);
}

_CMD2(escape)
{
    char magic[16] = "";	// WARNING: do not use patterns longer than 15 chars!
    
    switch (*++pat)
    {
        // problem 1: inefficient copying everytime
        // problem 2: mem illegal access for long magic
        case 'd':   // digit
            strcpy(magic, "[0-9]");
            break;
        case 'D':   // non-digit
            strcpy(magic, "[^0-9]");
            break;
        case 'x':   // hex digit
            strcpy(magic, "[0-9A-Fa-f]");
            break;
        case 'X':
            strcpy(magic, "[^0-9A-Fa-f]");
            break;
        case 'o':   // octal digit
            strcpy(magic, "[0-7]");
            break;
        case 'O':
            strcpy(magic, "[^0-7]");
            break;
        case 'w':   // word character
            strcpy(magic, "[0-9A-Za-z_]");
            break;
        case 'W':
            strcpy(magic, "[^0-9A-Za-z_]");
            break;
        case 'h':   // head of word character
            strcpy(magic, "[0-9A-Za-z]");
            break;
        case 'H':
            strcpy(magic, "[^0-9A-Za-z]");
            break;
        case 'a':   // alphabetic character
            strcpy(magic, "[A-Za-z]");
            break;
        case 'A':
            strcpy(magic, "[^A-Za-z]");
            break;
        case 'l':   // lowercase character
            strcpy(magic, "[a-z]");
            break;
        case 'L':
            strcpy(magic, "[^a-z]");
            break;
        case 'u':   // uppercase character
            strcpy(magic, "[A-Z]");
            break;
        case 'U':
            strcpy(magic, "[^A-Z]");
            break;
        case 's':	// spaces
            strcpy(magic, "[ \t]");
            break;
        case 'S':
            strcpy(magic, "[^ \t]");
            break;
    }

    if (*magic)
        return match(magic, sam, strchr(magic, 0));
    else
        return *pat == *sam ? 1 : EOF;
}


_CMD2(option)
{
    bool invert;
    char* from = NULL;
    char* to = NULL;
    char *close;

    close = pat;
    do {
        close = find_close(close, ']');
        if (!close) return EOF; // wrong pattern? // false;
    } while (close[-1] == '\\');

    invert = (pat[1] == '^');
    pat += invert ? 2 : 1;

    while (pat < close)
    {
        if (*pat == '-' && from)
        {
            to = pat + 1;
            if (*to == '\\') to ++;
            if (to >= close) break;
            if (*sam >= *from && *sam <= *to)
                return (invert ? EOF : 1);
            pat = to + 1;
            continue;
        }

        from = pat;
        if (*from == '\\') from ++;
        if (from >= close) break;
        if (*sam == *from)
            return (invert ? EOF : 1);

        pat++;
    }

    return (invert ? 1 : EOF);
}

_CMD3(multi)
{
    // repeated variables
    Cmd* cmd = get_cmd(*pat);
    int  found;
    char* start_sam = sam;

    // new local variables
    int repeat = 0;
    int good_follows = 0;

    char *ends = strchr(sam, 0);
    char *good_sam = NULL;

    char *multi = get_next_pat(pat);
    char *next_pat = get_next_pat(multi);

    int min = 0, max = 0;

    switch (*multi)
    {
        case '{':  // for range of repetition
            {
                char* comma = strchr(multi, ',');
                char* close = strchr(multi, '}');

                min = atoi(multi + 1);
                if (comma && comma < close)
                    max = atoi(comma + 1);
                else
                    max = min;
            } break;
        case '+':
            min = 1;
            break;
        case '?':
            max = 1;
            break;
    }

    while (sam < ends)
    {
        found = (CMD2 cmd->fcn)(pat, sam);

        if (found < 0) break;  // can be less than min

        // condition 1
        repeat ++;
        sam += found;   // advance ptr

        if (min && repeat < min)
            continue;

        if (*next_pat)
        {
            if (sam >= ends)
                break;

            found = match(next_pat, sam, endp);
            if (found >= 0)    // condition 2
            {
                good_follows = found;
                good_sam = sam;
            }
        }

        if (max && repeat >= max)
            break;
    }

    // return here
    if (repeat < min)
        found = EOF;

    else if (!*next_pat)
        found = sam - start_sam;

    else if (good_sam)  // *next_pat > 0
        found = good_sam + good_follows - start_sam;

    else if (!min)
    {
        if (repeat == 0)    // no match before *multi
            found = match(next_pat, sam, endp);

        else if (!good_sam) // repeat > 0 is wrongly match before *multi
            found = match(next_pat, start_sam, endp);
    }

    else found = EOF;

    return found;
}

/*
 * return: number char found in sam, 0+
 * return EOF if not found.  i.e. 0 char is a valid result.
 */
int match(char* pat, char* sam, char* endp)
{
    char* start_sam = sam;	// remember where sample string starts, because we will iterate sam.

    while (pat < endp)
    {
        if (*pat == '\0')  break;		// all pattern consumed  i.e. found
        if (*sam == '\0')  return 0;	// sample string ran out i.e. not found

        Cmd* cmd = get_cmd(*pat);
        char* next_pat = get_next_pat(pat);
		int  found;

        if (next_pat  &&  pat < endp  &&  is_suffix(*next_pat))
        {
            cmd = get_cmd(*next_pat);

			found = (CMD3 cmd->fcn)(pat, sam, endp);
            if (found < 0) return EOF;

            return (sam - start_sam) + found;
        }
        else
        {
            found = (CMD2 cmd->fcn)(pat, sam);
            if (found < 0) return EOF;

            sam += found;
            pat = next_pat;
        }
    }

    return (sam - start_sam);
}

// -------------- external interface ---------------

/*
 * return pointer of found string, NULL otherwise.
 */
char* regex(char* pat, char* sam, int* len)
{
	char* endp = strchr(pat, '\0');		// endp not necessarily '\0' in mid string

    while (*pat && *sam)
    {
        *len = match(pat, sam, endp);
        if (*len >= 0)
            return sam;

        ++sam;		// try next character
    }

	*len = 0;
	return NULL;
}

