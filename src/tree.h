/* Copyright (c) 2007 by Ian Piumarta
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the 'Software'),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, provided that the above copyright notice(s) and this
 * permission notice appear in all copies of the Software.  Acknowledgement
 * of the use of this Software in supporting documentation would be
 * appreciated but is not required.
 * 
 * THE SOFTWARE IS PROVIDED 'AS IS'.  USE ENTIRELY AT YOUR OWN RISK.
 * 
 * Last edited: 2012-05-15 22:37:53 by piumarta on emilia
 */

#include <stdio.h>

typedef struct POSITION { int line;  int column;    } Position;

enum { Unknown= 0, Rule, Variable, Name, Dot, Character, String, Class, Action, Predicate, Error, Alternate, Sequence, PeekFor, PeekNot, Query, Star, Plus };

enum {
  RuleUsed	= 1<<0,
  RuleReached	= 1<<1,
};

typedef union Node Node;

struct Rule	 { int type;  Node *next;   char *name;	 Node *variables;  Node *expression;  int id;  int flags;	};
struct Variable	 { int type;  Node *next;   char *name;  Node *value;  int offset;					};
struct Name	 { int type;  Node *next;   Node *rule;  Node *variable;						};
struct Dot	 { int type;  Node *next;										};
struct Character { int type;  Node *next;   char *value;								};
struct String	 { int type;  Node *next;   char *value;								};
struct Class	 { int type;  Node *next;   unsigned char *value;							};
struct Action	 { int type;  Node *next;   char *text;	  Node *list;  char *name;  Node *rule;  Position *linenum;				};
struct Predicate { int type;  Node *next;   char *text;									};
struct Error	 { int type;  Node *next;   Node *element;  char *text;  Position* linenum;							};
struct Alternate { int type;  Node *next;   Node *first;  Node *last;							};
struct Sequence	 { int type;  Node *next;   Node *first;  Node *last;							};
struct PeekFor	 { int type;  Node *next;   Node *element;								};
struct PeekNot	 { int type;  Node *next;   Node *element;								};
struct Query	 { int type;  Node *next;   Node *element;								};
struct Star	 { int type;  Node *next;   Node *element;								};
struct Plus	 { int type;  Node *next;   Node *element;								};
struct Any	 { int type;  Node *next;										};

union Node
{
  int			type;
  struct Rule		rule;
  struct Variable	variable;
  struct Name		name;
  struct Dot		dot;
  struct Character	character;
  struct String		string;
  struct Class		cclass;
  struct Action		action;
  struct Predicate	predicate;
  struct Error		error;
  struct Alternate	alternate;
  struct Sequence	sequence;
  struct PeekFor	peekFor;
  struct PeekNot	peekNot;
  struct Query		query;
  struct Star		star;
  struct Plus		plus;
  struct Any		any;
};

extern Node *actions;
extern Node *rules;
extern Node *start;

extern int   ruleCount;

extern FILE *output;
extern char	*fileName;
extern int	 lineNumber;
extern char	*fileNameOut;
extern int	 lineNumberOut;

extern int	 lineFlag;

extern Node *makeRule(char *name);
extern Node *findRule(char *name);
extern Node *beginRule(Node *rule);
extern void  Rule_setExpression(Node *rule, Node *expression);
extern Node *Rule_beToken(Node *rule);
extern Node *makeVariable(char *name);
extern Node *makeName(Node *rule);
extern Node *makeDot(void);
extern Node *makeCharacter(char *text);
extern Node *makeString(char *text);
extern Node *makeClass(char *text);
extern Node *makeAction(char *text, Position *linenum);
extern Node *makePredicate(char *text);
extern Node *makeError(Node *e, char *text, Position *linenum);
extern Node *makeAlternate(Node *e);
extern Node *Alternate_append(Node *e, Node *f);
extern Node *makeSequence(Node *e);
extern Node *Sequence_append(Node *e, Node *f);
extern Node *makePeekFor(Node *e);
extern Node *makePeekNot(Node *e);
extern Node *makeQuery(Node *e);
extern Node *makeStar(Node *e);
extern Node *makePlus(Node *e);
extern Node *push(Node *node);
extern Node *top(void);
extern Node *pop(void);

extern void  Rule_compile_c_header(void);
extern void  Rule_compile_c(Node *node);

extern void  Node_print(Node *node);
extern void  Rule_print(Node *node);

#ifdef __GNUC__
extern int   codePrintf(FILE *stream, const char *format, ...)
    __attribute__ ((format (printf, 2, 3)));
#else
extern int   codePrintf(FILE *stream, const char *format, ...);
#endif

extern void  changeLineNum(Position *n);
extern void  restoreLineNum();
extern Position *newPosition(int line, int column);
extern Position *getPosition(int *lines, int linelast, int pos);

#define LINE_SIZE 128

#define pushLine(yy, pos) do \
{ \
  while (yy->linelen <= yy->linepos) \
    { \
      yy->linelen = yy->linelen ? yy->linelen * 2 : LINE_SIZE; \
      yy->lines= (int *)YY_REALLOC(yy, yy->lines, sizeof(int) * yy->linelen); \
    } \
  yy->lines[yy->linepos++] = pos; \
} while(0)

