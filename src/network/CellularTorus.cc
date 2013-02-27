#include "network/CellularTorus.h"
#include "driver/Driver.h"
#include "node/node.h"
#include "motion/motion.h"

bool CellularTorus::ReceiverIterator::operator==(const ReceiverIterator& ref) {
  return ref.cell_center == cell_center && &ref.net == &net && ref.now == now;
}

bool CellularTorus::ReceiverIterator::operator!=(const ReceiverIterator& ref) {
  return !operator==(ref);
}

auto CellularTorus::ReceiverIterator::operator++() -> ReceiverIterator& {
  ++now;
  auto lc = last_cell();
  auto last = net.end(lc.first, lc.second);
  while (true) {
    auto current = current_cell();
    if (now != net.end(current.first, current.second) || now == last) {
      break;
    }
    ++index_now;
    current = current_cell();
    now = net.begin(current.first, current.second);
  }
  return *this;
}

const int CellularTorus::ReceiverIterator::OFFSET_X[9] = {
  -1, 0, 1, -1, 0, 1, -1, 0, 1};
const int CellularTorus::ReceiverIterator::OFFSET_Y[9] = {
  -1, -1, -1, 0, 0, 0, 1, 1, 1};

CellularTorus::ReceiverIterator::ReceiverIterator(
    CellularTorus& net, int cx, int cy)
  : net(net), cell_center(cx, cy) {
  for (index_now = 0; index_now < 9; ++index_now) {
    auto c = current_cell();
    auto& l = net.nodes[c.first][c.second];
    if (!l.empty()) {
      now = l.begin();
      return;
    }
  }
  index_now = 8;
  auto c = current_cell();
  now = net.nodes[c.first][c.second].end();
}

CellularTorus::ReceiverIterator::ReceiverIterator(CellularTorus& net)
  : net(net) {
}

IntPos CellularTorus::ReceiverIterator::last_cell() {
  IntPos p(cell_center.first + 1, cell_center.second + 1);
  if (p.first == net.get_size()) {
    p.first = 0;
  }
  if (p.second == net.get_size()) {
    p.second = 0;
  }
  return p;
}

IntPos CellularTorus::ReceiverIterator::current_cell() {
  IntPos p(cell_center.first + OFFSET_X[index_now], 
      cell_center.second + OFFSET_Y[index_now]);
  if (p.first < 0) {
    p.first = net.get_size() - 1;
  }
  if (p.second < 0) {
    p.second = net.get_size() - 1;
  }
  if (p.first == net.get_size()) {
    p.first = 0;
  }
  if (p.second == net.get_size()) {
    p.second = 0;
  }
  return p;
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

auto CellularTorus::receiver_begin(int x, int y) -> ReceiverIterator {
  return ReceiverIterator(*this, x, y);
}

auto CellularTorus::receiver_end(int x, int y) -> ReceiverIterator {
  ReceiverIterator iter(*this);
  iter.cell_center = IntPos(x, y);
  iter.index_now = 8;
  auto lc = iter.last_cell();
  iter.now = nodes[lc.first][lc.second].end();
  return iter;
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
