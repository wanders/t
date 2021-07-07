#pragma once

#include <iostream>

#include "module.hpp"

namespace awcdst {
class DotRenderer {
public:
  explicit DotRenderer(Function::Ptr func) : func(func) {}
  friend std::ostream &operator<<(std::ostream &out, DotRenderer const &d) {
    out << "digraph cfg {\n";

    /* Nodes */
    int i = 0;
    std::unordered_map<BasicBlock::Ptr, int> nodeids;
    for (auto &bb : *d.func) {
      out << "  n" << i << " [label=\"" << bb->getName() << "\"];\n";
      nodeids.emplace(bb, i++);
    }
    /* Edges */
    for (auto &bb : *d.func) {
      bb->eachSuccessor([&](std::string tag, BasicBlock::Ptr &succ) {
        out << "  n" << nodeids[bb] << " -> n" << nodeids[succ] << " [label=\""
            << tag << "\"];\n";
      });
    }
    out << "}\n";
    return out;
  }

private:
  Function::Ptr func;
};
} // namespace awcdst
