//Task 1: Huffman Coding using Binary Heap
#include <iostream>
#include <string>
using namespace std;

//node of the huffman tree
struct Node {
    char ch;
    int freq;
    Node* left;
    Node* right;

    Node(char c, int f) {
        ch = c;
        freq = f;
        left = NULL;
        right = NULL;
    }
};

//min heap that keeps the node with lowest frequency at the top
class MinHeap {
private:
    Node** arr;
    int size;
    int capacity;

    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }

    void swapNodes(int i, int j) {
        Node* temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    //keep moving the node up while its parent has bigger frequency
    void heapifyUp(int i) {
        while (i > 0 && arr[parent(i)]->freq > arr[i]->freq) {
            swapNodes(i, parent(i));
            i = parent(i);
        }
    }

    //push the node down until both children are bigger than it
    void heapifyDown(int i) {
        int smallest = i;
        int l = leftChild(i);
        int r = rightChild(i);

        if (l < size && arr[l]->freq < arr[smallest]->freq)
            smallest = l;
        if (r < size && arr[r]->freq < arr[smallest]->freq)
            smallest = r;

        if (smallest != i) {
            swapNodes(i, smallest);
            heapifyDown(smallest);
        }
    }

public:
    MinHeap(int cap) {
        capacity = cap;
        size = 0;
        arr = new Node*[capacity];
    }

    ~MinHeap() {
        delete[] arr;
    }

    int getSize() {
        return size;
    }

    //add at the end then fix the heap upwards
    void insert(Node* n) {
        if (size == capacity) {
            cout << "Heap is full" << endl;
            return;
        }
        arr[size] = n;
        heapifyUp(size);
        size++;
    }

    //take the root out and put the last node in its place
    Node* extractMin() {
        if (size == 0)
            return NULL;

        Node* minNode = arr[0];
        arr[0] = arr[size - 1];
        size--;
        heapifyDown(0);
        return minNode;
    }
};

//builds the tree by joining the two smallest nodes again and again
Node* buildHuffmanTree(string S, int f[], int n) {
    MinHeap heap(n);

    for (int i = 0; i < n; i++)
        heap.insert(new Node(S[i], f[i]));

    while (heap.getSize() > 1) {
        Node* left = heap.extractMin();
        Node* right = heap.extractMin();

        //internal node has no character so '$' is used
        Node* parent = new Node('$', left->freq + right->freq);
        parent->left = left;
        parent->right = right;

        heap.insert(parent);
    }

    return heap.extractMin();
}

//preorder traversal, left edge adds 0 and right edge adds 1
void printCodes(Node* root, string code) {
    if (root == NULL)
        return;

    //only leaf nodes have actual characters
    if (root->left == NULL && root->right == NULL) {
        cout << root->ch << " : " << code << endl;
        return;
    }

    printCodes(root->left, code + "0");
    printCodes(root->right, code + "1");
}

//free all the nodes to avoid memory leak
void deleteTree(Node* root) {
    if (root == NULL)
        return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    int n;
    cout << "Enter number of characters: ";
    cin >> n;

    string S;
    int* f = new int[n];

    cout << "Enter the characters (without spaces): ";
    cin >> S;

    cout << "Enter their frequencies: ";
    for (int i = 0; i < n; i++)
        cin >> f[i];

    Node* root = buildHuffmanTree(S, f, n);

    cout << "\nHuffman Codes (Preorder):" << endl;

    //if there is only one character it gets code 0
    if (root->left == NULL && root->right == NULL) {
        cout << root->ch << " : 0" << endl;
    }
    else {
        printCodes(root, "");
    }

    deleteTree(root);
    delete[] f;

    return 0;
}