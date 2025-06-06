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
    size_t numberOfNodes;

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
    }

    Node* merge(Node* left, Node* right)
    {
      Node dummy(0);
      Node* head = &dummy;

      Node* current = head;
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

    Node* mergeSortPrivate(Node* current)
    {
      if (current == 0 || current->next == 0) return current;
      
      Node* left = current;
      Node* right = current->next;

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

    Node* getMiddle(Node* start, Node* end)
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

    Node* binarySearchPrivate(Node* start, Node* end, const T& value)
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
        Node* previous;

      public:
        Iterator(): current(nullptr), previous(nullptr) {}
        Iterator(Node* node): current(node) {}
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
          first = new Node(value);
          last = first;
          ++numberOfNodes;
          return;
        }
        if (last == first)
        {
          Node* second = new Node(value);
          first->next = second;
          second->prev = first;
          last = second;
          second = nullptr;
          ++numberOfNodes;
          return;
        }
        Node* newLast = new Node(value);
        last->next = newLast;
        newLast->prev = last;
        last = newLast;
        newLast = nullptr;
        ++numberOfNodes;
    }

    T popFont()
    {
      Node* toPop = first;
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
      Node* toPop = last;
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
      Node* found = binarySearchPrivate(first, last, value);
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
