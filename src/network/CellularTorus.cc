#include "network/CellularTorus.h"
#include "driver/Driver.h"
#include "node/node.h"
#include "motion/motion.h"
#include "logger/logger.h"

bool CellularTorus::Iterator::operator==(const Iterator& ref) {
  return iter == ref.iter;
}

bool CellularTorus::Iterator::operator!=(const Iterator& ref) {
  return iter != ref.iter;
}

Node& CellularTorus::Iterator::operator*() {
  return *iter->second;
}

Node* CellularTorus::Iterator::operator->() {
  return iter->second;
}

auto CellularTorus::Iterator::operator++() -> Iterator& {
  ++iter;
  return *this;
}

CellularTorus::Iterator::Iterator(TagIterator iter) : iter(iter) {
}

bool CellularTorus::CellIterator::operator==(const CellIterator& ref) {
  return iter == ref.iter;
}

bool CellularTorus::CellIterator::operator!=(const CellIterator& ref) {
  return iter != ref.iter;
}

Node& CellularTorus::CellIterator::operator*() {
  return *iter->get();
}

Node* CellularTorus::CellIterator::operator->() {
  return iter->get();
}

auto CellularTorus::CellIterator::operator++() -> CellIterator& {
  ++iter;
  return *this;
}

CellularTorus::CellIterator::CellIterator(CellList::iterator iter)
  : iter(iter) {
}

bool CellularTorus::ReceiverIterator::operator==(const ReceiverIterator& ref) {
  return now == ref.now;
}

bool CellularTorus::ReceiverIterator::operator!=(const ReceiverIterator& ref) {
  return now != ref.now;
}

Node& CellularTorus::ReceiverIterator::operator*() {
  return *now->get();
}

Node* CellularTorus::ReceiverIterator::operator->() {
  return now->get();
}

auto CellularTorus::ReceiverIterator::operator++() -> ReceiverIterator& {
  ++now;
  return validate();
}

const int CellularTorus::ReceiverIterator::OFFSET_X[9] = {
  -1, 0, 1, -1, 0, 1, -1, 0, 1};
const int CellularTorus::ReceiverIterator::OFFSET_Y[9] = {
  -1, -1, -1, 0, 0, 0, 1, 1, 1};

CellularTorus::ReceiverIterator::ReceiverIterator(CellularTorus* net, 
    Node* sender) : net(net), sender(sender) {
  index_now = 0;
  now = get_current_list().begin();
}

auto CellularTorus::ReceiverIterator::validate() -> ReceiverIterator& {
  while (true) {
    CellList& l = get_current_list();
    auto e = l.end();
    while (now != e) {
      if (now->get() == sender) {
        ++now;
      } else {
        return *this;
      }
    }
    if (++index_now < 9) {
      now = get_current_list().begin();
    } else {
      return set_last();
    }
  }
}

auto CellularTorus::ReceiverIterator::set_first() -> ReceiverIterator& {
  index_now = 0;
  now = get_first_list().begin();
  return validate();
}

auto CellularTorus::ReceiverIterator::set_last() -> ReceiverIterator& {
  index_now = 8;
  now = get_last_list().end();
  return *this;
}

auto CellularTorus::ReceiverIterator::get_first_list() -> CellList& {
  const IntPos& c = sender->get_pos();
  IntPos p(c.first - 1, c.second - 1);
  if (p.first < 0) {
    p.first = net->get_size() - 1;
  }
  if (p.second < 0) {
    p.second = net->get_size() - 1;
  }
  return net->nodes[p.first][p.second];
}

auto CellularTorus::ReceiverIterator::get_last_list() -> CellList& {
  const IntPos& c = sender->get_pos();
  IntPos p(c.first + 1, c.second + 1);
  if (p.first == net->get_size()) {
    p.first = 0;
  }
  if (p.second == net->get_size()) {
    p.second = 0;
  }
  return net->nodes[p.first][p.second];
}

auto CellularTorus::ReceiverIterator::get_current_list() -> CellList& {
  const IntPos& c = sender->get_pos();
  IntPos p(c.first + OFFSET_X[index_now], c.second + OFFSET_Y[index_now]);
  if (p.first < 0) {
    p.first = net->get_size() - 1;
  }
  if (p.second < 0) {
    p.second = net->get_size() - 1;
  }
  if (p.first == net->get_size()) {
    p.first = 0;
  }
  if (p.second == net->get_size()) {
    p.second = 0;
  }
  return net->nodes[p.first][p.second];
}

CellularTorus* CellularTorus::create(Driver& driver) {
  int size = driver.get_option<int>("network.size");
  return new CellularTorus(driver, size);
}

void CellularTorus::announce_options(Driver& driver) {
  driver.register_option("network.size", po::value<int>(),
      "cell count for both scales");
}

CellularTorus::~CellularTorus() {
}

void CellularTorus::add_node(Motion& placer, NodePtr&& node) {
  IntPos pos = placer.random_place(*node);
  node->set_pos(pos);
  tag_map[node->get_tag()] = node.get();
  nodes[pos.first][pos.second].push_back(std::move(node));
  heads[pos.first][pos.second] = nodes[pos.first][pos.second].begin();
}

auto CellularTorus::begin() -> Iterator {
  return Iterator(tag_map.begin());
}

auto CellularTorus::end() -> Iterator {
  return Iterator(tag_map.end());
}

auto CellularTorus::begin(int x, int y) -> CellIterator {
  return CellIterator(nodes[x][y].begin());
}

auto CellularTorus::end(int x, int y) -> CellIterator {
  return CellIterator(nodes[x][y].end());
}

auto CellularTorus::receiver_begin(Node& sender) -> ReceiverIterator {
  return ReceiverIterator(this, &sender).validate();
}

auto CellularTorus::receiver_end(Node& sender) -> ReceiverIterator {
  return ReceiverIterator(this, &sender).set_last();
}

void CellularTorus::move_nodes(Motion& mover) {
  for (int x = 0; x < size; ++x) {
    for (int y = 0; y < size; ++y) {
      auto& from = nodes[x][y];
      auto& from_iter = heads[x][y];
      while (from_iter != from.end()) {
        Node& node = *from_iter->get();
        IntPos pos = mover.random_move(node);
        node.set_pos(pos);
        driver.get_logger().node_moved(node);
        auto& to = nodes[pos.first][pos.second];
        auto& to_iter = heads[pos.first][pos.second];
        to.splice(to_iter, from, from_iter++);
      }
    }
  }
  reset_heads();
}

Node& CellularTorus::get_node(int tag) {
  return *tag_map[tag];
}

int CellularTorus::get_node_count() {
  return tag_map.size();
}

int CellularTorus::get_size() const {
  return size;
}

CellularTorus::CellularTorus(Driver& driver, int size)
  : driver(driver), size(size) {
  nodes.resize(size);
  heads.resize(size);
  for (int i = 0; i < size; ++i) {
    nodes[i].resize(size);
    heads[i].resize(size);
  }
  reset_heads();
}

void CellularTorus::reset_heads() {
  for (int x = 0; x < size; ++x) {
    for (int y = 0; y < size; ++y) {
      heads[x][y] = nodes[x][y].begin();
    }
  }
}

