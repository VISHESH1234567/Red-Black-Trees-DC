#include <iostream>
using namespace std;

class node{
private:
	int value; node* left,* right,* parent; bool isred;
public:
	node(int n,bool red = true,node* p=nullptr){
		value = n;
		left = nullptr; right = nullptr;
		isred = red;
		parent = p;
	}
	friend class redblack_tree;
};

class redblack_tree{
private:
	node* root;
	node* getuncle(node* child){
		if (child->parent->parent->right == child->parent) return child->parent->parent->left;
		return child->parent->parent->right;
	}
	
	int left_rotate(node* address){
		node* rightchild = address->right;
		node* parents = address->parent;
		address->right = rightchild->left;
		if (address->right!=nullptr){address->right->parent = address;}
		rightchild->left = address;
		address->parent = rightchild;

		rightchild->parent = parents;
		if (parents==nullptr){root = rightchild;}
		else if (parents->left==address){parents->left = rightchild;}
		else if (parents->right==address){parents->right = rightchild;}

		return 0;
	}
	int right_rotate(node* address){
		node* leftchild = address->left;
		node* parents = address->parent;
		address->left = leftchild->right;
		if (leftchild->right!=nullptr){leftchild->right->parent = address;}
		leftchild->right = address;
		address->parent = leftchild;

		if (parents==nullptr){root = leftchild;leftchild->parent = nullptr;}
		else if (parents->left==address){parents->left = leftchild;leftchild->parent = parents;}
		else if (parents->right==address){parents->right = leftchild;leftchild->parent = parents;}

		return 0;
	}
	int correct(node* address){
		if (address==nullptr) return 0;
		node* uncle;
		while (address->parent->isred){
			uncle = getuncle(address);
			if (uncle!=nullptr and uncle->isred){
				uncle->isred = false;
				address->parent->isred = false;
				address->parent->parent->isred = true;
				address = address->parent->parent;
			}
			else{
				if (address->parent==address->parent->parent->left){
					if (address==address->parent->right){
						left_rotate(address->parent);
						address = address->left;
					}
					right_rotate(address->parent->parent);

					address->parent->isred = false; address->parent->right->isred = true;address->parent->left->isred = true;
					break;
				}
				else{
					if (address==address->parent->left){
						right_rotate(address->parent);
						address = address->right;
					}
					left_rotate(address->parent->parent);

					address->parent->isred = false; address->parent->left->isred = true;address->parent->right->isred = true;
					break;
				}
			}
			if (address==root) break;
		}
		root->isred = false;
		return 0;
	}

	int deletefix(node* dbparent,bool rights){
		while (dbparent!=nullptr){
			if (rights){
				if (dbparent->left->isred){
					dbparent->isred = true;
					dbparent->left->isred = false;
					right_rotate(dbparent);
				}
				else{
					if ((dbparent->left->left==nullptr or dbparent->left->left->isred==false) and (dbparent->left->right==nullptr or dbparent->left->right->isred==false)){
						dbparent->left->isred = true;
						if (dbparent!=root and dbparent->parent->left==dbparent){rights = false;}
						else if (dbparent!=root){rights = true;}
						if (dbparent->isred){dbparent->isred = false; return 0;}
						dbparent = dbparent->parent;
					}
					else{
						if (dbparent->left->left==nullptr or dbparent->left->left->isred==false){
							dbparent->left->isred = true; dbparent->left->right->isred = false;
							left_rotate(dbparent->left);
						}
						dbparent->left->isred = dbparent->isred;
						dbparent->isred = false;
						dbparent->left->left->isred = false;
						right_rotate(dbparent);
						dbparent = nullptr;
					}
				}
			}
			else{
				if (dbparent->right->isred){
					dbparent->isred = true;
					dbparent->right->isred = false;
					left_rotate(dbparent);
				}
				else{
					if ((dbparent->right->left==nullptr or dbparent->right->left->isred==false) and (dbparent->right->right==nullptr or dbparent->right->right->isred==false)){
						dbparent->right->isred = true;
						if (dbparent!=root and dbparent->parent->left==dbparent){rights = false;}
						else if (dbparent!=root){rights = true;}
						if (dbparent->isred){dbparent->isred = false; return 0;}
						dbparent = dbparent->parent;
					}
					else{
						if (dbparent->right->right==nullptr or dbparent->right->right->isred==false){
							dbparent->right->isred = true; dbparent->right->left->isred = false;
							right_rotate(dbparent->right);
						}
						dbparent->right->isred = dbparent->isred;
						dbparent->isred = false;
						dbparent->right->right->isred = false;
						left_rotate(dbparent);
						dbparent = nullptr;
					}
				}
			}
		}
		root->isred = false;
		return 0;
	}

