//Abdulrahman Derbala 400301521
#include <list>
#include <iostream> 
using namespace std;

class RuntimeException {  // generic run-time Exception
    private:
        string errorMsg;  // error message
    public:
        RuntimeException(const string& err)  //constructor
        {
            errorMsg = err; 
        }
        string getMessage() const {   //access error message
            return errorMsg; 
        }
};

class NonExistentElement : public RuntimeException {  // No such element Exception for BST
    public:
        NonExistentElement(const string& err) : RuntimeException(err) {}
};

typedef int KT;	
class KeyValues{
    public:
        KT& getKey()                                        //define this to return the key
            {return key;}
        list<KT> getValues()                               // define this to return values
            {return values;}
        void setKey(KT k)                                  // define this to set the key
            {key = k;}
        void setValues(list<KT> vals_lst)                  //define this to set a given list vals_lst to values
            {values = vals_lst;}
        void addValue(KT v)                                // define this to push back v to values.
            {values.push_back(v);}
    private:
        KT key;
        list<KT> values;
};

struct Node {					                            // a node of the tree
    KeyValues    elt;					                    // element value
    Node*   par;					                        // parent
    Node*   left;					                        // left child
    Node*   right;					                        // right child
    Node() : elt(), par(NULL), left(NULL), right(NULL) { }  // constructor
};

class Position {	 				                        // position in the tree
    private:
        Node* v;						                    // pointer to the node
    public:
        Position(Node* _v = NULL) : v(_v) { }		        // constructor
        KT& operator*()					                    // get element
            { return v->elt.getKey(); }
        Position left() const				                // get left child
            { return Position(v->left); }
        Position right() const				                // get right child
            { return Position(v->right); }
        Position parent() const				                // get parent
            { return Position(v->par); }
        bool isRoot() const				                    // root of the tree?
            { return v->par == NULL; }
        bool isNull() const				                    // root of the tree?
            { return v == NULL; }
        bool isExternal() const				                // an external node?
            { return v->left == NULL && v->right == NULL; }
        bool operator==(const Position& p) const	        // are equal?
            { return v == p.v; }
        bool operator!=(const Position& p) const			// are not equal?
            { return v != p.v; }
        KT& getElem()	const				                // get element
            { return v->elt.getKey(); }
        Node* getNode()     const
            { return v;}
    friend class BinaryTree;			                    // give tree access
};

typedef list<Position> PositionList;                        // list of positions

class BinaryTree {
    public:
        BinaryTree();					                    // constructor
        int size() const;					                // number of nodes
        bool empty() const;					                // is tree empty?
        Position root() const;				                // get the root
        PositionList positions() const;  			        // list of nodes
        void addRoot();					                    // add root to empty tree
        void expandExternal(const Position& p);		        // expand external node
        Position removeAboveExternal(const Position& p);	// remove p and parent
        // housekeeping functions omitted...
    protected: 						                        // local utilities
        void preorder(const Position& v, PositionList& pl) const;	// preorder utility
    private:
        Node* _root;					                    // pointer to the root
        int n;						                        // number of nodes
};

BinaryTree::BinaryTree()			                        // constructor
    : _root(NULL), n(0) { }

int BinaryTree::size() const			                    // number of nodes
    { return n; }

bool BinaryTree::empty() const			                    // is tree empty?
    { return size() == 0; }

Position BinaryTree::root() const                           // get the root
    { return Position(_root); }

void BinaryTree::addRoot()			                        // add root to empty tree
    { _root = new Node; n = 1; }

void BinaryTree::expandExternal(const Position& p) {        // expand external node
    Node* v = p.v;					                        // p's node
    v->left = new Node;					                    // add a new left child
    v->left->par = v;					                    // v is its parent
    v->right = new Node;				                    // and a new right child
    v->right->par = v;					                    // v is its parent
    n += 2;						                            // two more nodes
}

PositionList BinaryTree::positions() const {                // list of all nodes
    PositionList pl;
    preorder(root(), pl);					                // preorder traversal
    return PositionList(pl);				                // return resulting list
}

void BinaryTree::preorder(const Position& v, PositionList& pl) const {  // preorder traversal
    pl.push_back(Position(v));				                // add this node
    if (!v.left().isExternal())					            // traverse left subtree
        preorder(v.left(), pl);
    if (!v.right().isExternal())					        // traverse right subtree
        preorder(v.right(), pl);
}

