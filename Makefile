
ifndef SCUDIR
$(error SCUDIR must be set)
endif

TESTCASES:=t_module t_dot
.PHONY: test
test: $(TESTCASES)
	$(SCUDIR)/testrunner $(TESTCASES)


CXXFLAGS:=-std=c++20 -Wall -Wextra -Werror

t_module: module.hpp
t_dot: module.hpp dot.hpp

CXX=clang++-11

t_%: t_%.cpp
	$(CXX) -o $@ $(CXXFLAGS) $<  -I$(SCUDIR)/libscu-c/ -L$(SCUDIR)/libscu-c/ -lscu-c

