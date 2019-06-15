#pragma once

namespace Sequence {

class Parent;

class Child {
public:
  //Child();
	virtual ~Child(){};
	virtual Child* update(Parent*) = 0;
private:
};

}