Position BinaryTree::removeAboveExternal(const Position& p) {   // this is needed for removal
    Node* w = p.v;  Node* v = w->par;			                // get p's node and parent
    Node* sib = (w == v->left ?  v->right : v->left);
    if (v == _root) {					                        // child of root?
      _root = sib;					                            // ...make sibling root
      sib->par = NULL;
    }
    else {
      Node* gpar = v->par;				                        // w's grandparent
      if (v == gpar->left) gpar->left = sib; 		            // replace parent by sib
      else gpar->right = sib;
      sib->par = gpar;
    }
    delete w; delete v;					                        // delete removed nodes
    n -= 2;						                                // two fewer nodes
    return Position(sib);
}

class SearchTree {					                            // a binary search tree
    public: 						                            // public types
        class Iterator {	                      		        // an iterator/position
            private:
                Position v;						                // which entry
            public:
                Iterator(const Position& vv) : v(vv) { }		// constructor
                KT& operator*() { return *v; }		            // get entry
                bool operator==(const Iterator& p) const		// are iterators equal?
                    { return v == p.v; }
                bool operator!=(const Iterator& p) const		// are iterators not equal?
                    { return v != p.v; }
                Iterator& operator++();				            // inorder successor
                friend class SearchTree;				        // give search tree access
                Iterator nextInPostorder();
                Iterator nextInLevelorder();
        };
    public:						                                // public functions
        SearchTree();					                        // constructor
        int size() const; 					                    // number of entries
        bool empty() const;					                    // is the tree empty?
        SearchTree::Iterator find(const KT& k);     			// find entry with key k
        SearchTree::Iterator insert(const KT& k, const KT x);		// insert (k,x)
        void erase(const KT& k);// throw(NonexistentElement);	// remove key k entry
        void erase( SearchTree::Iterator& p);			        // remove entry at p
        SearchTree::Iterator begin();					        // iterator to first entry
        SearchTree::Iterator end();					            // iterator to end entry
        PositionList getPositionList();                         // ge the positions of the binary tree
        SearchTree::Iterator min();
        SearchTree::Iterator max();
        int rank(KT key);
        SearchTree::Iterator floor(KT key);
        SearchTree::Iterator ceil(KT key);
        void eraseMin();
        void eraseMax();
        SearchTree::Iterator selectAtRank(int i);
        int countKeysBetween(KT l,KT h);
        list<SearchTree::Iterator> entriesBetween(KT l,KT h);
        SearchTree::Iterator median();
        int height();
        int depth(const SearchTree::Iterator& p);

    protected:						                            // local utilities
        Position root() const;					                // get virtual root
        Position finder(const KT& k, const Position& v);		// find utility
        Position inserter(const KT& k, const KT x);		// insert utility
        Position eraser(Position& w);				            // erase utility
        int inorderCounting(Position node);   // utility for counting the desc. of a node
        int rank(Position node, KT key);     // utility for rank
        int height(Position w);

    private: 						                            // member data
        BinaryTree T;					                        // the binary tree
        int n;						                            // number of entries
};

SearchTree::Iterator SearchTree::min() {
    return begin();
}	
SearchTree::Iterator SearchTree::max() {
    SearchTree::Iterator max(begin());
    for(SearchTree::Iterator w = begin();w!=end();++w){
        if(w.v.getElem()>max.v.getElem())
            { max=w;}
    }
    return max;
}	
int SearchTree::inorderCounting(Position p) {
    int lCounter,rCounter;
    if (p.isExternal())
        return 0;
    lCounter = inorderCounting(p.left());
    rCounter = inorderCounting(p.right());
    return 1+lCounter+rCounter;
}
int SearchTree::rank(KT key) {
    return rank(root(), key);
}

int SearchTree::rank(Position p, KT key)
{
    if (find(key)==end())
        { throw NonExistentElement("this key is not in the tree");}
    if (p.isExternal()) {
        return 0;
    }
    if (p.getElem() > key) {
        return rank(p.left(), key);
    }
    else if (p.getElem() < key){
        return 1 + inorderCounting(p.left()) + rank(p.right(), key);
    }
    else{
        return inorderCounting(p.left());
    }
}

SearchTree::Iterator SearchTree::floor(KT key) {                 // largest key on tree <= key
    if (T.empty())
        { throw NonExistentElement("floor() is called on an empty tree");}
    if (min().v.getElem()>key)  // happens when the key is less than the minimum key stored on BST
        { throw NonExistentElement("the key is too small");}
    SearchTree::Iterator temp = begin();
    while(*temp<=key){
        if(*temp==key)
            return SearchTree::Iterator(temp);
        SearchTree::Iterator temp2=temp;
        ++temp;
        if(*temp>key)
            return SearchTree::Iterator(temp2);
    }
}

