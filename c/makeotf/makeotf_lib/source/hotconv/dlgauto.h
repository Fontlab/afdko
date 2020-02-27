/* dlgauto.h automaton
 *
 * SOFTWARE RIGHTS
 *
 * We reserve no LEGAL rights to the Purdue Compiler Construction Tool
 * Set (PCCTS) -- PCCTS is in the public domain.  An individual or
 * company may do whatever they wish with source code distributed with
 * PCCTS or the code generated by PCCTS, including the incorporation of
 * PCCTS, or its output, into commerical software.
 *
 * We encourage users to develop software with PCCTS.  However, we do ask
 * that credit is given to us for developing PCCTS.  By "credit",
 * we mean that if you incorporate our source code into one of your
 * programs (commercial product, research project, or otherwise) that you
 * acknowledge this fact somewhere in the documentation, research report,
 * etc...  If you like PCCTS and have developed a nice tool with the
 * output, please mention that you developed it using PCCTS.  In
 * addition, we ask that this header remain intact in our source code.
 * As long as these guidelines are kept, we expect to continue enhancing
 * this system and expect to make other tools available as they are
 * completed.
 *
 * ANTLR 1.33
 * Will Cohen and Terence Parr
 * Parr Research Corporation
 * with Purdue University and AHPCRC, University of Minnesota
 * 1989-1995
 */

#ifndef ZZDEFAUTO_H
#define ZZDEFAUTO_H

/* 10-Apr-97  133MR1  Uses __USE_PROTOS show should #include config.h */

#include "config.h"

zzchar_t *zzlextext;                                       /* text of most recently matched token */
zzchar_t *zzbegexpr;                                       /* beginning of last reg expr recogn. */
zzchar_t *zzendexpr;                                       /* beginning of last reg expr recogn. */
int zzbufsize = 0; /* number of characters in zzlextext */ /* MR7 */
int zzbegcol = 0;                                          /* column that first character of token is in*/
int zzendcol = 0;                                          /* column that last character of token is in */
int zzline = 1;                                            /* line current token is on */
int zzreal_line = 1;                                       /* line of 1st portion of token that is not skipped */
int zzchar;                                                /* character to determine next state */
int zzbufovf;                                              /* indicates that buffer too small for text */
int zzcharfull = 0;
static zzchar_t *zznextpos; /* points to next available position in zzlextext*/
static int zzclass;
//FONTLAB
int err_fatal_linenum = -1;


#ifdef __USE_PROTOS
void zzerrstd(const char *);

void (*zzerr)(const char *) = zzerrstd; /* pointer to error reporting function */
extern int zzerr_in(void);

#else
void zzerrstd();

void (*zzerr)() = zzerrstd; /* pointer to error reporting function */
extern int zzerr_in();

#endif

static FILE *zzstream_in = 0;
static int (*zzfunc_in)(void) = zzerr_in;
static zzchar_t *zzstr_in = 0;


//FONTLAB
//#ifdef USER_ZZMODE_STACK
int zzauto = 0;
//#else
//static int zzauto = 0;
//#endif

static int zzadd_erase;
static char zzebuf[70];

#ifdef ZZCOL
#define ZZINC (++zzendcol)
#else
#define ZZINC
#endif

#define ZZGETC_STREAM               \
    {                               \
        zzchar = getc(zzstream_in); \
        zzclass = ZZSHIFT(zzchar);  \
    }
#define ZZGETC_FUNC                \
    {                              \
        zzchar = (*zzfunc_in)();   \
        zzclass = ZZSHIFT(zzchar); \
    }
#define ZZGETC_STR                 \
    {                              \
        if (*zzstr_in) {           \
            zzchar = *zzstr_in;    \
            ++zzstr_in;            \
        } else {                   \
            zzchar = EOF;          \
        }                          \
        zzclass = ZZSHIFT(zzchar); \
    }

#define ZZNEWSTATE (newstate = dfa[state][zzclass])

#ifndef ZZCOPY
#define ZZCOPY                                            \
    /* Truncate matching buffer to size (not an error) */ \
    if (zznextpos < lastpos) {                            \
        *(zznextpos++) = zzchar;                          \
    } else {                                              \
        zzbufovf = 1;                                     \
    }
#endif

void
#ifdef __USE_PROTOS
zzrdstream(FILE *f)
#else
    zzrdstream(f)
        FILE *f;

#endif
{
    /* make sure that it is really set to something, */
    /* otherwise just leave it be.                   */
    if (f) {
        /* make sure that there is always someplace */
        /* to get input before closing zzstream_in  */
#if 0
        if (zzstream_in && zzstream_in != stdin) {
            fclose(zzstream_in);
        }
#endif
        zzline = 1;
        zzstream_in = f;
        zzfunc_in = NULL;
        zzstr_in = 0;
        zzcharfull = 0;
    }
}

