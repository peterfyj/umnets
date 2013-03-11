#ifndef NETWORK_CELLULARTORUS_H_
#define NETWORK_CELLULARTORUS_H_

/**
 * @file network/CellularTorus.h
 * @brief Header for CellularTorus.
 */

#include "driver/types.h"
#include "util/Math.h"
#include <list>
#include <vector>
#include <iterator>
#include <unordered_map>

/**
 * @brief Cellular network with torus property.
 * @ingroup network
 */
class CellularTorus {
  
  public:

    /**
     * @brief For iterating over all nodes.
     */ 
    class Iterator : public std::iterator<std::input_iterator_tag, Node> {

      public:

        /**
         * @brief Test if they point to the same network node.
         */
        bool operator==(const Iterator& ref);

        /**
         * @brief Test if they point to different network nodes.
         */
        bool operator!=(const Iterator& ref);

        /**
         * @brief Get the reference of network nodes.
         */
        Node& operator*();

        /**
         * @brief Used to reference members of the node pointed to.
         */
        Node* operator->();
        
        /**
         * @brief Increment the iterator.
         */
        Iterator& operator++();

      private:

        friend class CellularTorus;

        typedef std::unordered_map<int, Node*>::iterator TagIterator;

        Iterator(TagIterator iter);

        TagIterator iter;

        
    };

    /**
     * @brief For iterating over a certain cell.
     */
    class CellIterator : public std::iterator<std::input_iterator_tag, Node> {

      public:

        /**
         * @brief Test if they point to the same network node.
         */
        bool operator==(const CellIterator& ref);

        /**
         * @brief Test if they point to different network nodes.
         */
        bool operator!=(const CellIterator& ref);

        /**
         * @brief Get the reference of network nodes.
         */
        Node& operator*();

        /**
         * @brief Used to reference members of the node pointed to.
         */
        Node* operator->();

        /**
         * @brief Increment the iterator.
         */
        CellIterator& operator++();

      private:

        friend class CellularTorus;
        
        typedef std::list<NodePtr> CellList;

        CellIterator(CellList::iterator iter);

        CellList::iterator iter;

    };

    /**
     * @brief For iterating over receivers for a certain transmitter.
     */
    class ReceiverIterator
      : public std::iterator<std::input_iterator_tag, Node> {
      
      public:

        /**
         * @brief Test if they point to the same network node.
         */
        bool operator==(const ReceiverIterator& ref);

        /**
         * @brief Test if they point to different network nodes.
         */
        bool operator!=(const ReceiverIterator& ref);

        /**
         * @brief Get the reference of network nodes.
         */
        Node& operator*();

        /**
         * @brief Used to reference members of the node pointed to.
         */
        Node* operator->();

        /**
         * @brief Increment the iterator.
         */
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
        CellList& get_current_list();  // According to index_now.

        CellularTorus* net;
        Node* sender;
        CellList::iterator now;
        int index_now;  // Range from 0 to 8.

    };

    /**
     * @brief Provides a way to randomly choose a node between two iterators.
     * @param a The starting iterator.
     * @param b The inclusive ending iterator.
     * @return The iterator randomly chosen between @a a and @a b.
     * 
     * This template provides a way to optimize the random choose process.
     */
    template<typename T>
    static T random_choose(T a, T b) {
      int d = std::distance(a, b);
      if (d == 0) {
        return b;
      }
      std::advance(a, Math::get_random_int(0, d - 1));
      return a;
    }

    /**
     * @brief Create instance.
     * @return The instance that should be deleted by user.
     */
    static CellularTorus* create(Driver& driver);

    /**
     * @brief Announce the options that is recognized by the class.
     */
    static void announce_options(Driver& driver);

    /**
     * @brief Default destructor.
     */
    ~CellularTorus();

    /**
     * @brief Add a node to the network.
     *
     * The ownership of the node is transfered to the network model.
     */
    void add_node(Motion& placer, NodePtr&& node);

    /**
     * @brief Get the beginning iterator for all nodes.
     */
    Iterator begin();

    /**
     * @brief Get the endding iterator for all nodes.
     */
    Iterator end();

    /**
     * @brief Get the beginning iterator for nodes in a certain cell.
     * @param x Cell index x, ranges from 0 to network size - 1.
     * @param y Cell index y, ranges from 0 to network size - 1.
     */
    CellIterator begin(int x, int y);

    /**
     * @brief Get the endding iterator for nodes in a certain cell.
     * @param x Cell index x, ranges from 0 to network size - 1.
     * @param y Cell index y, ranges from 0 to network size - 1.
     */
    CellIterator end(int x, int y);

    /**
     * @brief Get the beginning iterator for all receivers of a sender.
     * @param sender The transmitter node.
     */ 
    ReceiverIterator receiver_begin(Node& sender);

    /**
     * @brief Get the endding iterator for all receivers of a sender.
     * @param sender The transmitter node.
     */ 
    ReceiverIterator receiver_end(Node& sender);

    /**
     * @brief Move all nodes in the network with a given @ref motion model.
     */
    void move_nodes(Motion& mover);

    /**
     * @brief Find the node with a given tag.
     */
    Node& get_node(int tag);

    /**
     * @brief Get the node count of this network.
     */
    int get_node_count();

    /**
     * @brief Get the network size.
     * @return Size for both x and y scales.
     */
    int get_size() const;

  private:

    typedef std::unordered_map<int, Node*> TagMap;
    typedef std::vector<std::vector<std::list<NodePtr>>> CellListArray;
    typedef std::vector<std::vector<std::list<NodePtr>::iterator>> HeadArray;

    CellularTorus(Driver& driver, int size);
    void reset_heads();

    Driver& driver;
    int size;
    CellListArray nodes;
    HeadArray heads;
    TagMap tag_map;

};

#endif
