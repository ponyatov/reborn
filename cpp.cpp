#include "hpp.hpp"
#define YYERR "\n\n"<<yylineno<<":"<<msg<<"["<<yytext<<"]\n\n"
void yyerror(string msg) { cout<<YYERR; cerr<<YYERR; exit(-1); }
int main() { glob_init(); return yyparse(); }

Sym::Sym(string V) { val=V; doc=NULL; }
void Sym::push(Sym*o) { nest.push_back(o); }

string Sym::head() { return "<"+val+">"; }
string Sym::pad(int n) { string S; for (int i=0;i<n;i++) S+='\t'; return S; }
string Sym::dump(int depth) { string S ="\n"+pad(depth)+head();
	if (doc) S += '\t'+doc->str();
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		S += (*it)->dump(depth+1);
	return S; }

Sym* Sym::eval() {
	Sym* G = glob[val]; if (G) return G;
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		(*it) = (*it)->eval();
	return this; }

string Sym::str() { return val; }
Sym* Sym::add(Sym*o) { return new Error(head()+"+"+o->head()); }

int Sym::len()	{ return 1; }
Sym* Sym::idx(int i) {
	if (i) return new Error("idx " + head());
	else return this; }
Sym* Sym::map(Sym*o) { Sym* L = new Vector();
	for (int i=0;i<o->len();i++)
		L->push(at(o->idx(i)));
	return L; }

Sym* Sym::at(Sym*o) { push(o); return this; }

Error::Error(string V):Sym("error") { yyerror(V); }

Str::Str(string V):Sym(V) {}
string Str::head() { string S = "'";
	for (int i=0;i<val.length();i++)
		switch (val[i]) {
			case '\n': S+="\\n"; break;
			case '\t': S+="\\t"; break;
			default: S+=val[i];
		}
	return S+"'"; }
Sym* Str::add(Sym*o) { return new Str(val+o->str()); }
int Str::len() { return val.length(); }
Sym* Str::idx(int i) {
	if (i>len()) return new Error("idx "+head()); else {
		ostringstream os; os << val[i];
		return new Str(os.str()); }}

Op::Op(string V):Sym(V) {}
string Op::head() { return "("+val+")"; }
Sym* Op::eval() {
	if (val=="~") return nest[0]; else Sym::eval();
	if (val=="+") return nest[0]->add(nest[1]);
	if (val=="|") return nest[0]->map(nest[1]);
	return this; }

Vector::Vector():Sym("[]") {}
string Vector::head() { return val; }

Block::Block():Sym("{}") {}
string Block::head() { return val; }

map<string,Sym*> glob;
void glob_init() {
	glob["MODULE"] = new Str(MODULE);
	glob["ABOUT"] = new Str(ABOUT);
	glob["AUTHOR"] = new Str(AUTHOR);
	glob["LICENSE"] = new Str(LICENSE);
	glob["GITHUB"] = new Str(GITHUB);
	glob["sp"] = new Str(" ");
	glob["nl"] = new Str("\n");
	glob["tab"] = new Str("\t");
}
