#include "Allocator.h"
#include <vector>
#include <iostream>

int main()
{
		std::vector<void*> vec;
		allocator::FixedSizeAllocator allctor(4);
		for (int i=0; i<513; i++)
		{
				vec.push_back(allctor.allocate());
				std::cout << vec.back() << std::endl;
		}
		for (int i=0; i<513; i++)
		{
				std::cout << i << std::endl;
				allctor.deallocate(vec[i]);
		}
}


