#pragma once
#define CLASS_SIZE(classname) \
	inline int GetClassSize() \
	{ \
	return sizeof(classname); \
	}
#define VIRTUAL_CLASS_SIZE(classname) \
	virtual int GetClassSize() \
	{ \
		return sizeof(classname); \
	}