	int prerec(node* ptr){
		if (ptr==nullptr) return 0;
		cout<<ptr->value<<' ';
		if (ptr->isred) cout<<"R ";
		prerec(ptr->left);
		prerec(ptr->right);
		return 0;
	}
public:
	redblack_tree(){
		root = nullptr;
	}
	int insert(int n){
		if (root==nullptr){root = new node(n,false,nullptr); return 0;}
		node* temp = root;
		bool flag = true;
		while (flag){
			if (n>temp->value){
				if (temp->right==nullptr){
					temp->right = new node(n, true, temp);
					flag = false;
				}
				temp = temp->right;
			}	
			else if (n<temp->value){
				if (temp->left==nullptr){
					temp->left = new node(n,true,temp);
					flag = false;
				}
				temp = temp->left;
			}
			else return 0;
		}
		correct(temp);
		return 0;
	}
	int search(int n){
		node* temp = root;
		while(temp!=nullptr){
			if (temp->value==n){cout<<1<<'\n';return 1;}
			else if (n>temp->value) temp = temp->right;
			else temp = temp->left;
		}
		cout<<-1<<'\n';
		return -1;
	}

	
	int deletenode(int n){
		node* temp = root;
		bool rights=true;
		while(temp!=nullptr){
			if (n>temp->value){
				temp = temp->right;
				rights = true;
			}
			else if (n<temp->value){
				temp = temp->left;
				rights = false;
			}
			else{
				if (temp->left==nullptr and temp->right==nullptr and temp->isred){
					if (rights){temp->parent->right=nullptr;}
					else {temp->parent->left = nullptr;}
					delete temp; return 0;
				}
				if (temp->left==nullptr and temp->right==nullptr){
					if (rights){temp->parent->right=nullptr;}
					else{temp->parent->left=nullptr;}
					node* p = temp->parent;
					delete temp;
					return deletefix(p,rights);
				}
				else if (temp->left==nullptr){
					temp->value = temp->right->value;
					delete temp->right;
					temp->right = nullptr;
					return 0;
				}
				else if (temp->right==nullptr){
					temp->value = temp->left->value;
					delete temp->left;
					temp->left = nullptr;
					return 0;
				}
				else{
					node* temporary = temp->right;
					while (temporary->left!=nullptr){
						temporary = temporary->left;
					}
					temp->value = temporary->value;
					if (temporary->right!=nullptr){
						temporary->value = temporary->right->value;
						delete temporary->right;
						temporary->right = nullptr;
						return 0;
					}
					if (temporary->isred){
						node* p = temporary->parent;
						delete temporary;
						if (p==temp) p->right = nullptr;
						else p->left = nullptr;
						return 0;
					}
					if (temporary->parent==temp){
						temp->right = nullptr;
						delete temporary;
						deletefix(temp,true);
					}
					else{
						temporary->parent->left = nullptr;
						node* p = temporary->parent;
						delete temporary;
						deletefix(p,false);
					}
					return 0;
				}
			}
		}
		return 0;
	}

	int preorder(){
		prerec(root); cout<<'\n';
		return 0;
	}
};

int main(){
	redblack_tree rbt1;
	rbt1.insert(50);
	rbt1.insert(70);
	rbt1.insert(60);
	rbt1.insert(20);
	rbt1.insert(10);
	rbt1.insert(90);
	rbt1.insert(100);
	rbt1.insert(40);
	rbt1.insert(30);
	rbt1.insert(80);
	rbt1.insert(55);
	rbt1.deletenode(55);
	rbt1.deletenode(40);
	//rbt1.deletenode(30);
	rbt1.preorder();
	//rbt1.deletenode(80);
	//rbt1.preorder();
	rbt1.search(60);
	return 0;
}	
