#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <fstream>
using namespace std::literals::chrono_literals;

class node{
private:
	std::string value; node* left,* right,* parent; bool isred; int index;
public:
	node(std::string name, int indexval, bool red = true,node* p=nullptr){
		value = name;
		left = nullptr; right = nullptr;
		isred = red;
		parent = p;
		index = indexval;
	}
	int get_index(){
		return index;
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
		std::cout<<ptr->value<<' ';
		if (ptr->isred) std::cout<<"R ";
		prerec(ptr->left);
		prerec(ptr->right);
		return 0;
	}
public:
	redblack_tree(){
		root = nullptr;
	}
	int insert(std::string name,int indexval = 0){
		if (root==nullptr){root = new node(name, indexval, false,nullptr); return 0;}
		node* temp = root;
		bool flag = true;
		while (flag){
			if (name>temp->value){
				if (temp->right==nullptr){
					temp->right = new node(name, indexval, true, temp);
					flag = false;
				}
				temp = temp->right;
			}	
			else if (name<temp->value){
				if (temp->left==nullptr){
					temp->left = new node(name,indexval,true,temp);
					flag = false;
				}
				temp = temp->left;
			}
			else return temp->get_index();
		}
		correct(temp);
		return indexval;
	}
	node* search(std::string name){
		node* temp = root;
		while(temp!=nullptr){
			if (temp->value==name){return temp;}
			else if (name>temp->value) temp = temp->right;
			else temp = temp->left;
		}
		return nullptr;
	}

	int deletenode(std::string name){
		node* temp = root;
		bool rights=true;
		while(temp!=nullptr){
			if (name>temp->value){
				temp = temp->right;
				rights = true;
			}
			else if (name<temp->value){
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
					temp->index = temp->right->index;
					delete temp->right;
					temp->right = nullptr;
					return 0;
				}
				else if (temp->right==nullptr){
					temp->value = temp->left->value;
					temp->index = temp->left->index;
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
					temp->index = temporary->index;
					if (temporary->right!=nullptr){
						temporary->value = temporary->right->value;
						temporary->index = temporary->right->index;
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
		prerec(root); std::cout<<'\n';
		return 0;
	}
};

int main(){
	redblack_tree rbt1;
	auto start = std::chrono::high_resolution_clock::now();
	int totalvertices = 1965206;
	std::vector<int>* adjacency_list = new std::vector<int>[totalvertices];
	std::string* vertices = new std::string[totalvertices];
	std::ifstream new_file("California road networks.txt");
	int indexval = 0;
	std::string edge1,edge2;
	int index1,index2;
	if (new_file.is_open()){
		std::getline(new_file, edge1,'\n');
		std::getline(new_file, edge1,'\n');
		std::getline(new_file, edge1,'\n');
		std::getline(new_file, edge1,'\n');
		while (!new_file.eof()){
			std::getline(new_file,edge1,'	');
			std::getline(new_file,edge2,'\n');
			
			index1 = rbt1.insert(edge1,indexval);
			if (index1==indexval) indexval++;
			vertices[index1].swap(edge1);
			index2 = rbt1.insert(edge2,indexval);
			if (index2==indexval) indexval++;
			vertices[index2].swap(edge2);

			adjacency_list[index1].push_back(index2);
		}
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> duration = end-start;
	std::cout<<duration.count()*1000<<"ms"<<'\n';
	//rbt1.preorder();
	std::cout<<indexval<<'\n';
	for (int i=0;i<20;i++){
		std::cout<<vertices[i]<<":";
		for (int j=0;j<adjacency_list[i].size();j++){
			std::cout<<adjacency_list[i][j]<<' ';
		}
		std::cout<<'\n';
	}
	return 0;
}	
