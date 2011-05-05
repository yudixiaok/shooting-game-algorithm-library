#pragma once
#include <vector>
#include <boost/shared_ptr.hpp>

#define SHARE_PTR(x) \
	typedef boost::shared_ptr<x>	   x##_Sptr; \
	typedef std::vector<x##_Sptr>			x##_Sptrs;

static struct
{
	template<class T> operator boost::shared_ptr<T> ()
	{
		return boost::shared_ptr<T> (new T);
	}
}
SharePtrNew;
