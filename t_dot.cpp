#include <scu.h>

#include "dot.hpp"
#include "module.hpp"
#include <sstream>

SCU_MODULE("dot");

using awcdst::BasicBlock;
using awcdst::DotRenderer;
using awcdst::Function;

SCU_TEST(bb_successor_dot, "dot") {
  auto f = Function::make("Function");

  auto bb_e = BasicBlock::make("Entry");
  auto bb_a = BasicBlock::make("A");
  auto bb_b = BasicBlock::make("B");
  auto bb_c = BasicBlock::make("C");

  f->addBasicBlock(bb_e, Function::Entry);
  f->addBasicBlock(bb_a);
  f->addBasicBlock(bb_b);
  f->addBasicBlock(bb_c);

  bb_e->addSuccessor(bb_a, "true");
  bb_e->addSuccessor(bb_b, "false");

  bb_a->addSuccessor(bb_c, "");
  bb_b->addSuccessor(bb_c, "");

  std::ostringstream o;
  o << DotRenderer(f);
  SCU_ASSERT_EQUAL(o.str(), "digraph cfg {\n"
                            "  n0 [label=\"Entry\"];\n"
                            "  n1 [label=\"A\"];\n"
                            "  n2 [label=\"B\"];\n"
                            "  n3 [label=\"C\"];\n"
                            "  n0 -> n2 [label=\"false\"];\n"
                            "  n0 -> n1 [label=\"true\"];\n"
                            "  n1 -> n3 [label=\"\"];\n"
                            "  n2 -> n3 [label=\"\"];\n"
                            "}\n");
}
