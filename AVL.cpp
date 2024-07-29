#include <cmath>
#include <iostream>
using namespace std;

struct AVL {
	int value;
	AVL* left;
	AVL* right;
	int balance;
};

AVL* Root = NULL;

int depth(AVL* root, int dep) {
    if (Root == NULL) {
        return 0;
    }
    if (root == NULL) {
        return dep;
    }
    else {
        return max(depth(root->left, dep + 1), depth(root->right, dep + 1));
    }
}

void bal(AVL* root) {
    int depl = 0, depr = 0;
    if (root->left != NULL) {
        depl = depth(root->left, 0);
        bal(root->left);
    }
    if (root->right != NULL) {
        depr = depth(root->right, 0);
        bal(root->right);
    }
    root->balance = depr - depl;
}

AVL* searchfather(AVL* root, int value) {
    if (root == NULL || Root->value == value) {
        return NULL;
    }
    else {
        if (root->left != NULL && root->left->value == value) {
            return root;
        }
        else if (root->value == value) {
            return NULL;
        }
        else if (root->right != NULL && root->right->value == value) {
            return root;
        }
        else if (root->value > value) {
            searchfather(root->left, value);
        }
        else if (root->value < value) {
            searchfather(root->right, value);
        }
    }
}

AVL* checking(AVL* root) {
    AVL* father = searchfather(Root, root->value);
    if (father == NULL) {
        return NULL;
    }
    if (abs(father->balance) <= 1) {
        checking(father);
    }
    else {
        return father;
    }
}

void rebuilding(AVL* root) {
    AVL* opor = checking(root);
    if (opor == NULL) {
        return;
    }
    AVL* father = searchfather(Root, opor->value);
    AVL* lson = opor->left;
    AVL* rson = opor->right;
    if (root->value < opor->value && root->value < lson->value) {
        opor->left = lson->right;
        lson->right = opor;
        if (father!=NULL && father->value > opor->value) {
            father->left = lson;
        }
        else if (father != NULL && father->value < opor->value){
            father->right = lson;
        }
    }
    if (root->value > opor->value && root->value > rson->value) {
        opor->right = rson->left;
        rson->left = opor;
        if (father!=NULL && father->value > opor->value) {
            father->left = rson;
        }
        else if (father != NULL && father->value < opor->value) {
            father->right = rson;
        }
    }
    if (root->value<opor->value && root->value>lson->value) {
        AVL* lrson = lson->right;
        lson->right = lrson->left;
        opor->left = lrson->left;
        if (father!=NULL && father->value > opor->value) {
            father->left = lrson;
        }
        else if (father != NULL && father->value < opor->value) {
            father->right = lrson;
        }
        lrson->left = lson;
        lrson->right = opor;
    }
    if (root->value > opor->value && root->value < rson->value) {
        AVL* rlson = rson->left;
        rson->left = rlson->right;
        opor->right = rlson->left;
        if (father!=NULL && father->value > opor->value) {
            father->left = rlson;
        }
        else if (father != NULL && father->value < opor->value) {
            father->right = rlson;
        }
        rlson->left = opor;
        rlson->right = rson;
    }
}

AVL* search(AVL* root, int value) {
    if (root == NULL) {
        return NULL;
    }
    else if (root->value == value) {
        return root;
    }
    else if (root->value > value) {
        if (root->left == NULL) {
            return NULL;
        }
        else {
            return search(root->left, value);
        }
    }
    else if (root->value < value) {
        if (root->right == NULL) {
            return NULL;
        }
        else {
            return search(root->right, value);
        }
    }
    return NULL;
}

void watch(AVL* root) {
    if (root == NULL) {
        return;
    }
    else {
        bal(root);
        cout << root->value << ' ' << root->balance << endl;
        if (root->left != NULL) {
            watch(root->left);
        }
        if (root->right != NULL) {
            watch(root->right);
        }
    }
    return;
}

AVL* create(int value) {
	AVL* NewAVL = new AVL;
	NewAVL->value = value;
	NewAVL->left = NewAVL->right = NULL;
	return NewAVL;
}

AVL* add(AVL* root, int value) {
    if (root == NULL) {
        root = create(value);
    }
    else if (root->value > value) {
        root->left = add(root->left, value);
    }
    else if (root->value < value) {
        root->right = add(root->right, value);
    }
    bal(root);
    return root;
}

void connection(int value) {
    Root = add(Root, value);
    rebuilding(search(Root, value));
}

AVL* mvalue(AVL* root) {
    if (root == NULL) {
        return NULL;
    }
    if (root->right == NULL) {
        return root;
    }
    else {
        return mvalue(root->right);
    }
}

void del(AVL* root, int value) {
    AVL* father = searchfather(root, value);
    AVL* node = search(root, value);
    if (node->left == NULL && node->right == NULL) {
        if (father->value > value) {
            father->left = NULL;
        }
        else {
            father->right = NULL;
        }
    }
    else if (node->left == NULL && node->right != NULL) {
        if (father->value > value) {
            father->left = node->right;
        }
        else {
            father->right = node->right;
        }
    }
    else if (node->left != NULL && node->right == NULL) {
        if (father->value > value) {
            father->left = node->left;
        }
        else {
            father->right = node->left;
        }
    }
    else if (node->left->right == NULL) {
        if (father->value > value) {
            father->left = node->left;
            node->left->right = node->right;
        }
        else {
            father->right = node->left;
            node->left->right = node->right;
        }
    }
    else if (node->left != NULL && node->right != NULL) {
        AVL* m = mvalue(node->left);
        searchfather(root, m->value)->right = NULL;
        if (father->value > value) {
            father->left = m;
            m->right = node->right;
        }
        else {
            father->right = m;
            m->right = node->right;
        }
    }
    delete node;
    if (father->right != NULL) {
        rebuilding(father->right);
    }
    else {
        rebuilding(father->left);
    }
}

int main()
{
    connection(16);
    connection(22);
    connection(10);
    connection(12);
    connection(13);
    connection(24);
    connection(26);
    connection(14);
    connection(15);
    watch(Root);
    cout << endl << endl;
    del(Root,14);
    del(Root, 13);
    watch(Root);
}

