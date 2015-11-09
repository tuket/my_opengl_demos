#ifndef SINGLETON_HPP
#define SINGLETON_HPP

template <typename T>
class Singleton
{
	
	static T* getSingleton()
	{
		static T* instance = new T();
		return instance;
	}
	
};

#endif
