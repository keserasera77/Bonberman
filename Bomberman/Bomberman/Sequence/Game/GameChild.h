#pragma once

namespace Sequence {
namespace Game {

class Parent;

class Child {
public:
  //Child();
	virtual ~Child(){};
	virtual Child* update(Parent*) = 0;
private:

};

} // namespace Sequence 
} // namespace Game 
