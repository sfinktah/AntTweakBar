#pragma once
#include <IPluginManager.h>
namespace PluginManager {

	class FakePluginManager : public PluginManager::IPluginManager
	{
	public:

		FakePluginManager()
		{
		}

		~FakePluginManager()
		{
		}

		/**
		* @brief Get PluginManager base interface
		*/
		virtual IPluginBase* GetBase() const { return reinterpret_cast<IPluginBase*>(0); }

		/**
		* @brief UnloadAllPlugins
		*/
		virtual void UnloadAllPlugins() {  }

		/**
		* @brief Unload Plugin DLL
		* Its better to directly call release on the interface of the plugin.
		* @param sPluginName Name of the plugin to unload.
		*/
		virtual void UnloadPlugin(const char* sPluginName) {  }

		/**
		* @brief ReloadAllPlugins
		*/
		virtual void ReloadAllPlugins() { }

		/**
		* @brief Reload Plugin DLL
		* @param sPluginPath Path of the plugin to reload
		*/
		virtual bool ReloadPlugin(const char* sPluginPath, bool bInitialize = false) { return static_cast<bool>(0); }

		/**
		* @brief Initialize a plugin directly
		* @param sPluginName Name of the plugin to unload.
		* @return success
		*/
		virtual bool InitializePlugin(const char* sPluginName) { return static_cast<bool>(0); }

		/**
		* @brief Initialize a range of plugins
		* @param nBeginAtMode Start of the range to initialize.
		* @param nEndAtMode End of the range to initialize.
		*/
		virtual void InitializePluginRange(int nBeginAtMode = IM_Min, int nEndAtMode = IM_Max) {  }

		/**
		* @brief Get base interface of a specific plugin if it is loaded
		* @return BaseInterface of the plugin requested or NULL if not found.
		*/
		virtual IPluginBase* GetPluginByName(const char* sPluginName) const { return reinterpret_cast<IPluginBase*>(0); }

		/**
		* @brief Retrieve directory of a plugin (e.g. "C:\cryengine3_3.4.0\Bin32\Plugins\Flite")
		* @return directory path without slash
		*/
		virtual const char* GetPluginDirectory(const char* sPluginName) const { return reinterpret_cast<const char*>(0); }

		/**
		* @brief Retrieve directory containing all plugins (e.g. "C:\cryengine3_3.4.0\Bin32\Plugins")
		* @return directory path without slash
		*/
		virtual const char* GetDirectoryPlugins() const { return reinterpret_cast<const char*>(0); }

		/**
		* @brief Retrieve directory containing all binaries (e.g. "C:\cryengine3_3.4.0\Bin32")
		* @return directory path without slash
		*/
		virtual const char* GetDirectoryBinary() const { return reinterpret_cast<const char*>(0); }

		/**
		* @brief Retrieve root engine directory (e.g. "C:\cryengine3_3.4.0")
		* @return directory path without slash
		*/
		virtual const char* GetDirectoryRoot() const { return reinterpret_cast<const char*>(0); }

		/**
		* @brief Retrieve Game directory (e.g. "C:\cryengine3_3.4.0\Game")
		* @return directory path without slash
		*/
		virtual const char* GetDirectoryGame() const { return reinterpret_cast<const char*>(0); }

		/**
		* @brief Retrieve User settings/cache directory (e.g. "C:\cryengine3_3.4.0\USER")
		* @return directory path without slash
		*/
		virtual const char* GetDirectoryUser() const { return reinterpret_cast<const char*>(0); }

		/**
		* @brief Retrieve a static interface
		* @warning This should never be a requirement for a plugin to work it can be an additional improvement, so use it sparingly if you want your plugin to be reusable.
		* @param sName name of the interface to retrieve
		* @param sVersion of the interface to retrieve
		*  @arg NULL current version/doesn't matter
		* @return the interface pointer, or NULL if not found. Use static_cast to get your interface.
		*/
		virtual void* GetStaticInterface(const char* sName, const char* sVersion = NULL) const { return NULL; }

		/**
		* @brief Register a static interface
		* A static interface is an interface that exist in a gamedll through the whole lifetime of the plugin manager.
		* It can be used to optionally provide/retrieve additional information/functionality to/from a gamedll.
		* @warning This should never be a requirement for a plugin to work it can be an additional improvement, so use it sparingly if you want your plugin to be reusable.
		* @param pInterface interface pointer (must exist through the whole plugin manager lifetime)
		* @param sName name of the interface to retrieve
		* @param sVersion of the interface to retrieve
		*  @arg NULL current version/doesn't matter
		*/
		virtual void RegisterStaticInterface(void* pInterface, const char* sName, const char* sVersion = NULL) {  }

