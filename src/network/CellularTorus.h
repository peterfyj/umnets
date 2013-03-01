#ifndef NETWORK_CELLULARTORUS_H_
#define NETWORK_CELLULARTORUS_H_

#include "driver/types.h"
#include <list>
#include <vector>
#include <iterator>

class CellularTorus {
  
  public:

    friend class Driver;

    typedef std::list<NodePtr> CellList;
    typedef std::vector<std::vector<CellList>> CellListArray;
    typedef CellList::iterator CellIterator;

    class ReceiverIterator
      : public std::iterator<std::input_iterator_tag, NodePtr> {
      
      public:

        bool operator==(const ReceiverIterator& ref);
        bool operator!=(const ReceiverIterator& ref);
        NodePtr& operator*();
        NodePtr& operator->();
        ReceiverIterator& operator++();

      private:

        friend class CellularTorus;

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
        CellIterator now; // Point to NodePtr.
        int index_now;  // Range from 0 to 8.

    };

    static CellularTorus* create(Driver& driver);
    static void announce_options(Driver& driver);

    ~CellularTorus();

    void add_node(Motion& placer, NodePtr&& node);
    CellIterator begin(int x, int y);
    CellIterator end(int x, int y);
    ReceiverIterator receiver_begin(Node* sender);
    ReceiverIterator receiver_end(Node* sender);
    int get_size() const;

  private:

    CellularTorus(Driver& driver, int size);
    Driver& driver;
    int size;
    CellListArray nodes;

};

#endif
