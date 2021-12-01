#pragma once

#include <string>
#include <list>

namespace dh
{
	namespace resources {
		template<typename ID>
		struct ResourceData
		{
			ResourceData(ID id, std::string filepath) :id(id), filepath(filepath) {};
			ID id;
			std::string filepath;
		};
	}
}
