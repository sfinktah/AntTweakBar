#pragma once
#if 0
#include <CPluginHTML5.h>
namespace HTML5Plugin {
	/**
	* @brief HTML5 Plugin concrete interface
	*/
	struct ISimplePluginHTML5
	{
		/**
		* @brief set the url of the frame that should be rendered
		* @param sURL the url of the website
		* @return true if successful
		*/
		virtual bool SetURL(const wchar_t* sURL) = 0;

		/**
		* @brief execute java script code in the current browser frame
		* @param sJS the java script code
		* @return true if successful
		*/
		virtual bool ExecuteJS(const wchar_t* sJS) = 0;
	};


	class CSimplePluginHTML5 : 
		private CPluginHTML5, 
		public ISimplePluginHTML5 
	{
	public:
		using CPluginHTML5::SetURL; // (const wchar_t* sURL)
		using CPluginHTML5::ExecuteJS; // (const wchar_t* sJS)
	};

	class TearlessImpl : public ISimplePluginHTML5 
	{
	public:
		TearlessImpl(void *_plugin) {
			plugin = dynamic_cast<CSimplePluginHTML5*>(plugin);
		}
		/**
		* @brief set the url of the frame that should be rendered
		* @param sURL the url of the website
		* @return true if successful
		*/
		virtual bool SetURL(const wchar_t* sURL) {
			plugin->SetURL(sURL);
		}

		/**
		* @brief execute java script code in the current browser frame
		* @param sJS the java script code
		* @return true if successful
		*/
		virtual bool ExecuteJS(const wchar_t* sJS) {
			plugin->ExecuteJS(sJS);
		}
	private:
		CSimplePluginHTML5* plugin;
	};

	class Tearless
	{
	public:
		Tearless(ISimplePluginHTML5 *plugin) {
			m_plugin = dynamic_cast<CSimplePluginHTML5 *>(plugin);
		}
		virtual ~Tearless() {};
	public:
		CSimplePluginHTML5* m_plugin;

	};
}
#endif