		/**
		* @brief Delay the call of the function
		* can be used if you don't need full control over delayed execution (the plugin manager will handle it)
		* @param sFilter custom filter for remove control
		* @param pFunc function pointer of delayed function
		* @param pFuncCleanup function pointer that cleans up the data when the delayed call is canceled or finished
		* @param pData Data for delayed function
		* @param fDelay delay amount
		* @param eType delay type
		* @see eDelayType
		* @param pFuncTrigger function pointer delay trigger function
		* @param pFuncTriggerCleanup function pointer that cleans up the data when the delayed call is canceled or finished
		* @param pDataTrigger Data for delay trigger function
		*/
		virtual void DelayFunction(const char* sFilter = NULL, tDelayedCall pFunc = NULL, tDelayedCall pFuncCleanup = NULL, void* pData = NULL, float fDelay = 1.0f, int eType = 1, tDelayedCallTrigger pFuncTrigger = NULL, tDelayedCall pFuncTriggerCleanup = NULL, void* pDataTrigger = NULL) {  }

		/**
		* @brief Uses the Plugin Manager Execution delayer
		* can be used if you don't need full control over delayed execution (the plugin manager will handle it)
		* @see PluginManager::DelayConsoleCommand
		* @param sCommand the console command
		* @param fDelay delay amount
		* @param eType delay type
		* @see eDelayType
		* @param pFuncTrigger function pointer delay trigger function
		* @param pFuncTriggerCleanup function pointer that cleans up the data when the delayed call is canceled or finished
		* @param pDataTrigger Data for delay trigger function
		*/
		virtual void DelayCommand(const char* sCommand, const char* sFilter = NULL, float fDelay = 1.0f, int eType = int(eDT_Default), tDelayedCallTrigger pFuncTrigger = NULL, tDelayedCall pFuncTriggerCleanup = NULL, void* pDataTrigger = NULL) { }

		/**
		* @brief Cancels registered delayed calls
		* @param sFilter only delete these delayed calls (if NULL then everything is deleted)
		*/
		virtual void DelayCancel(const char* sFilter = NULL) {  }

		/**
		* @brief Test Lua logic
		* @param sLogic Lua Logic to test e.g. "System.GetCVar("g_skipintro") == 0"
		* @return Lua result or false when error
		*/
		virtual bool TestLuaLogic(const char* sLogic) { return false; }

		/**
		* @brief Run Lua code
		* @param sCode Lua code to execute
		* @return false when error
		*/
		virtual  bool RunLua(const char* sCode) { return false; }

		/**
		* @brief Uses the Plugin Manager Execution delayer
		* can be used if you don't need full control over delayed execution (the plugin manager will handle it)
		* @see PluginManager::DelayConsoleCommand
		* @param sCommand the console command
		* @param fDelay delay amount
		* @param eType delay type
		* @param pFuncTrigger function pointer delay trigger function
		* @param pFuncTriggerCleanup function pointer that cleans up the data when the delayed call is canceled or finished
		* @param pDataTrigger Data for delay trigger function
		*/
		virtual void DelayLua(const char* sCode, const char* sFilter = NULL, float fDelay = 1.0f, int eType = int(eDT_Default), tDelayedCallTrigger pFuncTrigger = NULL, tDelayedCall pFuncTriggerCleanup = NULL, void* pDataTrigger = NULL) { }

		/**
		* @brief Register/Unregister Types in a range of plugins (independent of Init / InitDependencies)
		* Certain object types need to be registered or reregistered at certain points in time or even certain threads.
		* This functionality was added to allow for more flexibility in that regard.
		* @param nBeginAtMode Start of the range
		* @param nEndAtMode End of the range
		* @param nFactoryType The factory type objects to be registered.
		* @see eFactoryType
		* @param bUnregister Set to true if types should be unregistered (most types can't be unregistered atm, but maybe it'll be possible in the future)
		* @return success
		*/
		virtual bool RegisterTypesPluginRange(int nBeginAtMode = IM_Min, int nEndAtMode = IM_Max, int nFactoryType = int(FT_None), bool bUnregister = false) { return static_cast<bool>(0); }

#if defined(WIN_INTERCEPTORS)
		/**
		* @brief Redirect unprocessed Message to interceptors
		* Can manipulate the parameters and skip the default implementation.
		* @return If true doen't process the original WinProc
		*/
		virtual bool PreWinProcInterceptor(HWND& hWnd, UINT& msg, WPARAM& wParam, LPARAM& lParam) const { return static_cast<bool>(0); }

		/**
		* @brief Redirect processed Message to interceptors
		* Can manipulate the LRESULT.
		* @param lResult original WinProc LRESULT
		*/
		virtual LRESULT PostWinProcInterceptor(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT lResult) const { return static_cast<LRESULT>(0); }

		/**
		* @brief Register a WinProc Message Interceptor
		* @param pInterceptor Interceptor to register
		*/
		virtual void RegisterWinProcInterceptor(IPluginWinProcInterceptor* pInterceptor) { ; }

		/**
		* @brief Unregister a WinProc Message Interceptor
		* @param pInterceptor Interceptor to unregister
		*/
		virtual void UnregisterWinProcInterceptor(IPluginWinProcInterceptor* pInterceptor) { }
#endif


	};

}
