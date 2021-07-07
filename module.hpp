#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <assert.h>

namespace awcdst {

class Module;
class Function;
class BasicBlock;

class BasicBlock {
  explicit BasicBlock(std::string name) : name(name){};
  friend class Module;

public:
  using Ptr = std::shared_ptr<BasicBlock>;

  static Ptr make(std::string name) {
    return std::shared_ptr<BasicBlock>(new BasicBlock(name));
  }

  const std::string &getName() const { return name; };

  void addSuccessor(Ptr &bb, std::string tag) {
    auto [_, did_insert] = successors.try_emplace(tag, bb);
    if (!did_insert)
      throw std::invalid_argument("tag already there");
  }

  void removeSuccessor(std::string tag) {
    size_t removed = successors.erase(tag);
    if (removed != 1)
      throw std::invalid_argument("can't remove tag that isn't here");
  }

  Ptr getSuccessor(std::string tag) {
    Ptr res = successors.at(tag).lock();
    if (!res) {
      successors.erase(tag);
      throw std::out_of_range(tag);
    }
    return res;
  }

  void eachSuccessor(std::function<void(std::string, Ptr &)> cb) {
    auto it = successors.begin();

    while (it != successors.end()) {
      if (auto x = it->second.lock()) {
        cb(it->first, x);
        it++;
      } else {
        it = successors.erase(it);
      }
    }
  }

private:
  std::unordered_map<std::string, Ptr::weak_type> successors;
  std::string name;
};

class Function {
  explicit Function(std::string name) : name(name){};
  friend class Module;

public:
  using Ptr = std::shared_ptr<Function>;

  static class Entry_t {
  } Entry;
  static Ptr make(std::string name) {
    return std::shared_ptr<Function>(new Function(name));
  }

  void addBasicBlock(BasicBlock::Ptr &bb) { basicblocks.emplace_back(bb); }
  void addBasicBlock(BasicBlock::Ptr &bb, Entry_t) {
    basicblocks.emplace_back(bb);
    entryblock = bb;
  }

  BasicBlock::Ptr at(size_t pos) { return basicblocks.at(pos); };

  BasicBlock::Ptr getEntry() const { return entryblock; }
  void setEntry(BasicBlock::Ptr &bb) {
    auto it = std::find(basicblocks.begin(), basicblocks.end(), bb);
    assert(it != basicblocks.end() &&
           "setEntry must only be called on an BasicBlock already added");
    entryblock = bb;
  }
  void removeBasicBlock(BasicBlock::Ptr &bb) {
    std::erase(basicblocks, bb);
    if (bb == entryblock)
      entryblock.reset();
  }

  const std::string &getName() const { return name; };

  auto begin() const { return basicblocks.begin(); }
  auto end() const { return basicblocks.end(); }

private:
  std::string name;
  std::vector<BasicBlock::Ptr> basicblocks;
  BasicBlock::Ptr entryblock;
};

class Module {
  explicit Module(std::string name) : name(name){};

public:
  using Ptr = std::shared_ptr<Module>;

  const std::string &getName() const { return name; };

  static Ptr make(std::string name) {
    return std::shared_ptr<Module>(new Module(name));
  }

  void addFunction(Function::Ptr &func) { functions.emplace_back(func); }
  void removeFunction(Function::Ptr &func) { std::erase(functions, func); }

  Function::Ptr at(size_t pos) { return functions.at(pos); };

  auto begin() const { return functions.begin(); }
  auto end() const { return functions.end(); }

private:
  std::string name;
  std::vector<Function::Ptr> functions;
};

} // namespace awcdst