SearchTree::Iterator SearchTree::ceil(KT key) {                 // largest key on tree <= key
    if (T.empty())
        { throw NonExistentElement("floor() is called on an empty tree");}
    if (max().v.getElem()<key)  // happens when the key is less than the minimum key stored on BST
        { throw NonExistentElement("the key is too big");}
    SearchTree::Iterator temp = begin();
    while(*temp<key)
        ++temp;
    return SearchTree::Iterator(temp);
}

void SearchTree::eraseMin() {
    erase(*min());
}
void SearchTree::eraseMax() {
    erase(*max());
}

SearchTree::Iterator SearchTree::selectAtRank(int i){
    SearchTree::Iterator temp = begin();
    while(rank(*temp)<i)
        ++temp;
    return temp;
}

int SearchTree::countKeysBetween(KT l, KT h) {
    int num_keys = 0;
    SearchTree::Iterator temp = begin();
    while(*temp < l)
        {++temp;}
    while(*temp<=h){
        ++temp;++num_keys;
    }
    return num_keys;
}

list<SearchTree::Iterator> SearchTree::entriesBetween(KT l,KT h){
    list<SearchTree::Iterator> ans;
    SearchTree::Iterator temp = begin();
    while(*temp < l)
        {++temp;}
    while(*temp<=h){
        ans.push_back(temp);
        ++temp;
    }
    return ans;
}

SearchTree::Iterator SearchTree::Iterator::nextInPostorder(){
    Position w = v.parent();
    if(w.isRoot()){ throw NonExistentElement("The iterator is the root of the tree");}
    if(w.right()==v)
        {return SearchTree::Iterator(w);}
    else if(w.right().getElem()==0)
        {return SearchTree::Iterator(w);}
    else{
        w = w.right();
        while(w.right().getElem()!=0){
            Position temp = w;
            while(w.left().getElem()!=0)
                { w = w.left();}
            if(temp == w){}
            else{w = w.parent();}
            w = w.right();
        }
        return w;
    }
}

SearchTree::Iterator SearchTree::Iterator::nextInLevelorder(){
    Position s1 = v;
    Position root1 = v;
    while(!(root1.isRoot())){root1 = root1.parent();}
    root1 = root1.left();

    if (root1==s1){
        if (root1.left().getElem()!=0){
            return Iterator(root1.left());
        } else if (root1.right().getElem()!=0){
            return Iterator(root1.right());
        } else {
            throw NonExistentElement("This iterartor is only thing in the tree");
        }
    }
    PositionList q;
    q.push_back(root1);
    while (!q.empty()){
        Position nd = q.front();
        q.pop_front();
        if (nd.left().getElem()!=0){
            q.push_back(nd.left());
        }
        if (nd.right().getElem()!=0)
            q.push_back(nd.right());
        if (nd==s1)
            return Iterator(q.front());
    }
}
SearchTree::Iterator SearchTree::median(){
    SearchTree::Iterator L=begin();
    for(L;L!=end();++L){
        if(rank(*L)==n/2)
            return L;
    }
}

int SearchTree::height()
    {return height(root())-1;}//-1 if root not counted

int SearchTree::height(Position w){
    if(w.isExternal())
        return 0;
    else{
        int rightHeight = height(w.right());
        int leftHeight = height(w.left());
        if(rightHeight>leftHeight)
            {return 1 + rightHeight;}
        else
            {return 1 + leftHeight;}
    }
}

int SearchTree::depth(const SearchTree::Iterator& p){
    int ans = 0;Position w = p.v;
    //if starting count from p do w.parent().isRoot() else remove parent
    while(!(w.parent().isRoot())){
        w = w.parent();
        ans++;
    }
    return ans;
}

SearchTree::SearchTree() : T(), n(0)                            // constructor
    { T.addRoot(); T.expandExternal(T.root()); }	            // create the super root
  
Position SearchTree::root() const                               // get virtual root
    { return T.root().left(); }				                    // left child of super root

int SearchTree::size() const                                    // get the size
    { return n; }

bool SearchTree::empty() const			                        // is search tree empty?
    { return size() == 0; }

SearchTree::Iterator SearchTree::begin() {                      // iterator to first entry
    Position v = root();					                    // start at virtual root
    while (!v.isExternal()) v = v.left();		                // find leftmost node
    return Iterator(v.parent());
} 		
SearchTree::Iterator SearchTree::end()                          // iterator to end entry
    { return Iterator(T.root()); }			                    // return the super root

PositionList SearchTree::getPositionList(){                     // ge the positions of the binary tree
    return T.positions();
}
				
