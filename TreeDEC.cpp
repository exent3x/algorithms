#include <bits/stdc++.h>
using namespace std; // Тоже для удобства я знаю что так не надо

template<typename T>
class DecTree {
private:
    struct Node {
        Node* left;
        Node* right;
        int priority;
        T key;
        Node(T _key) : key(_key), left(nullptr), right(nullptr), priority(rand()) {}
    };

    typedef pair<Node*, Node*> Pair; // для удобства
    enum SplitCondition { LESS, LESS_EQ };

    Node* root = nullptr;
    int size = 0;

    Node* merge(Node* left, Node* right) {

        /*
        *Сравниваем приоритеты корней дерева
        * Если l>r то соединяем лево справа и право и цепляем к лево справа
        * Иначе соединияем лево и право слева и цепляем слева к праву
        */

        if (!left) return right;
        if (!right) return left;
        if (left->priority > right->priority) {
            left->right = merge(left->right, right);
            return left;
        } else {
            right->left = merge(left, right->left);
            return right;
        }
    }

    Pair split(Node* node, T key, SplitCondition cond) {
        /*
        * Получаем условие на вход и проверяем ключ на его выполнения
        * Продолжаем рекурсивно резать дерево по полученному условию
        */
        if (!node) return {nullptr, nullptr};
        bool compare_result;
        if (cond == LESS) {
            compare_result = (node->key < key);
        } else {
            compare_result = (node->key <= key);
        }
        if (compare_result) {
            Pair p = split(node->right, key, cond);
            node->right = p.first;
            return {node, p.second};
        } else {
            Pair p = split(node->left, key, cond);
            node->left = p.second;
            return {p.first, node};
        }
    }

public:
    void insert(T key) {
        /*
        *делим на < <= затем добавляем новый элемент в < а затем сливаем вместе
        */
        Pair p = split(root, key, LESS);
        Node* newNode = new Node(key);
        root = merge(merge(p.first, newNode), p.second);
        size++;
    }

    void remove_first(T key) {

        /*
        * Разделяем на 3 части < = >, затем начинаем удалять и склеивать
        * Если есть равные удаляем 1  и вызываем слияние, если нет не трогаем дерево а просто сливаем части вместе
        */


        Pair less = split(root, key, LESS);
        Pair less_eq = split(less.second, key, LESS_EQ);
        if (less_eq.first) {
            Node* toRemove = less_eq.first;
            Node* mergedChildren = merge(toRemove->left, toRemove->right);
            delete toRemove;
            root = merge(merge(less.first, mergedChildren), less_eq.second);
            size--;
        } else {
            root = merge(less.first, merge(less_eq.first, less_eq.second));
        }
    }

    int get_size() const {
        return size;
    }

    void DFS(Node* node) {

        /*
        * Обход inorder для вывода дерева в отсортированном порядке
        */
        if (!node) return;
        DFS(node->left);
        cout << node->key << " ";
        DFS(node->right);
    }

    void BFS(Node* node) {
        // обход в ширину просто чтоб был
        if (!node) return;
        deque<Node*> q;
        q.push_back(node);
        while (!q.empty()) {
            Node* current = q.front();
            q.pop_front();
            cout << current->key << " ";
            if (current->left) q.push_back(current->left);
            if (current->right) q.push_back(current->right);
        }
    }

    Node* getRoot() { return root; } // достать корень дерева
};
int main() {
    srand(time(nullptr));
    DecTree<int> tree;

    // Генерация 100 случайных чисел от 1 до 50
    cout << "Generating 100 random elements (1-50)...\n";
    vector<int> elements;
    for(int i = 0; i < 100; ++i) {
        elements.push_back(1 + rand() % 50);
    }

    // Вставка элементов
    for(int val : elements) {
        tree.insert(val);
    }

    cout << "\nAfter insertion:\n";
    cout << "Size: " << tree.get_size() << "\n";
    cout << "DFS (first 50 elements): ";
    tree.DFS(tree.getRoot());
    cout << "\n\n";

    // Удаление 5 случайных элементов (первых вхождений)
    cout << "Removing first occurrences of 5 random elements...\n";
    for(int i = 0; i < 5; ++i) {
        int target = elements[rand() % elements.size()];
        cout << "Removing first " << target << endl;
        tree.remove_first(target);
        
        cout << "New size: " << tree.get_size() << endl;
        cout << "DFS snippet: ";
        tree.DFS(tree.getRoot());
        cout << "\n\n";
    }

    cout << "Trying to remove non-existing value (1000):\n";
    tree.remove_first(1000);
    cout << "Size remains: " << tree.get_size() << endl;

    return 0;
}
