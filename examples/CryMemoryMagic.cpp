#include <malloc.h>
extern "C" {
	void* CryModuleMalloc(size_t size)
	{
		void* ret = malloc(size);
		return ret;
	};


	//////////////////////////////////////////////////////////////////////////
	void  CryModuleFree(void *ptr)
	{
		free(ptr);
	};
}
