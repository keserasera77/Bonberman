#pragma once

namespace Sequence {

class Child;

class Parent {
public:
  Parent();
	~Parent();

  static Parent* instance();
	static void create();
	static void destroy();
	void setStageName(const char* filename);
	const char* getStageName() const;
	void update();

private:
  Child* mChild;
  static Parent* mInstance;
	const char* mStageName; 
};

} // namespace Sequence 