SearchTree::Iterator& SearchTree::Iterator::operator++() {      // inorder successor
    Position w = v.right();
    if (!w.isExternal()) {				                        // have right subtree?
      do { v = w; w = w.left(); }			                    // move down left chain
      while (!w.isExternal());
    }
    else {
      w = v.parent();					                        // get parent
      while (v == w.right())				                    // move up right chain
        { v = w; w = w.parent(); }
      v = w;						                            // and first link to left
    }
    return *this;
}
	
Position SearchTree::finder(const KT& k, const Position& v) { // find utility
    if (v.isExternal()) return v;			                    // key not found
    if (k < v.getElem()) return finder(k, v.left());	        // search left subtree
    else if (v.getElem() < k) return finder(k, v.right());	    // search right subtree
    else return v;					                            // found it here
}
			
SearchTree::Iterator SearchTree::find(const KT& k) {          // find entry with key k
    Position v = finder(k, root());				                // search from virtual root
    if (!v.isExternal()) return Iterator(v);		            // found it
    else return end();					                        // didn't find it
}
					
Position SearchTree::inserter(const KT& k, const KT x) {  // insert utility
    if(find(k)==end()){
        Position v = finder(k, root());				            // search from virtual root
        while (!v.isExternal())				                    // key already exists?
        v = finder(k, v.right());				                // look further
        T.expandExternal(v);				                    // add new internal node
        v.getNode()->elt.setKey(k);                             // set the key!
        v.getNode()->elt.addValue(x);
        n++;						                            // one more entry
        return v;						                        // return insert position
    }
    else{
        find(k).v.getNode()->elt.addValue(x);
        return find(k).v;
    }
}

SearchTree::Iterator SearchTree::insert(const KT& k, const KT x)   // insert (k,x)
    { Position v = inserter(k, x); return Iterator(v); }

Position SearchTree::eraser(Position& w) {                          // remove utility
    Position z;
    if (w.left().isExternal()) z = w.left();		                // remove from left case 1
    else if (w.right().isExternal()) z = w.right();	                // remove from right case 1
    else {						                                    // both internal? case 2
      z = w.right();					                            // go to right subtree
      do { z = z.left(); } while (!z.isExternal());	                // get leftmost node
      Position v = z.parent();
      w.getNode()->elt.setKey(v.getElem()); w.getNode()->elt.setValues(v.getNode()->elt.getValues());	            // copy z's parent to w
    //   *w = v.getElem();	                                            // copy z's parent to v
    }
    n--;						                                    // one less entry
    return T.removeAboveExternal(z);			                    // remove z and its parent
}
					
void SearchTree::erase(const KT& k){ //throw(NonexistentElement) { // remove key k entry
    Position w = finder(k, root());				                    // search from virtual root
    if (w.isExternal())					                        // not found?
        throw NonExistentElement("Erase of nonexistent element");
    eraser(w);						                                // remove it
}
	
void SearchTree::erase( Iterator& p)                                // erase entry at p
    { eraser(p.v); }


int main()
{
    cout << "sorry of u needed the orignal main function i changed a lot so i deleted all of it!!!"<<endl;
    cout<<"Thank for a great semster, have a great summer and I hope to see you as my professor again."<<endl;
    cout<<"ILY <3"<<endl;
    int a, b, size = 15;
   
    /* FOR THE APEX OF HEART */
    for (a = size/2; a <= size; a = a+2)
    {
        // FOR SPACE BEFORE PEAK-1 : PART 1
        for (b = 1; b < size-a; b = b+2)        
            cout<<" ";
   
        // FOR PRINTING PEAK-1 : PART 2
        for (b = 1; b <= a; b++)
            cout<<"A";
    
        // FOR SPACE B/W PEAK-1 AND PEAK-2 : PART 3
        for (b = 1; b <= size-a; b++)
            cout<<" ";
           
        // FOR PRINTING PEAK-2 : PART 4
        for (b = 1; b <= a-1; b++)
            cout<<"A";
   
        cout<<endl;
    }
   
    /*FOR THE BASE OF HEART ie. THE INVERTED TRIANGLE */
   
    for (a = size; a >= 0; a--)
    {
        // FOR SPACE BEFORE THE INVERTED TRIANGLE : PART 5 
        for (b = a; b < size; b++)
            cout<<" ";
   
        // FOR PRINTING THE BASE OF TRIANGLE : PART 6
        for (b = 1; b <= ((a * 2) - 1); b++)
            cout<<"B";
   
        cout<<endl;  
    }
    return 0;
}