#ifndef _H_bI
#define _H_bI

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <map>
using namespace std;
#include "meta.hpp"

struct Sym {
	string val; Sym* doc;
	Sym(string);
	vector<Sym*> nest; void push(Sym*);
	string dump(int=0); virtual string head(); string pad(int);
	virtual Sym* eval();
	virtual string str();
	virtual Sym* add(Sym*);
	virtual Sym* map(Sym*); virtual int len(); virtual Sym* idx(int);
	virtual Sym* at(Sym*);
};
extern map<string,Sym*> glob;
extern void glob_init();

struct Error: Sym { Error(string); };

struct Str: Sym { Str(string); string head(); Sym*add(Sym*);
	int len(); Sym*idx(int); };
struct Vector: Sym { Vector(); string head(); };

struct Op : Sym { Op(string); string head(); Sym*eval(); };
struct Block: Sym { Block(); string head(); };

extern int yylex();
extern int yylineno;
extern char* yytext;
#define TOC(C,X) { yylval.o = new C(yytext); return X; }
extern int yyparse();
extern void yyerror(string);
#include "ypp.tab.hpp"

#endif // _H_bI
