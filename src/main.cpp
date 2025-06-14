#include <iostream>

template <class T>
class LinkedList 
{
  private:
    struct Node 
    {
        T data;
        Node* next;
        Node* prev;
        Node(const T& oData): data(oData), next(nullptr), prev(nullptr) {}
    };

    Node* first;
    Node* last;
    int numberOfNodes;

    void destroy()
    {
      Node* current = first;
      while (current != nullptr)
      {
        Node* next = current->next;
        delete current;
        current = next;
      }
      first = nullptr;
      last = nullptr;
    }

    void copy(const LinkedList<T>& other)
    {
      if (other.first == nullptr)
      {
        first = last = nullptr;
        return;
      }

      first = new Node(other.first->data);
      Node* current = first;
      numberOfNodes = 1;
      // start from node after first
      Node* otherCurrent = other.first->next;
      while(otherCurrent != nullptr)
      {
        current->next = new Node(otherCurrent->data);
        current->next->prev = current;
        otherCurrent = otherCurrent->next;
        current = current->next;
        ++numberOfNodes; 
      }
      last = current;
    }

    static Node* merge(Node* left, Node* right)
    {
      Node dummy(0);
      Node* head = &dummy;

      Node* current = head;
      while (left && right)
      {
        if (left->data < right->data)
        {
          current->next = left;
          left->prev = current;
          current = left;
          left = left->next;
        }
        else
        {
          current->next = right;
          right->prev = current;
          current = right;
          right = right->next;
        }
      }
      
      Node* remaining = (left) ? left : right;
      while (remaining)
      {
        current->next = remaining;
        remaining->prev = current;
        current = remaining;
        remaining = remaining->next;
      }

      Node* sortedHead = head->next;
      if (sortedHead) sortedHead->prev = nullptr;
      return sortedHead;
    }

    static Node* mergeSortPrivate(Node* current)
    {
      if (!current || !current->next) return current;
      
      Node* left = current;
      Node* right = current->next;

      // find the middle of the curret list and split it on left and right 
      while (right && right->next)
      {
        current = current->next;
        right = right->next->next;
      }
      // right starts from current->next
      right = current->next;
      if (current->next) current->next->prev = nullptr;
      current->next = nullptr;
      
      return merge(mergeSortPrivate(left), mergeSortPrivate(right));
    }

    static Node* getMiddle(Node* start, Node* end)
    {
      Node* slow = start;
      Node* fast = start;

      while (fast != end && fast->next != end)
      {
        slow = slow->next;
        fast = fast->next->next; 
      }

      return slow;
    }

    static Node* binarySearchPrivate(Node* start, Node* end, const T& value)
    {
      if (start == end)
      {
        return nullptr;
      }

      Node* mid = getMiddle(start, end);

      if (mid->data == value)
      {
        return mid;
      }
      else if (value < mid->data)
      {
        return binarySearchPrivate(start, mid, value);
      }
      else
      {
        return binarySearchPrivate(mid->next, end, value);
      }
    }

  public:
    class Iterator 
    {
      private:
        Node* current;

      public:
        Iterator(): current(nullptr) {}
        Iterator(Node* node): current(node) {}
        Iterator& operator++()
        {
          if (current)
          {
            current = current->next;
          }
          return *this;
        }  
        Iterator operator++(int)
        {
          Iterator tempIter = *this;
          ++(*this);
          return tempIter;
        }
        Iterator& operator--()
        {
          if (current)
          {
            current = current->prev;
          }
          return *this;
        }
        bool operator!=(const Iterator& other) const { return current != other.current; }
        bool operator==(const Iterator& other) const { return !(*this!=other); }
        T& operator*(){ return current->data; }
    };

    class const_Iterator
    {
      private:
        const Node* current;
      
      public:
        const_Iterator() : current(nullptr) {};
        const_Iterator(const Node* node): current(node) {}
        // Construct const_iterator from non-const iterator
        const_Iterator(const Iterator& it): current(it.current) {}
        const_Iterator& operator++()
        {
          if (current)
          {
            current = current->next;
          }
          return *this;
        }
        const_Iterator operator++(int)
        {
          const_Iterator tempIt = *this;
          ++(*this);
          return tempIt;
        }
        const_Iterator& operator--()
        {
          if (current)
          {
            current = current->prev;
          }
          return *this;
        }
        bool operator!=(const const_Iterator& other) const { return current != other.current; }
        bool operator==(const const_Iterator& other) const { return !(*this!=other); }
        const T& operator*() const { return current->data; };
    };

    LinkedList(): first(nullptr), last(nullptr), numberOfNodes(0) {}
    LinkedList(const LinkedList& other): first(nullptr), last(nullptr), numberOfNodes(0) { copy(other); }
    LinkedList& operator=(const LinkedList& other)
    {
      if (this != &other)
      {
        destroy();
        copy(other);
      }

      return *this;
    }
    ~LinkedList() { destroy(); }

    Iterator begin() { return Iterator(first); }
    Iterator end() { return Iterator(); }
    const_Iterator begin() const { return const_Iterator(first); }
    const_Iterator end() const { return const_Iterator(); }
    const_Iterator cbegin() const { return const_Iterator(first); }
    const_Iterator cend() const { return const_Iterator(); }

