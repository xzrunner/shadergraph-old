#ifndef _SHADERLAB_OBJECT_H_
#define _SHADERLAB_OBJECT_H_

namespace sl
{

class Object
{
public:
	Object() : m_count(1) {}
	virtual ~Object() {}

	virtual void Retain() const {
		++m_count;
	}
	virtual void Release() const {
		if (--m_count <= 0) {
			delete this;
		}
	}

private:
	mutable int m_count;

}; // Object

#define OBJ_ASSIGN(left, right) { \
	if ((left) != (right)) { \
		if ((left)) (left)->Release(); \
		left = right; \
		if ((left)) (left)->Retain(); \
	} \
}

}

#endif // _SHADERLAB_OBJECT_H_