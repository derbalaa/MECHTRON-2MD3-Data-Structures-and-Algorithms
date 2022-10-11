/*
Abdulrahman Derbala
400301521
*/
#include <iostream>
#include <math.h>

using namespace std;
class Term
{
    public:
        Term(int c=0.0, int e=0) : coef(c), exp(e) {}
        int coef;
        int exp;
};

typedef Term Elem;				// list element type Term
class DNode {					    // doubly linked list node
    private:
        Elem elem;					// node element value
        DNode* prev;				// previous node in list
        DNode* next;				// next node in list
        friend class DLinkedList;	// allow DLinkedList access
        friend class Polynomial;
};

class DLinkedList {				// doubly linked list
    public:
        DLinkedList();				// constructor
        ~DLinkedList();				// destructor
        bool empty() const;				// is list empty?
        const Elem& front() const;			// get front element
        const Elem& back() const;			// get back element
        void addFront(const Elem& e);		// add to front of list
        void addBack(const Elem& e);		// add to back of list
        void removeFront();				// remove from front
        void removeBack();				// remove from back
        friend class Polynomial;
        // friend ostream &operator<<(ostream &out, Polynomial &a);
    private:					// local type definitions
        DNode* header;				// list sentinels
        DNode* trailer;
    protected:					// local utilities
        void add(DNode* v, const Elem& e);		// insert new node before v
        void remove(DNode* v);			// remove node v
};

DLinkedList::DLinkedList() {			// constructor
    header = new DNode;				// create sentinels
    trailer = new DNode;
    header->next = trailer;			// have them point to each other
    trailer->prev = header;
}

bool DLinkedList::empty() const		// is list empty?
    { return (header->next == trailer); }

const Elem& DLinkedList::front() const	// get front element
    { return header->next->elem; }

const Elem& DLinkedList::back() const		// get back element
    { return trailer->prev->elem; }

DLinkedList::~DLinkedList() {			// destructor
    while (!empty()) removeFront();		// remove all but sentinels
    delete header;				// remove the sentinels
    delete trailer;
}

void DLinkedList::remove(DNode* v) {		// remove node v
    DNode* u = v->prev;				// predecessor
    DNode* w = v->next;				// successor
    u->next = w;				// unlink v from list
    w->prev = u;
    delete v;
}

void DLinkedList::removeFront()		// remove from font
    { remove(header->next); }
  
void DLinkedList::removeBack()		// remove from back
    { remove(trailer->prev); }

void DLinkedList::add(DNode* v, const Elem& e) { // insert new node before v
    DNode* u = new DNode;  
    u->elem = e;		// create a new node for e
    u->next = v;				// link u in between v
    u->prev = v->prev;				// ...and v->prev
    v->prev->next = u;
    v->prev = u;
  }

void DLinkedList::addFront(const Elem& e)	// add to front of list
    { add(header->next, e); }
  
void DLinkedList::addBack(const Elem& e)	// add to back of list
    { add(trailer, e); }

class Polynomial{
    private:
        DLinkedList poly;
        string toString(){
            string a = "";
            DNode* v = new DNode;
            v = poly.header->next;
            while(v != poly.trailer){
                if(v->elem.coef<0){
                    if(v->elem.coef==-1){a+="-";}
                    else{a += to_string(v->elem.coef);}
                }else if(v->elem.coef==1){
                    if(a !=""){a += "+";}
                }else{
                    if(a !=""){a += "+";}
                    a += to_string(v->elem.coef);
                }
                if(v->elem.exp > 1){
                    a += "x^";
                    a += to_string(v->elem.exp);
                }else if(v->elem.exp == 1){a += "x";}
                v = v->next;
            }
            return a;
        };
    public:
        Polynomial();
        ~Polynomial();
        void insertTerm(int c,int e);
        friend istream &operator>>(istream &inp, Polynomial &a);
        friend ostream &operator<<(ostream &out, Polynomial &a);
        Polynomial& operator+(Polynomial &a);
        int eval(int x);
};
Polynomial::Polynomial(){}
Polynomial::~Polynomial(){}
void Polynomial::insertTerm(int c,int e){
    if(c==0){}
    else if(poly.empty()){poly.addBack(Term(c,e));}
    else{
        DNode* v = new DNode;
        v = poly.header->next;
        while(v != poly.trailer){
            if(v->elem.exp<e){
                poly.add(v,Term(c,e));
                v = poly.trailer->prev;
            }else if(v->elem.exp == e){
                v->elem.coef = c;
                v = poly.trailer->prev;
            }else if(v->next==poly.trailer){
                poly.addBack(Term(c,e));
                v = poly.trailer->prev;
            }
            v = v->next;
        }
    }
}
istream &operator>>(istream &inp, Polynomial &a){
    int c, e, i;
    cout << "Please input the total number of terms: ";
    inp >> i;
    for(int j = 0; j<i;j++){
        cout << "Please input coefficient number "<<j+1<< ": ";
        inp >> c;
        cout << "Please input exponent number "<<j+1<< ": ";
        inp >> e;
        a.insertTerm(c,e);
    }
    return inp;
}
ostream &operator<<(ostream &out, Polynomial &a){
    out << a.toString();
    return out;
}
Polynomial& Polynomial::operator+(Polynomial &a){
    Polynomial *p = new Polynomial;
    DNode* v = new DNode;
    v = poly.header->next;
    DNode* u = new DNode;
    u = a.poly.header->next;
    while(v!=poly.trailer&&u!=a.poly.trailer){
        int coef1 = v->elem.coef;int coef2 = u->elem.coef;
        int exp1 = v->elem.exp;int exp2 = u->elem.exp;
        if(exp1==exp2){
            int coef = coef1+coef2;
            p->insertTerm(coef,exp1);
            v = v->next;
            u = u->next;
        }else if(exp1 > exp2){
            p->insertTerm(coef1,exp1);
            v = v->next;
        }else{
            p->insertTerm(coef2,exp2);
            u = u->next;
        }
        
    }
    if(v==poly.trailer&&u!=a.poly.trailer){
        while(u!=a.poly.trailer){
            p->insertTerm(u->elem.coef,u->elem.exp);
            u = u->next;
        }
    }else if(v!=poly.trailer&&u==a.poly.trailer){
        while(v!=poly.trailer){
            p->insertTerm(v->elem.coef,v->elem.exp);
            v = v->next;
        }
    }
    return *p;
}
int Polynomial::eval(int x){
    int ans = 0;
    DNode* v = new DNode;
    v = poly.header->next;
    while(v!=poly.trailer){
        ans += v->elem.coef*(pow(x,v->elem.exp));
        v = v->next;
    }
    return ans;
}
int main( ) {

    
    DLinkedList a;	
    a.addBack(Term(5, 2));
    a.addBack(Term(4, 1));
    a.addBack(Term(6, 0));
    cout << a.front().coef << " x " << a.front().exp << endl;
    a.removeFront();
    
    Polynomial pp;
    cin >> pp;
    cout << pp <<endl;

    pp.insertTerm(3, 4);
    cout << pp << endl;
    cout << pp.eval(4) << endl;
}