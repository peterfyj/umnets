#ifndef NETWORK_CELLULARTORUS_H_
#define NETWORK_CELLULARTORUS_H_

#include "driver/types.h"
#include <list>
#include <vector>
#include <iterator>
#include <unordered_map>

class CellularTorus {
  
  public:

    class Iterator : public std::iterator<std::input_iterator_tag, NodePtr> {

      public:

        bool operator==(const Iterator& ref);
        bool operator!=(const Iterator& ref);
        Node& operator*();
        Node* operator->();
        Iterator& operator++();

      private:

        friend class CellularTorus;

        typedef std::unordered_map<int, Node*>::iterator TagIterator;

        Iterator(TagIterator iter);

        TagIterator iter;

        
    };

    class CellIterator : public std::iterator<std::input_iterator_tag, Node> {

      public:

        bool operator==(const CellIterator& ref);
        bool operator!=(const CellIterator& ref);
        Node& operator*();
        Node* operator->();
        CellIterator& operator++();

      private:

        friend class CellularTorus;
        
        typedef std::list<NodePtr> CellList;

        CellIterator(CellList::iterator iter);

        CellList::iterator iter;

    };

    class ReceiverIterator
      : public std::iterator<std::input_iterator_tag, Node> {
      
      public:

        bool operator==(const ReceiverIterator& ref);
        bool operator!=(const ReceiverIterator& ref);
        Node& operator*();
        Node* operator->();
        ReceiverIterator& operator++();

      private:

        friend class CellularTorus;

        typedef std::list<NodePtr> CellList;

        static const int OFFSET_X[9];
        static const int OFFSET_Y[9];

        ReceiverIterator(CellularTorus* net, Node* sender);  // Not valid.
        ReceiverIterator& validate();
        ReceiverIterator& set_first();  // Will validate.
        ReceiverIterator& set_last();  // Will validate.
        CellList& get_first_list();
        CellList& get_last_list();
        CellList& get_current_list();

        CellularTorus* net;
        Node* sender;
        CellList::iterator now;
        int index_now;  // Range from 0 to 8.

    };

    static CellularTorus* create(Driver& driver);
    static void announce_options(Driver& driver);

    ~CellularTorus();

    void add_node(Motion& placer, NodePtr&& node);
    Iterator begin();
    Iterator end();
    CellIterator begin(int x, int y);
    CellIterator end(int x, int y);
    ReceiverIterator receiver_begin(Node& sender);
    ReceiverIterator receiver_end(Node& sender);
    Node& get_node(int tag);
    int get_node_count();
    int get_size() const;

  private:

    typedef std::unordered_map<int, Node*> TagMap;
    typedef std::vector<std::vector<std::list<NodePtr>>> CellListArray;

    CellularTorus(Driver& driver, int size);
    Driver& driver;
    int size;
    CellListArray nodes;
    TagMap tag_map;

};

#endif
