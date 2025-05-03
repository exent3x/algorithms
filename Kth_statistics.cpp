#include <iostream>
#include <algorithm>
#include <exception>
using namespace std;

class AvlTree {
    struct Node {
        Node* left_child;
        Node* right_child;
        int value;
        int left_size;
        int height_left;
        int height_right;
        Node(int _val) : left_child(nullptr), right_child(nullptr), 
                         value(_val), left_size(0), height_left(0), height_right(0) {}
    };
    Node* root;

    int left_size(Node* node) { return node ? node->left_size : 0; }

    void updateHeights(Node* node) {
        node->height_left = (node->left_child ? max(node->left_child->height_left, node->left_child->height_right) + 1 : 0);
        node->height_right = (node->right_child ? max(node->right_child->height_left, node->right_child->height_right) + 1 : 0);
    }

    int balanceFactor(Node* node) { return node->height_left - node->height_right; }

    Node* leftRotate(Node* node) {
        Node* right = node->right_child;
        Node* rl = right->left_child;
        right->left_child = node;
        node->right_child = rl;

        right->left_size += node->left_size + 1;
        updateHeights(node);
        updateHeights(right);
        return right;
    }

    Node* rightRotate(Node* node) {
        Node* left = node->left_child;
        Node* lr = left->right_child;
        left->right_child = node;
        node->left_child = lr;

        node->left_size = (lr ? lr->left_size + 1 : 0);
        updateHeights(node);
        updateHeights(left);
        return left;
    }

    Node* balance(Node* node) {
        updateHeights(node);
        int bf = balanceFactor(node);
        if (bf > 1) {
            if (balanceFactor(node->left_child) >= 0) return rightRotate(node);
            else {
                node->left_child = leftRotate(node->left_child);
                return rightRotate(node);
            }
        }
        if (bf < -1) {
            if (balanceFactor(node->right_child) <= 0) return leftRotate(node);
            else {
                node->right_child = rightRotate(node->right_child);
                return leftRotate(node);
            }
        }
        return node;
    }

    Node* insert(Node* node, int value) {
        if (!node) return new Node(value);
        if (value < node->value) {
            node->left_child = insert(node->left_child, value);
            node->left_size++;
        } else if (value > node->value) {
            node->right_child = insert(node->right_child, value);
        }
        return balance(node);
    }

    int findKth_(Node* node, int k) {
        if(node==nullptr) throw std::out_of_range("k is invalid");
        int currentLeftSize = node->left_size;
        if (k < currentLeftSize) return findKth_(node->left_child, k);
        else if (k == currentLeftSize) return node->value;
        else return findKth_(node->right_child, k - currentLeftSize - 1);
    }

public:
    AvlTree() : root(nullptr) {}
    void insert(int value) { root = insert(root, value); }
    int findKth(int k) { return findKth_(root, k); }
};

int main() {
    AvlTree tree;
    tree.insert(3);
    tree.insert(1);
    tree.insert(4);
    tree.insert(2);
    tree.insert(5);

    cout << "k=0: " << tree.findKth(0) << endl;
    cout << "k=1: " << tree.findKth(1) << endl;
    cout << "k=2: " << tree.findKth(2) << endl;
    cout << "k=3: " << tree.findKth(3) << endl;
    cout << "k=15: " << tree.findKth(15) << endl; 
}