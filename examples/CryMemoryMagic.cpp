#include <malloc.h>
extern "C" {
	void* CryModuleMalloc(size_t size)
	{
		void* ret = malloc(size);
		return ret;
	};

	void* CryModuleRealloc(void* block, size_t size)
	{
		void* ret = realloc(block, size);
		return ret;
	};


	//////////////////////////////////////////////////////////////////////////
	void  CryModuleFree(void *ptr)
	{
		free(ptr);
	};
}
