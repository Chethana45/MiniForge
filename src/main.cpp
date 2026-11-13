
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <variant>
#include <unordered_map>
#include <functional>
#include <chrono>
#include <cmath>
#include <cstdlib>
using namespace std;

struct Token {
 enum Type { LP,RP,LB,RB,LSB,RSB,COMMA,MINUS,PLUS,SEMI,SLASH,STAR,PERCENT,BANG,NE,EQ,EE,GT,GE,LT,LE,
 ID,STR,NUM,LET,FN,IF,ELSE,WHILE,TRUE,FALSE,NIL,RETURN,PRINT,AND,OR,END } type;
 string text; int line;
};
class Lexer {
 string s; size_t p=0,start=0; int line=1; vector<Token> out;
 char peek(int n=0){return p+n<s.size()?s[p+n]:'\0';}
 char take(){return s[p++];}
 void add(Token::Type t){out.push_back({t,s.substr(start,p-start),line});}
 void identifier(){while(isalnum((unsigned char)peek())||peek()=='_')take();string w=s.substr(start,p-start);
  static unordered_map<string,Token::Type> k={{"let",Token::LET},{"fn",Token::FN},{"if",Token::IF},{"else",Token::ELSE},{"while",Token::WHILE},{"true",Token::TRUE},{"false",Token::FALSE},{"null",Token::NIL},{"return",Token::RETURN},{"print",Token::PRINT},{"and",Token::AND},{"or",Token::OR}};
  add(k.count(w)?k[w]:Token::ID);
 }
 void number(){while(isdigit((unsigned char)peek()))take();if(peek()=='.'&&isdigit((unsigned char)peek(1))){take();while(isdigit((unsigned char)peek()))take();}add(Token::NUM);}
 void stringLit(){while(peek()&&peek()!='"'){if(peek()=='\n')line++;take();}if(!peek())throw runtime_error("Lexer: unterminated string");take();add(Token::STR);}
public:
 explicit Lexer(string x):s(move(x)){}
 vector<Token> scan(){while(p<s.size()){start=p;char c=take();switch(c){
 case '(':add(Token::LP);break;case ')':add(Token::RP);break;case '{':add(Token::LB);break;case '}':add(Token::RB);break;
 case '[':add(Token::LSB);break;case ']':add(Token::RSB);break;case ',':add(Token::COMMA);break;case '-':add(Token::MINUS);break;case '+':add(Token::PLUS);break;case ';':add(Token::SEMI);break;case '*':add(Token::STAR);break;case '%':add(Token::PERCENT);break;
 case '!':add(peek()=='='?(take(),Token::NE):Token::BANG);break;case '=':add(peek()=='='?(take(),Token::EE):Token::EQ);break;
 case '>':add(peek()=='='?(take(),Token::GE):Token::GT);break;case '<':add(peek()=='='?(take(),Token::LE):Token::LT);break;
 case '/':if(peek()=='/'){while(peek()&&peek()!='\n')take();}else add(Token::SLASH);break;
 case ' ':case '\r':case '\t':break;case '\n':line++;break;case '"':stringLit();break;
 default:if(isdigit((unsigned char)c))number();else if(isalpha((unsigned char)c)||c=='_')identifier();else throw runtime_error("Lexer line "+to_string(line)+": unexpected character");
 }}out.push_back({Token::END,"",line});return out;}
};

