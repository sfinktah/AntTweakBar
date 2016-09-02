#include "../../../../dll/src/AuthorityProjectConfig.h"
#ifdef TEARLESS_MESSAGE_ROUTER
// Copyright (c) 2012 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "binding.h"

#include <algorithm>
#include <string>

#include "include/wrapper/cef_stream_resource_handler.h"
#include "../../../../dll/src/Sfinktah/debug.h"

namespace client {
	namespace binding_test {
		namespace {
			const char kTestUrl[] = "http://tests/binding";
			const char kTestMessageName[] = "BindingTest";

			// Handle messages in the browser process.
			class Handler : public CefMessageRouterBrowserSide::Handler {
			public:
				Handler() {
					DEBUG_OUT("binding_test::Handler created");
				}

				// Called due to cefQuery execution in binding.html.
				virtual bool OnQuery(CefRefPtr<CefBrowser> browser,
					CefRefPtr<CefFrame> frame,
					int64 query_id,
					const CefString& request,
					bool persistent,
					CefRefPtr<Callback> callback) OVERRIDE 
				{
					// Only handle messages from the test URL.
					const std::string& url = frame->GetURL();
					//if (url.find(kTestUrl) != 0)
					//	return false;
					DEBUG_OUT("MessageRouterBrowser from URL '%s'", url);
					const std::string& message_name = request;
					if (message_name.find(kTestMessageName) == 0) {
						// Reverse the string and return.
						std::string result = message_name.substr(sizeof(kTestMessageName));
						std::reverse(result.begin(), result.end());
						callback->Success(result);
						return true;
					}

					return false;
				}
			};

		}  // namespace
		CefMessageRouterBrowserSide::Handler* newHandler() {
			return new Handler;
		}
		void CreateMessageHandlers(HTML5Plugin::CPluginHTML5::MessageHandlerSet & handlers) {
			handlers.insert(new Handler());
		}

	}  // namespace binding_test
}  // namespace client
#endif