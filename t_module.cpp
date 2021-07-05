#include <scu.h>

#include "module.hpp"

SCU_MODULE("module");

using awcdst::BasicBlock;
using awcdst::Function;
using awcdst::Module;

/*
 * A module contains zero or more functions.
 */

SCU_TEST(mod_zero, "A module can contain zero functions") {
  auto mod = Module::make("Empty module");

  SCU_ASSERT_EQUAL(std::distance(mod->begin(), mod->end()), 0);
}

SCU_TEST(mod_more, "A module can contain more functions") {
  auto mod = Module::make("Not so empty module");

  auto f1 = Function::make("Func 1");
  auto f2 = Function::make("Func 2");

  mod->addFunction(f1);
  mod->addFunction(f2);

  SCU_ASSERT_EQUAL_FATAL(std::distance(mod->begin(), mod->end()), 2);

  SCU_ASSERT_NOT_EQUAL(mod->at(0), mod->at(1));
  SCU_ASSERT_TRUE(f1 == mod->at(0) || f2 == mod->at(0));
  SCU_ASSERT_TRUE(f1 == mod->at(1) || f2 == mod->at(1));
}

/*
 * A function contains one or more basic blocks. One of them is the entry basic
 * block.
 */
SCU_TEST(func_one, "A function can contain one basic block") {
  auto f = Function::make("Function");

  auto bb = BasicBlock::make("bb8");
  f->addBasicBlock(bb);

  SCU_ASSERT_EQUAL(std::distance(f->begin(), f->end()), 1);
}

SCU_TEST(func_more, "A function can contain more basic blocks") {
  auto func = Function::make("Function");

  auto bb1 = BasicBlock::make("bb8");
  auto bb2 = BasicBlock::make("r2d2");
  func->addBasicBlock(bb1);
  func->addBasicBlock(bb2);

  SCU_ASSERT_EQUAL_FATAL(std::distance(func->begin(), func->end()), 2);

  SCU_ASSERT_NOT_EQUAL(func->at(0), func->at(1));
  SCU_ASSERT_TRUE(bb1 == func->at(0) || bb2 == func->at(0));
  SCU_ASSERT_TRUE(bb1 == func->at(1) || bb2 == func->at(1));
}

SCU_TEST(func_entry, "One of them is the entry basic block") {
  auto f = Function::make("Function");

  auto bb1 = BasicBlock::make("bb8");
  auto bb2 = BasicBlock::make("r2d2");

  f->addBasicBlock(bb1);
  SCU_ASSERT_FALSE((bool)f->getEntry());

  f->addBasicBlock(bb2, Function::Entry);
  SCU_ASSERT_TRUE((bool)f->getEntry());
  SCU_ASSERT_EQUAL(f->getEntry(), bb2);

  f->setEntry(bb1);
  SCU_ASSERT_EQUAL(f->getEntry(), bb1);
}

/*
 * Modules, functions and basic blocks have names.
 */

SCU_TEST(mod_name, "Modules have names") {
  auto mod = Module::make("My module");
  SCU_ASSERT_EQUAL(mod->getName(), "My module");
  mod = Module::make("Not returning a constant");
  SCU_ASSERT_EQUAL(mod->getName(), "Not returning a constant");
}

SCU_TEST(func_name, "Functions have names") {
  auto func = Function::make("Function");
  SCU_ASSERT_EQUAL(func->getName(), "Function");
  func = Function::make("Not returning a constant");
  SCU_ASSERT_EQUAL(func->getName(), "Not returning a constant");
}

SCU_TEST(bb_name, "Basicblocks have names") {
  auto bb = BasicBlock::make("bb");
  SCU_ASSERT_EQUAL(bb->getName(), "bb");
  bb = BasicBlock::make("Not returning a constant");
  SCU_ASSERT_EQUAL(bb->getName(), "Not returning a constant");
}

/*
 * API for creating, inserting and removing
 */

SCU_TEST(mod_removefunc, "Function can be removed from module") {
  auto mod = Module::make("Mod");

  auto f1 = Function::make("Func 1");
  auto f2 = Function::make("Func 2");

  mod->addFunction(f1);
  mod->addFunction(f2);

  SCU_ASSERT_EQUAL_FATAL(std::distance(mod->begin(), mod->end()), 2);

  mod->removeFunction(f1);

  SCU_ASSERT_EQUAL_FATAL(std::distance(mod->begin(), mod->end()), 1);

  SCU_ASSERT_EQUAL(f2, mod->at(0));
}

SCU_TEST(mod_removebb, "Basic blocks can be removed from function") {
  auto f = Function::make("Function");

  auto bb1 = BasicBlock::make("foo");
  auto bb2 = BasicBlock::make("bar");

  f->addBasicBlock(bb1, Function::Entry);
  f->addBasicBlock(bb2);

  SCU_ASSERT_TRUE((bool)f->getEntry());
  SCU_ASSERT_EQUAL(std::distance(f->begin(), f->end()), 2);

  f->removeBasicBlock(bb1);
  SCU_ASSERT_EQUAL(std::distance(f->begin(), f->end()), 1);
  SCU_ASSERT_FALSE((bool)f->getEntry());
  SCU_ASSERT_EQUAL(bb2, f->at(0));
}