void
#ifdef __USE_PROTOS
zzrdfunc(int (*f)(void))
#else
    zzrdfunc(f) int (*f)();
#endif
{
    /* make sure that it is really set to something, */
    /* otherwise just leave it be.                   */
    if (f) {
        /* make sure that there is always someplace */
        /* to get input before closing zzstream_in  */
#if 0
        if (zzstream_in && zzstream_in != stdin) {
            fclose(zzstream_in);
        }
#endif
        zzline = 1;
        zzstream_in = NULL;
        zzfunc_in = f;
        zzstr_in = 0;
        zzcharfull = 0;
    }
}

void
#ifdef __USE_PROTOS
zzrdstr(zzchar_t *s)
#else
    zzrdstr(s)
        zzchar_t *s;

#endif
{
    /* make sure that it is really set to something, */
    /* otherwise just leave it be.                   */
    if (s) {
        /* make sure that there is always someplace */
        /* to get input before closing zzstream_in  */
#if 0
        if (zzstream_in && zzstream_in != stdin) {
            fclose(zzstream_in);
        }
#endif
        zzline = 1;
        zzstream_in = NULL;
        zzfunc_in = 0;
        zzstr_in = s;
        zzcharfull = 0;
    }
}

void zzclose_stream() {
#if 0
    fclose(zzstream_in);
    zzstream_in = NULL;
    zzfunc_in = NULL;
#endif
}

/* saves dlg state, but not what feeds dlg (such as file position) */
void
#ifdef __USE_PROTOS
zzsave_dlg_state(struct zzdlg_state *state)
#else
    zzsave_dlg_state(state) struct zzdlg_state *state;

#endif
{
    state->stream = zzstream_in;
    state->func_ptr = zzfunc_in;
    state->str = zzstr_in;
    state->auto_num = zzauto;
    state->add_erase = zzadd_erase;
    state->lookc = zzchar;
    state->char_full = zzcharfull;
    state->begcol = zzbegcol;
    state->endcol = zzendcol;
    state->line = zzline;
    state->lextext = zzlextext;
    state->begexpr = zzbegexpr;
    state->endexpr = zzendexpr;
    state->bufsize = zzbufsize;
    state->bufovf = zzbufovf;
    state->nextpos = zznextpos;
    state->class_num = zzclass;
}

void
#ifdef __USE_PROTOS
zzrestore_dlg_state(struct zzdlg_state *state)
#else
    zzrestore_dlg_state(state) struct zzdlg_state *state;

#endif
{
    zzstream_in = state->stream;
    zzfunc_in = state->func_ptr;
    zzstr_in = state->str;
    zzauto = state->auto_num;
    zzadd_erase = state->add_erase;
    zzchar = state->lookc;
    zzcharfull = state->char_full;
    zzbegcol = state->begcol;
    zzendcol = state->endcol;
    zzline = state->line;
    zzlextext = state->lextext;
    zzbegexpr = state->begexpr;
    zzendexpr = state->endexpr;
    zzbufsize = state->bufsize;
    zzbufovf = state->bufovf;
    zznextpos = state->nextpos;
    zzclass = state->class_num;
}

void
#ifdef __USE_PROTOS
zzmode(int m)
#else
    zzmode(m) int m;

#endif
{
    /* points to base of dfa table */
    if (m < MAX_MODE) {
        zzauto = m;
        /* have to redo class since using different compression */
        zzclass = ZZSHIFT(zzchar);
    } else {
        sprintf(zzebuf, "Invalid automaton mode = %d ", m);
        zzerr(zzebuf);
    }
}

/* erase what is currently in the buffer, and get a new reg. expr */
void zzskip() {
    zzadd_erase = 1;
}

/* don't erase what is in the zzlextext buffer, add on to it */
void zzmore() {
    zzadd_erase = 2;
}

/* substitute c for the reg. expr last matched and is in the buffer */
#ifdef __USE_PROTOS
void zzreplchar(zzchar_t c)
#else
void
    zzreplchar(c)
        zzchar_t c;
#endif
{
    /* can't allow overwriting null at end of string */
    if (zzbegexpr < &zzlextext[zzbufsize - 1]) {
        *zzbegexpr = c;
        *(zzbegexpr + 1) = '\0';
    }
    zzendexpr = zzbegexpr;
    zznextpos = zzbegexpr + 1;
}

/* replace the string s for the reg. expr last matched and in the buffer */
void
#ifdef __USE_PROTOS
zzreplstr(register zzchar_t *s)
#else
    zzreplstr(s) register zzchar_t *s;

