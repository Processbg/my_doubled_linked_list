#include <iostream>

template<class T>
struct Node 
{
    T data;
    Node* next;
    Node* prev;
    Node(const T& oData): data(oData), next(nullptr), prev(nullptr) {}
};

template <class T>
class LinkedList 
{
  private:
    Node<T>* first;
    Node<T>* last;
    size_t numberOfNodes;

    void destroy()
    {
      Node<T>* current = first;
      while (current != nullptr)
      {
        Node<T>* next = current->next;
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
        return;
      }

      first = new Node<T>(other.first->data);
      Node<T>* current = first;
      numberOfNodes = 1;
      // start from node after first
      Node<T>* otherCurrent = other.first->next;
      while(otherCurrent != nullptr)
      {
        current->next = new Node<T>(otherCurrent->data);
        current->next->prev = current;
        otherCurrent = otherCurrent->next;
        current = current->next;
        ++numberOfNodes; 
      }
    }

    Node<T>* merge(Node<T>* left, Node<T>* right)
    {
      Node<T> dummy(0);
      Node<T>* head = &dummy;

      Node<T>* current = head;
      while ((left != 0) && (right != 0))
      {
        if (left->data < right->data)
        {
          current->next = left;
          current = left;
          left = left->next;
        }
        else
        {
          current->next = right;
          current = right;
          right = right->next;
        }
      }
      current->next = (left == 0) ? right : left;
      return head->next;
    }

    Node<T>* mergeSortPrivate(Node<T>* current)
    {
      if (current == 0 || current->next == 0) return current;
      
      Node<T>* left = current;
      Node<T>* right = current->next;

      // find the middle of the curret list and split it on left and right 
      while ((right != 0) && (right->next != 0))
      {
        current = current->next;
        right = right->next->next;
      }
      // right starts from current->next
      right = current->next;
      current->next = 0;
      return merge(mergeSortPrivate(left), mergeSortPrivate(right));
    }

    Node<T>* getMiddle(Node<T>* start, Node<T>* end)
    {
      Node<T>* slow = start;
      Node<T>* fast = start;

      while (fast != end && fast->next != end)
      {
        slow = slow->next;
        fast = fast->next->next; 
      }

      return slow;
    }

    Node<T>* binarySearchPrivate(Node<T>* start, Node<T>* end, const T& value)
    {
      if (start == end)
      {
        return nullptr;
      }

      Node<T>* mid = getMiddle(start, end);

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
        Node<T>* current;
        Node<T>* previous;

      public:
        Iterator(): current(nullptr), previous(nullptr) {}
        Iterator(Node<T>* node): current(node) {}
        Iterator& operator++()
        {
          if (current != nullptr)
          {
            previous = current;
            current = current->next;
          }
          return *this;
        }  
        Iterator operator++(int)
        {
          Iterator tempIter = *this;
          ++*this;
          return tempIter;
        }
        bool operator!=(const Iterator& other){ return current != other.current; }
        bool operator==(const Iterator& other){ return !(*this!=other); }
        T operator*(){ return current->data; }
    };

    LinkedList(): first(nullptr), last(nullptr), numberOfNodes(0) {}
    LinkedList(const LinkedList& other){ copy(other); }
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

    Iterator begin() const { return Iterator(first); }
    Iterator end() const { return Iterator(); }
    bool isEmpty() const { return numberOfNodes == 0; }
    size_t numElements() const { return numberOfNodes; }
    void insertBack(const T& value)
    {
        if (!first)
        {
          first = new Node<T>(value);
          last = first;
          ++numberOfNodes;
          return;
        }
        if (last == first)
        {
          Node<T> *second = new Node<T>(value);
          first->next = second;
          second->prev = first;
          last = second;
          second = nullptr;
          ++numberOfNodes;
          return;
        }
        Node<T> *newLast = new Node<T>(value);
        last->next = newLast;
        newLast->prev = last;
        last = newLast;
        newLast = nullptr;
        ++numberOfNodes;
    }

    T popFont()
    {
      Node<T>* toPop = first;
      T toPopData = toPop->data;
      // make sure that the node after the first has previous null
      if (first->next != nullptr)
      {
        first->next->prev = nullptr;
      }
      first = first->next;
      delete toPop;
      --numberOfNodes;
      return toPopData;
    }

    T popBack()
    {
      Node<T>* toPop = last;
      T toPopData = toPop->data;
      // make sure that the node before the last has next null
      if (last->prev != nullptr)
      {
        last->prev->next = nullptr;
      }
      last = last->prev;
      delete toPop;
      --numberOfNodes;
      return toPopData;
    }
    
    void mergeSort() { first = mergeSortPrivate(first); }
    bool binarySearch(const T& value)
    {
      Node<T>* found = binarySearchPrivate(first, last, value);
      if (!found)
      {
        return false;
      }

      return found->data == value;
    }
};

template<class T>
void print(const LinkedList<T>& list)
{
  std::cout << "contents of list are [";
  typename LinkedList<T>::Iterator it = list.begin();
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
  
  list.insertBack(1);
  list.insertBack(5);
  list.insertBack(1234);
  list.insertBack(7);
  list.insertBack(4);
  list.insertBack(12344);
  list.insertBack(13);
  
  std::cout << "Poped front value: " << list.popFont() << std::endl;
  std::cout << "Poped back value: " << list.popBack() << std::endl;

  std::cout << "Before merge sort.\n";
  print(list);
  list.mergeSort();
  std::cout << "After merge sort.\n";
  print(list);
  std::cout << "Binary search found 4 " << std::boolalpha << list.binarySearch(4) << std::endl;
  std::cout << "First copy\n";
  LinkedList<int> copylist = list;
  print(copylist);
  std::cout << "Second copy\n";
  LinkedList<int> otherCopylist;
  otherCopylist = list;
  print(otherCopylist);

  return 0;
}