struct Expr {virtual ~Expr()=default;}; using E=shared_ptr<Expr>;
struct Lit:Expr{variant<monostate,double,bool,string> v;explicit Lit(decltype(v)x):v(move(x)){}};
struct Var:Expr{Token n;explicit Var(Token x):n(move(x)){}};
struct Set:Expr{Token n;E v;Set(Token a,E b):n(move(a)),v(move(b)){}};
struct Unary:Expr{Token op;E r;Unary(Token a,E b):op(move(a)),r(move(b)){}};
struct Binary:Expr{E l,r;Token op;Binary(E a,Token b,E c):l(move(a)),r(move(c)),op(move(b)){}};
struct Logical:Expr{E l,r;Token op;Logical(E a,Token b,E c):l(move(a)),r(move(c)),op(move(b)){}};
struct Call:Expr{E callee;vector<E> args;Call(E c,vector<E>a):callee(move(c)),args(move(a)){}};
struct Arr:Expr{vector<E> items;explicit Arr(vector<E>x):items(move(x)){}};
struct Index:Expr{E a,i;Index(E x,E y):a(move(x)),i(move(y)){}};
struct Stmt{virtual ~Stmt()=default;};using S=shared_ptr<Stmt>;
struct ExprS:Stmt{E e;explicit ExprS(E x):e(move(x)){}};
struct Print:Stmt{E e;explicit Print(E x):e(move(x)){}};
struct Decl:Stmt{Token n;E init;Decl(Token a,E b):n(move(a)),init(move(b)){}};
struct Block:Stmt{vector<S> s;explicit Block(vector<S>x):s(move(x)){}};
struct If:Stmt{E c;S a,b;If(E x,S y,S z):c(move(x)),a(move(y)),b(move(z)){}};
struct While:Stmt{E c;S b;While(E x,S y):c(move(x)),b(move(y)){}};
struct Fn:Stmt{Token n;vector<Token> params;vector<S> body;Fn(Token x,vector<Token>p,vector<S>b):n(move(x)),params(move(p)),body(move(b)){}};
struct Ret:Stmt{E e;explicit Ret(E x):e(move(x)){}};