#endif
{
    register zzchar_t *l = &zzlextext[zzbufsize - 1];

    zznextpos = zzbegexpr;
    if (s) {
        while ((zznextpos <= l) && (*(zznextpos++) = *(s++)) != 0) {
            /* empty */
        }
        /* correct for NULL at end of string */
        zznextpos--;
    }
    if ((zznextpos <= l) && (*(--s) == 0)) {
        zzbufovf = 0;
    } else {
        zzbufovf = 1;
    }
    *(zznextpos) = '\0';
    zzendexpr = zznextpos - 1;
}

void zzgettok() {
    register int state, newstate;
    /* last space reserved for the null char */
    register zzchar_t *lastpos;

skip:
    zzreal_line = zzline;
    zzbufovf = 0;
    lastpos = &zzlextext[zzbufsize - 1];
    zznextpos = zzlextext;
    zzbegcol = zzendcol + 1;
more:
    zzbegexpr = zznextpos;
#ifdef ZZINTERACTIVE
    /* interactive version of automaton */
    /* if there is something in zzchar, process it */
    state = newstate = dfa_base[zzauto];
    if (zzcharfull) {
        ZZINC;
        ZZCOPY;
        ZZNEWSTATE;
    }
    if (zzstr_in) {
        while (zzalternatives[newstate]) {
            state = newstate;
            ZZGETC_STR;
            ZZINC;
            ZZCOPY;
            ZZNEWSTATE;
        }
    } else if (zzstream_in) {
        while (zzalternatives[newstate]) {
            state = newstate;
            ZZGETC_STREAM;
            ZZINC;
            ZZCOPY;
            ZZNEWSTATE;
        }
    } else if (zzfunc_in) {
        while (zzalternatives[newstate]) {
            state = newstate;
            ZZGETC_FUNC;
            ZZINC;
            ZZCOPY;
            ZZNEWSTATE;
        }
    }
    /* figure out if last character really part of token */
    if ((state != dfa_base[zzauto]) && (newstate == DfaStates)) {
        zzcharfull = 1;
        --zznextpos;
    } else {
        zzcharfull = 0;
        state = newstate;
    }
    *(zznextpos) = '\0';
    /* Able to transition out of start state to some non err state?*/
    if (state == dfa_base[zzauto]) {
        /* make sure doesn't get stuck */
        zzadvance();
    }
#else
    /* non-interactive version of automaton */
    if (!zzcharfull) {
        zzadvance();
    } else {
        ZZINC;
    }
    state = dfa_base[zzauto];
    if (zzstr_in) {
        while (ZZNEWSTATE != DfaStates) {
            state = newstate;
            ZZCOPY;
            ZZGETC_STR;
            ZZINC;
        }
    } else if (zzstream_in) {
        while (ZZNEWSTATE != DfaStates) {
            state = newstate;
            ZZCOPY;
            ZZGETC_STREAM;
            ZZINC;
        }
    } else if (zzfunc_in) {
        while (ZZNEWSTATE != DfaStates) {
            state = newstate;
            ZZCOPY;
            ZZGETC_FUNC;
            ZZINC;
        }
    }
    zzcharfull = 1;
    if (state == dfa_base[zzauto]) {
        if (zznextpos < lastpos) {
            *(zznextpos++) = zzchar;
        } else {
            zzbufovf = 1;
        }
        *zznextpos = '\0';
        /* make sure doesn't get stuck */
        zzadvance();
    } else {
        *zznextpos = '\0';
    }
#endif
#ifdef ZZCOL
    zzendcol -= zzcharfull;
#endif
    zzendexpr = zznextpos - 1;
    zzadd_erase = 0;
    (*actions[accepts[state]])();
    switch (zzadd_erase) {
        case 1:
            goto skip;

        case 2:
            goto more;
    }
}

void zzadvance() {
    if (zzstream_in) {
        ZZGETC_STREAM;
        zzcharfull = 1;
        ZZINC;
    }
    if (zzfunc_in) {
        ZZGETC_FUNC;
        zzcharfull = 1;
        ZZINC;
    }
    if (zzstr_in) {
        ZZGETC_STR;
        zzcharfull = 1;
        ZZINC;
    }
    if (!(zzstream_in || zzfunc_in || zzstr_in)) {
        zzerr_in();
    }
}

void
#ifdef __USE_PROTOS
zzerrstd(const char *s)
#else
    zzerrstd(s) char *s;

#endif
{
    fprintf(stderr,
            "%s near line %d (text was '%s')\n",
            ((s == NULL) ? "Lexical error" : s),
            zzline, zzlextext);
}

int zzerr_in() {
    fprintf(stderr, "No input stream, function, or string\n");
    /* return eof to get out gracefully */
    return EOF;
}

#endif