    bool isEmpty() const { return numberOfNodes == 0; }
    int numElements() const { return numberOfNodes; }
    void pushBack(const T& value)
    {
        Node* newNode = new Node(value);
        if (last)
        {
          newNode->prev = last;
          last->next = newNode;
        } 
        else 
        {
          first = newNode;
        }
        last = newNode;
        ++numberOfNodes;
    }

    void pushFront(const T& value)
    {
        Node* newNode = new Node(value);
        if (first)
        {
          newNode->next = first;
          first->prev = newNode;
        } 
        else 
        {
          last = newNode;
        }
        first = newNode;
        ++numberOfNodes;
    }

    T& front() { return first->data; }
    const T& front() const { return first->data; }

    void popFont()
    {
      if (!first) return;
      
      Node* toPop = first;
      // make sure that the node after the first has previous null
      if (first->next)
      {
        first->next->prev = nullptr;
      }
      first = first->next;
      if (!first) last = nullptr;
      delete toPop;
      --numberOfNodes;
    }

    T& back() { return last->data; }
    const T& back() const { return last->data; }

    void popBack()
    {
      if (!last)  return;

      Node* toPop = last;
      // make sure that the node before the last has next null
      if (last->prev)
      {
        last->prev->next = nullptr;
      }
      last = last->prev;
      if (!last) first = nullptr;
      delete toPop;
      --numberOfNodes;
    }

    void remove(const T& value)
    {
      Node* current = first;
      while (current)
      {
        if (current->data == value)
        {
          if (current == first)
          {
            // Node is the head
            first = current->next;
            if (first) first->prev = nullptr;
            else last = nullptr;
          }
          else if (current == last)
          {
            // Node is the tail
            last = current->prev;
            if (last) last->next = nullptr;
            else first = nullptr;
          }
          else
          {
            // Node is in the middle
            current->next->prev = current->prev;
            current->prev->next = current->next;
          }
          delete current;
          --numberOfNodes;
          return;
        }
        current = current->next;
      }
    }
    
    void mergeSort()
    {
      first = mergeSortPrivate(first);
      // Recompute tail:
      last = first;
      while (last && last->next)
      {
        last = last->next;
      }
    }

    const_Iterator binarySearch(const T& value)
    {
      Node* found = binarySearchPrivate(first, last, value);
      if (!found)
      {
        return const_Iterator();
      }

      return const_Iterator(found);
    }
};

template<class T>
typename LinkedList<T>::const_Iterator find(const LinkedList<T>& list, const T& value)
{
  typename LinkedList<T>::const_Iterator it = list.cbegin();
  for (; it != list.cend(); ++it)
  {
    if (*it == value)
    {
      return it;
    }
  }

  return list.cend();
}

template<class T>
void print(const LinkedList<T>& list)
{
  std::cout << "contents of list are [";
  typename LinkedList<T>::const_Iterator it = list.begin();
  while (it != list.end())
  {
    if (it == list.begin())
    {
      std::cout << *it;
    }
    else 
    {
      std::cout << "," << *it;
    }
    ++it;
  }
  std::cout << "]\n";
  std::cout << "There are " << list.numElements() << " nodes in the list\n";
}

int main()
{
  LinkedList<int> list;
  
  list.pushBack(1);
  list.pushBack(5);
  list.pushBack(1234);
  list.pushBack(7);
  list.pushBack(4);
  list.pushBack(12344);
  list.pushBack(13);
  
  print(list);
  list.remove(12344);
  std::cout << "Removed element 12344." << std::endl;
  print(list);
  
  int topPopFront = list.front();
  list.popFont();
  std::cout << "Poped front value: " << topPopFront << std::endl;
  int topPopBack = list.back();
  list.popBack();
  std::cout << "Poped back value: " << topPopBack << std::endl;
  print(list);

  std::cout << "Before merge sort.\n";
  print(list);
  list.mergeSort();
  std::cout << "After merge sort.\n";
  print(list);
  std::cout << "Normal search found " << std::boolalpha << *find(list, 4) << std::endl;
  std::cout << "Binary search found " << std::boolalpha << *list.binarySearch(4) << std::endl;
  
  std::cout << "First copy\n";
  LinkedList<int> copylist = list;
  print(copylist);
  topPopFront = copylist.front();
  copylist.popFont();
  std::cout << "Poped front value: " << topPopFront << std::endl;
  topPopBack = copylist.back();
  copylist.popBack();
  std::cout << "Poped back value: " << topPopBack << std::endl;
  print(copylist);

  std::cout << "Second copy\n";
  LinkedList<int> otherCopylist;
  otherCopylist = list;
  topPopFront = otherCopylist.front();
  otherCopylist.popFont();
  std::cout << "Poped front value: " << topPopFront << std::endl;
  topPopBack = otherCopylist.back();
  otherCopylist.popBack();
  std::cout << "Poped back value: " << topPopBack << std::endl;
  print(otherCopylist);

  return 0;
}
