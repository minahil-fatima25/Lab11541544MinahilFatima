//Task 2: Merge k Sorted Linked Lists using Binary Heap
#include <iostream>
using namespace std;

//node of the linked list
struct ListNode {
    int data;
    ListNode* next;

    ListNode(int val) {
        data = val;
        next = NULL;
    }
};

//min heap of list nodes compared by their data
class MinHeap {
private:
    ListNode** arr;
    int size;
    int capacity;

    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }

    void swapNodes(int i, int j) {
        ListNode* temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    //move the new node up to its correct place
    void heapifyUp(int i) {
        while (i > 0 && arr[parent(i)]->data > arr[i]->data) {
            swapNodes(i, parent(i));
            i = parent(i);
        }
    }

    //move the root down after extracting the minimum
    void heapifyDown(int i) {
        int smallest = i;
        int l = leftChild(i);
        int r = rightChild(i);

        if (l < size && arr[l]->data < arr[smallest]->data)
            smallest = l;
        if (r < size && arr[r]->data < arr[smallest]->data)
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
        arr = new ListNode*[capacity];
    }

    ~MinHeap() {
        delete[] arr;
    }

    bool isEmpty() {
        return size == 0;
    }

    void insert(ListNode* node) {
        if (size == capacity)
            return;
        arr[size] = node;
        heapifyUp(size);
        size++;
    }

    ListNode* extractMin() {
        if (size == 0)
            return NULL;

        ListNode* minNode = arr[0];
        arr[0] = arr[size - 1];
        size--;
        heapifyDown(0);
        return minNode;
    }
};

//heap never has more than k nodes so time is O(N log k)
ListNode* mergeKLists(ListNode* lists[], int k) {
    MinHeap heap(k);

    //put the first node of every list in the heap
    for (int i = 0; i < k; i++) {
        if (lists[i] != NULL)
            heap.insert(lists[i]);
    }

    //dummy node so we dont have to handle the head separately
    ListNode dummy(0);
    ListNode* tail = &dummy;

    while (!heap.isEmpty()) {
        ListNode* smallest = heap.extractMin();
        tail->next = smallest;
        tail = smallest;

        //next node of the same list takes its place in the heap
        if (smallest->next != NULL)
            heap.insert(smallest->next);
    }

    return dummy.next;
}

//adds a node at the end and returns the head
ListNode* insertAtEnd(ListNode* head, int val) {
    ListNode* newNode = new ListNode(val);
    if (head == NULL)
        return newNode;

    ListNode* temp = head;
    while (temp->next != NULL)
        temp = temp->next;
    temp->next = newNode;
    return head;
}

void printList(ListNode* head) {
    while (head != NULL) {
        cout << head->data;
        if (head->next != NULL)
            cout << " -> ";
        head = head->next;
    }
    cout << endl;
}

//free memory of the whole list
void deleteList(ListNode* head) {
    while (head != NULL) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    int k;
    cout << "Enter number of lists: ";
    cin >> k;

    ListNode** lists = new ListNode*[k];

    for (int i = 0; i < k; i++) {
        lists[i] = NULL;
        int n;
        cout << "Enter size of list " << i + 1 << ": ";
        cin >> n;

        cout << "Enter " << n << " sorted values: ";
        for (int j = 0; j < n; j++) {
            int val;
            cin >> val;
            lists[i] = insertAtEnd(lists[i], val);
        }
    }

    cout << "\nInput Lists:" << endl;
    for (int i = 0; i < k; i++) {
        cout << "List " << i + 1 << ": ";
        printList(lists[i]);
    }

    ListNode* merged = mergeKLists(lists, k);

    cout << "\nMerged List: ";
    printList(merged);

    //all nodes are now in the merged list so deleting it frees everything
    deleteList(merged);
    delete[] lists;

    return 0;
}