class Parser{
 vector<Token> t;size_t p=0;
 Token peek(){return t[p];}Token prev(){return t[p-1];}bool check(Token::Type x){return peek().type==x;}bool end(){return check(Token::END);}
 bool match(initializer_list<Token::Type> xs){for(auto x:xs)if(check(x)){p++;return true;}return false;}
 Token need(Token::Type x,string m){if(check(x))return t[p++];throw runtime_error("Parser line "+to_string(peek().line)+": "+m);}
 void semi(){need(Token::SEMI,"expected ';'");}
 vector<S> blockBody(){vector<S> v;while(!check(Token::RB)&&!end())v.push_back(decl());need(Token::RB,"expected '}'");return v;}
 S function(){Token n=need(Token::ID,"expected function name");need(Token::LP,"expected '('");vector<Token> ps;if(!check(Token::RP)){do{ps.push_back(need(Token::ID,"expected parameter"));}while(match({Token::COMMA}));}need(Token::RP,"expected ')'");need(Token::LB,"expected '{'");return make_shared<Fn>(n,ps,blockBody());}
 S decl(){if(match({Token::FN}))return function();if(match({Token::LET})){Token n=need(Token::ID,"expected variable");E e=nullptr;if(match({Token::EQ}))e=expr();semi();return make_shared<Decl>(n,e);}return stmt();}
 S stmt(){if(match({Token::PRINT})){E e=expr();semi();return make_shared<Print>(e);}if(match({Token::RETURN})){E e=nullptr;if(!check(Token::SEMI))e=expr();semi();return make_shared<Ret>(e);}
 if(match({Token::IF})){need(Token::LP,"expected '('");E c=expr();need(Token::RP,"expected ')'");S a=stmt(),b=nullptr;if(match({Token::ELSE}))b=stmt();return make_shared<If>(c,a,b);}
 if(match({Token::WHILE})){need(Token::LP,"expected '('");E c=expr();need(Token::RP,"expected ')'");return make_shared<While>(c,stmt());}
 if(match({Token::LB}))return make_shared<Block>(blockBody());E e=expr();semi();return make_shared<ExprS>(e);}
 E expr(){return assign();}E assign(){E e=logicOr();if(match({Token::EQ})){Token n=prev();E v=assign();if(auto x=dynamic_pointer_cast<Var>(e))return make_shared<Set>(x->n,v);throw runtime_error("Parser: invalid assignment");}return e;}
 E logicOr(){E e=logicAnd();while(match({Token::OR})){Token o=prev();e=make_shared<Logical>(e,o,logicAnd());}return e;}
 E logicAnd(){E e=equality();while(match({Token::AND})){Token o=prev();e=make_shared<Logical>(e,o,equality());}return e;}
 E equality(){E e=compare();while(match({Token::EE,Token::NE})){Token o=prev();e=make_shared<Binary>(e,o,compare());}return e;}
 E compare(){E e=term();while(match({Token::GT,Token::GE,Token::LT,Token::LE})){Token o=prev();e=make_shared<Binary>(e,o,term());}return e;}
 E term(){E e=factor();while(match({Token::PLUS,Token::MINUS})){Token o=prev();e=make_shared<Binary>(e,o,factor());}return e;}
 E factor(){E e=unary();while(match({Token::STAR,Token::SLASH,Token::PERCENT})){Token o=prev();e=make_shared<Binary>(e,o,unary());}return e;}
 E unary(){if(match({Token::BANG,Token::MINUS}))return make_shared<Unary>(prev(),unary());return call();}
 E call(){E e=primary();for(;;){if(match({Token::LP})){vector<E>a;if(!check(Token::RP)){do{a.push_back(expr());}while(match({Token::COMMA}));}need(Token::RP,"expected ')'");e=make_shared<Call>(e,a);}else if(match({Token::LSB})){E ix=expr();need(Token::RSB,"expected ']'");e=make_shared<Index>(e,ix);}else break;}return e;}
 E primary(){if(match({Token::TRUE}))return make_shared<Lit>(true);if(match({Token::FALSE}))return make_shared<Lit>(false);if(match({Token::NIL}))return make_shared<Lit>(monostate{});
 if(match({Token::NUM}))return make_shared<Lit>(stod(prev().text));if(match({Token::STR}))return make_shared<Lit>(prev().text.substr(1,prev().text.size()-2));if(match({Token::ID}))return make_shared<Var>(prev());
 if(match({Token::LP})){E e=expr();need(Token::RP,"expected ')'");return e;}if(match({Token::LSB})){vector<E>a;if(!check(Token::RSB)){do{a.push_back(expr());}while(match({Token::COMMA}));}need(Token::RSB,"expected ']'");return make_shared<Arr>(a);}throw runtime_error("Parser: expected expression");}
public:explicit Parser(vector<Token>x):t(move(x)){}vector<S> parse(){vector<S>v;while(!end())v.push_back(decl());return v;}
};

struct Value;struct Env;struct Func;using A=shared_ptr<vector<Value>>;
struct Value{using D=variant<monostate,double,bool,string,A,shared_ptr<Func>>;D d;Value():d(monostate{}){}template<class T>Value(T x):d(move(x)){}};
string show(const Value&v){if(holds_alternative<monostate>(v.d))return "null";if(auto x=get_if<double>(&v.d)){ostringstream o;o<<*x;return o.str();}if(auto x=get_if<bool>(&v.d))return *x?"true":"false";if(auto x=get_if<string>(&v.d))return *x;if(auto x=get_if<A>(&v.d)){string s="[";for(size_t i=0;i<(*x)->size();i++){if(i)s+=", ";s+=show((**x)[i]);}return s+"]";}return "<fn>";}
bool truth(const Value&v){if(holds_alternative<monostate>(v.d))return false;if(auto b=get_if<bool>(&v.d))return *b;return true;}
struct Env{unordered_map<string,Value> v;shared_ptr<Env> parent;explicit Env(shared_ptr<Env>p=nullptr):parent(move(p)){}Value get(Token n){if(v.count(n.text))return v[n.text];if(parent)return parent->get(n);throw runtime_error("Runtime: undefined variable '"+n.text+"'");}void set(Token n,Value x){if(v.count(n.text)){v[n.text]=x;return;}if(parent){parent->set(n,x);return;}throw runtime_error("Runtime: undefined variable '"+n.text+"'");}};
struct Func{vector<Token> params;vector<S> body;shared_ptr<Env> closure;Func(vector<Token>p,vector<S>b,shared_ptr<Env>e):params(move(p)),body(move(b)),closure(move(e)){}};
struct Returned{Value v;};

