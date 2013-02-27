#ifndef NETWORK_CELLULARTORUS_H_
#define NETWORK_CELLULARTORUS_H_

#include "driver/types.h"
#include <list>
#include <vector>

class CellularTorus {
  
  public:

    typedef std::list<NodePtr> CellList;
    typedef std::vector<std::vector<CellList>> CellListArray;
    typedef CellList::iterator CellIterator;

    class ReceiverIterator {
      
      public:

        bool operator==(const ReceiverIterator& ref);
        bool operator!=(const ReceiverIterator& ref);
        ReceiverIterator& operator++();

      private:

        friend class CellularTorus;

        static const int OFFSET_X[9];
        static const int OFFSET_Y[9];

        ReceiverIterator(CellularTorus& net, int cx, int cy);
        ReceiverIterator(CellularTorus& net);
        IntPos last_cell();
        IntPos current_cell();

        CellularTorus& net;
        CellularTorus::CellIterator now;
        IntPos cell_center;
        int index_now;  // Range from 0 to 8.

    };

    static CellularTorus* create(Driver& driver);
    static void announce_options(Driver& driver);

    ~CellularTorus();

    void add_node(Motion& placer, NodePtr&& node);
    CellIterator begin(int x, int y);
    CellIterator end(int x, int y);
    ReceiverIterator receiver_begin(int x, int y);
    ReceiverIterator receiver_end(int x, int y);
    int get_size() const;

  private:

    CellularTorus(Driver& driver, int size);
    Driver& driver;
    int size;
    CellListArray nodes;

};

namespace std {

  template<>
  struct iterator_traits<CellularTorus::ReceiverIterator> {
    
    typedef input_iterator_tag iterator_category;
    typedef CellularTorus::ReceiverIterator value_type;
    typedef ptrdiff_t difference_type;
    typedef CellularTorus::ReceiverIterator& reference;
    typedef CellularTorus::ReceiverIterator* pointer;

  };

};

#endif
