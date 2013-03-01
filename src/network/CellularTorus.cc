#include "network/CellularTorus.h"
#include "driver/Driver.h"
#include "node/node.h"
#include "motion/motion.h"

bool CellularTorus::ReceiverIterator::operator==(const ReceiverIterator& ref) {
  return now == ref.now;
}

bool CellularTorus::ReceiverIterator::operator!=(const ReceiverIterator& ref) {
  return now != ref.now;
}

NodePtr& CellularTorus::ReceiverIterator::operator*() {
  return *now;
}

NodePtr& CellularTorus::ReceiverIterator::operator->() {
  return *now;
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
    CellIterator e = l.end();
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
  nodes[pos.first][pos.second].push_back(std::move(node));
}

auto CellularTorus::begin(int x, int y) -> CellIterator {
  return nodes[x][y].begin();
}

auto CellularTorus::end(int x, int y) -> CellIterator {
  return nodes[x][y].end();
}

auto CellularTorus::receiver_begin(Node* sender) -> ReceiverIterator {
  return ReceiverIterator(this, sender).validate();
}

auto CellularTorus::receiver_end(Node* sender) -> ReceiverIterator {
  return ReceiverIterator(this, sender).set_last();
}

int CellularTorus::get_size() const {
  return size;
}

CellularTorus::CellularTorus(Driver& driver, int size)
  : driver(driver), size(size) {
  nodes.resize(size);
  for (auto& i : nodes) {
    i.resize(size);
  }
}