class Interpreter{
 shared_ptr<Env> env=make_shared<Env>();bool trace=false;
 Value eval(E e){
 if(auto x=dynamic_pointer_cast<Lit>(e)){if(holds_alternative<monostate>(x->v))return {};if(auto n=get_if<double>(&x->v))return *n;if(auto b=get_if<bool>(&x->v))return *b;return get<string>(x->v);}
 if(auto x=dynamic_pointer_cast<Var>(e))return env->get(x->n);
 if(auto x=dynamic_pointer_cast<Set>(e)){Value v=eval(x->v);env->set(x->n,v);return v;}
 if(auto x=dynamic_pointer_cast<Arr>(e)){auto a=make_shared<vector<Value>>();for(auto q:x->items)a->push_back(eval(q));return a;}
 if(auto x=dynamic_pointer_cast<Index>(e)){Value a=eval(x->a),i=eval(x->i);if(!holds_alternative<A>(a.d)||!holds_alternative<double>(i.d))throw runtime_error("Runtime: array index requires array and number");auto ar=get<A>(a.d);int n=(int)get<double>(i.d);if(n<0)n+=(int)ar->size();if(n<0||n>=(int)ar->size())throw runtime_error("Runtime: index out of range");return (*ar)[n];}
 if(auto x=dynamic_pointer_cast<Unary>(e)){Value r=eval(x->r);if(x->op.type==Token::BANG)return !truth(r);if(auto n=get_if<double>(&r.d))return -*n;throw runtime_error("Runtime: unary '-' expects number");}
 if(auto x=dynamic_pointer_cast<Logical>(e)){Value l=eval(x->l);if(x->op.type==Token::OR&&truth(l))return l;if(x->op.type==Token::AND&&!truth(l))return l;return eval(x->r);}
 if(auto x=dynamic_pointer_cast<Binary>(e)){Value l=eval(x->l),r=eval(x->r);auto a=get_if<double>(&l.d),b=get_if<double>(&r.d);
 switch(x->op.type){case Token::PLUS:if(a&&b)return *a+*b;if(holds_alternative<string>(l.d)||holds_alternative<string>(r.d))return show(l)+show(r);throw runtime_error("Runtime: '+' requires numbers or strings");
 case Token::MINUS:case Token::STAR:case Token::SLASH:case Token::PERCENT:if(!a||!b)throw runtime_error("Runtime: arithmetic requires numbers");if(x->op.type==Token::MINUS)return *a-*b;if(x->op.type==Token::STAR)return *a**b;if(*b==0)throw runtime_error("Runtime: division by zero");if(x->op.type==Token::SLASH)return *a / *b;return fmod(*a,*b);
 case Token::GT:if(!a||!b)throw runtime_error("Runtime: comparison requires numbers");return *a>*b;case Token::GE:if(!a||!b)throw runtime_error("Runtime: comparison requires numbers");return *a>=*b;case Token::LT:if(!a||!b)throw runtime_error("Runtime: comparison requires numbers");return *a<*b;case Token::LE:if(!a||!b)throw runtime_error("Runtime: comparison requires numbers");return *a<=*b;case Token::EE:return show(l)==show(r);case Token::NE:return show(l)!=show(r);default:break;}}
 if(auto x=dynamic_pointer_cast<Call>(e)){Value c=eval(x->callee);vector<Value>a;for(auto q:x->args)a.push_back(eval(q));if(auto n=get_if<string>(&c.d)){if(*n=="@len"){if(a.size()!=1)throw runtime_error("len expects one argument");if(auto s=get_if<string>(&a[0].d))return (double)s->size();if(auto ar=get_if<A>(&a[0].d))return (double)(*ar)->size();throw runtime_error("len expects string or array");}
 if(*n=="@push"){if(a.size()!=2||!holds_alternative<A>(a[0].d))throw runtime_error("push expects array and value");auto ar=get<A>(a[0].d);ar->push_back(a[1]);return (double)ar->size();}
 if(*n=="@type"){if(a.size()!=1)throw runtime_error("type expects one argument");auto&d=a[0].d;return string(holds_alternative<monostate>(d)?"null":holds_alternative<double>(d)?"number":holds_alternative<bool>(d)?"boolean":holds_alternative<string>(d)?"string":holds_alternative<A>(d)?"array":"function");}}
 if(auto f=get_if<shared_ptr<Func>>(&c.d)){auto fn=*f;if(a.size()!=fn->params.size())throw runtime_error("Function argument count mismatch");auto old=env;env=make_shared<Env>(fn->closure);for(size_t i=0;i<a.size();i++)env->v[fn->params[i].text]=a[i];try{for(auto s:fn->body)exec(s);}catch(Returned&r){env=old;return r.v;}env=old;return {};}throw runtime_error("Runtime: value is not callable");}
 throw runtime_error("Runtime: unsupported expression");
 }
 void block(vector<S> ss,shared_ptr<Env> e){auto old=env;env=e;try{for(auto s:ss)exec(s);}catch(...){env=old;throw;}env=old;}
 void exec(S s){if(trace)cerr<<"[trace] executing statement\n";if(auto x=dynamic_pointer_cast<ExprS>(s)){eval(x->e);return;}if(auto x=dynamic_pointer_cast<Print>(s)){cout<<show(eval(x->e))<<"\n";return;}
 if(auto x=dynamic_pointer_cast<Decl>(s)){env->v[x->n.text]=x->init?eval(x->init):Value();return;}if(auto x=dynamic_pointer_cast<Block>(s)){block(x->s,make_shared<Env>(env));return;}
 if(auto x=dynamic_pointer_cast<If>(s)){if(truth(eval(x->c)))exec(x->a);else if(x->b)exec(x->b);return;}if(auto x=dynamic_pointer_cast<While>(s)){int n=0;while(truth(eval(x->c))){if(++n>1000000)throw runtime_error("Loop safety limit exceeded");exec(x->b);}return;}
 if(auto x=dynamic_pointer_cast<Fn>(s)){env->v[x->n.text]=make_shared<Func>(x->params,x->body,env);return;}if(auto x=dynamic_pointer_cast<Ret>(s))throw Returned{x->e?eval(x->e):Value()};}
public:Interpreter(){env->v["len"]=string("@len");env->v["push"]=string("@push");env->v["type"]=string("@type");env->v["clock"]=string("@clock");}
 void run(vector<S> ss,bool tr=false){trace=tr;for(auto s:ss)exec(s);}
 void source(const string&s,bool tr=false){run(Parser(Lexer(s).scan()).parse(),tr);}
};
int main(int argc,char**argv){bool tok=false,tr=false;string file;for(int i=1;i<argc;i++){string a=argv[i];if(a=="--tokens")tok=true;else if(a=="--trace")tr=true;else if(a=="--help"){cout<<"MiniForge usage: miniforge [--tokens] [--trace] [file.mf]\n";return 0;}else file=a;}
 Interpreter in;if(!file.empty()){ifstream f(file);if(!f){cerr<<"Cannot open "<<file<<"\n";return 2;}stringstream b;b<<f.rdbuf();try{if(tok){for(auto t:Lexer(b.str()).scan())cout<<t.line<<" "<<t.type<<" "<<t.text<<"\n";}else in.source(b.str(),tr);}catch(exception&e){cerr<<e.what()<<"\n";return 1;}return 0;}
 cout<<"MiniForge 0.1.0 REPL (:quit to exit)\n";string line,buf;while(cout<<"> "&&getline(cin,line)){if(line==":quit")break;buf+=line+"\n";try{in.source(buf,tr);buf.clear();}catch(exception&e){cerr<<e.what()<<"\n";buf.clear();}}}
