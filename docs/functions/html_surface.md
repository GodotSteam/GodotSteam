# Functions - HTML Surface

The **this_handle** argument is optional.  If you do not pass one, GodotSteam will use the internally stored handle instead.

---

## addHeader

!!! function "addHeader( ```string``` key, ```string``` value, ```uint32``` this_handle )"
	Add a header to any HTTP requests from this browser. A full list of standard request fields are available here on [Wikipedia](https://en.wikipedia.org/wiki/List_of_HTTP_header_fields){ target="_blank" }.

	If no handle is passed, GodotSteam will use the internal one.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#AddHeader){ .md-button .md-button--store target="_blank" }

## allowStartRequest

!!! function "allowStartRequest( ```bool``` allowed, ```uint32``` this_handle )"
	Sets whether a pending load is allowed or if it should be canceled. You can use this feature to limit the valid pages allowed in your HTML surface.

	If no handle is passed, GodotSteam will use the internal one.

	**Returns:** void

	**Note:** You _must_ call this in response to a [html_start_request](/signals/html_surface/#html_start_request) callback.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#AllowStartRequest){ .md-button .md-button--store target="_blank" }

## copyToClipboard

!!! function "copyToClipboard( ```uint32``` this_handle )"
	Copy the currently selected text from the current page in an HTML surface into the local clipboard.

	If no handle is passed, GodotSteam will use the internal one.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#CopyToClipboard){ .md-button .md-button--store target="_blank" }

## createBrowser

!!! function "createBrowser( ```string``` user_agent, ```string``` user_css )"
	Create a browser object for displaying of an HTML page.

	If no handle is passed, GodotSteam will use the internal one.

	**Returns:** void

	**Note:** You _must_ call [removeBrowser](/functions/html_surface/#removebrowser) when you are done using this browser to free up the resources associated with it. Failing to do so will result in a memory leak.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#CreateBrowser){ .md-button .md-button--store target="_blank" }

## executeJavascript

!!! function "executeJavascript( ```string``` script, ```uint32``` this_handle )"
	Run a javascript script in the currently loaded page.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#ExecuteJavascript){ .md-button .md-button--store target="_blank" }

## find

!!! function "find( ```string``` search, ```bool``` currently_in_find, ```bool``` reverse, ```uint32``` this_handle )"
	Find a string in the current page of an HTML surface. This is the equivalent of "ctrl+f" in your browser of choice. It will highlight all of the matching strings. You should call [stopFind](/functions/html_surface/#stopfind) when the input string has changed or you want to stop searching.

	Triggers a [html_search_results](/signals/html_surface/#html_search_results) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#Find){ .md-button .md-button--store target="_blank" }

## getLinkAtPosition

!!! function "getLinkAtPosition( ```int``` x, ```int``` y, ```uint32``` this_handle )"
	Retrieves details about a link at a specific position on the current page in an HTML surface.

	Triggers a [html_link_at_position](/signals/html_surface/#html_link_at_position) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#GetLinkAtPosition){ .md-button .md-button--store target="_blank" }

## goBack

!!! function "goBack( ```uint32``` this_handle )"
	Navigate back in the page history.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#GoBack){ .md-button .md-button--store target="_blank" }

## goForward

!!! function "goForward( ```uint32``` this_handle )"
	Navigate forward in the page history.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#GoForward){ .md-button .md-button--store target="_blank" }

## htmlInit

!!! function "htmlInit()"
	Initializes the HTML Surface API. This must be called prior to using any other functions in this interface. You _must_ call [htmlShutdown](/functions/html_surface/#htmlshutdown) when you are done using the interface to free up the resources associated with it. Failing to do so will result in a memory leak.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#Init){ .md-button .md-button--store target="_blank" }

## jsDialogResponse

!!! function "jsDialogResponse( ```bool``` result, ```uint32``` this_handle )"
	Allows you to react to a page wanting to open a javascript modal dialog notification.

	**Returns:** void

	**Note:** You _must_ call this in response to [html_js_alert](/signals/html_surface/#html_js_alert) and [html_js_confirm](/signals/html_surface/#html_js_confirm) callbacks.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#JSDialogResponse){ .md-button .md-button--store target="_blank" }

## keyChar

!!! function "keyChar( ```uint32``` unicode_char, ```int``` key_modifiers, ```uint32``` this_handle )"
	**unicode_char** is the unicode character point for this keypress (and potentially multiple chars per press).

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#KeyChar){ .md-button .md-button--store target="_blank" }

## keyDown

!!! function "keyDown( ```uint32``` native_key_code, ```int``` key_modifiers, ```uint32``` this_handle )"
	Keyboard interactions, native keycode is the virtual key code value from your OS.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#KeyDown){ .md-button .md-button--store target="_blank" }

## keyUp

!!! function "keyUp( ```uint32``` native_key_code, ```int``` key_modifiers, ```uint32``` this_handle )"
	Keyboard interactions, native keycode is the virtual key code value from your OS.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#KeyUp){ .md-button .md-button--store target="_blank" }

## loadURL

!!! function "loadURL( ```string``` url, ```string``` post_data, ```uint32``` this_handle )"
	Navigate to a specified URL. If you send POST data with ```post_data``` then the data should be formatted as: ```name1=value1&name2=value2```. You can load any URI scheme supported by Chromium Embedded Framework including but not limited to: ```http://, https://, ftp://, and file:///```. If no scheme is specified then ```http://``` is used.

	Triggers a [html_start_request](/signals/html_surface/#html_start_request) callback.

	**Returns:** void

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#LoadURL){ .md-button .md-button--store target="_blank" }

## mouseDoubleClick

!!! function "mouseDoubleClick( ```int``` mouseButton, ```uint32``` this_handle )"
	Tells an HTML surface that a mouse button has been double clicked. The click will occur where the surface thinks the mouse is based on the last call to [mouseMove](/functions/html_surface/#mousemove).

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#MouseDoubleClick){ .md-button .md-button--store target="_blank" }

## mouseDown

!!! function "mouseDown( ```int``` mouse_button, ```uint32``` this_handle )"
	Tells an HTML surface that a mouse button has been pressed. The click will occur where the surface thinks the mouse is based on the last call to [mouseMove](/functions/html_surface/#mousemove).

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#MouseDown){ .md-button .md-button--store target="_blank" }

## mouseMove

!!! function "mouseMove( ```int``` x, ```int``` y, ```uint32``` this_handle )"
	Tells an HTML surface where the mouse is.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#MouseMove){ .md-button .md-button--store target="_blank" }

## mouseUp

!!! function "mouseUp( ```int``` mouse_button, ```uint32``` this_handle )"
	Tells an HTML surface that a mouse button has been released. The click will occur where the surface thinks the mouse is based on the last call to [mouseMove](/functions/html_surface/#mousemove).

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#MouseUp){ .md-button .md-button--store target="_blank" }

## mouseWheel

!!! function "mouseWheel( ```int32``` delta )"
	Tells an HTML surface that the mouse wheel has moved.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#MouseWheel){ .md-button .md-button--store target="_blank" }

## pasteFromClipboard

!!! function "pasteFromClipboard( ```uint32``` this_handle )"
	Paste from the local clipboard to the current page in an HTML surface.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#PasteFromClipboard){ .md-button .md-button--store target="_blank" }

## reload

!!! function "reload( ```uint32``` this_handle )"
	Refreshes the current page. The reload will most likely hit the local cache instead of going over the network. This is equivalent to F5 or Ctrl+R in your browser of choice.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#Reload){ .md-button .md-button--store target="_blank" }

## removeBrowser

!!! function "removeBrowser( ```uint32``` this_handle )"
	You _must_ call this when you are done with an HTML surface, freeing the resources associated with it. Failing to call this will result in a memory leak.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#RemoveBrowser){ .md-button .md-button--store target="_blank" }

## setBackgroundMode

!!! function "setBackgroundMode( ```bool``` background_mode, ```uint32``` this_handle )"
	Enable/disable low-resource background mode, where Javascript and repaint timers are throttled, resources are more aggressively purged from memory, and audio/video elements are paused. When background mode is enabled, all HTML5 video and audio objects will execute ".pause()" and gain the property ".\_steam_background_paused = 1". When background mode is disabled, any video or audio objects with that property will resume with ".play()".

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#SetBackgroundMode){ .md-button .md-button--store target="_blank" }

## setCookie

!!! function "setCookie( ```string``` hostname, ```string``` key, ```string``` value, ```string``` path, ```uint32``` expires, ```bool``` secure, ```bool``` http_only )"
	Set a webcookie for a specific hostname. You can read more about the specifics of setting cookies here on [Wikipedia](https://en.wikipedia.org/wiki/HTTP_cookie#Implementation){ target="_blank" }.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#SetCookie){ .md-button .md-button--store target="_blank" }

## setHorizontalScroll

!!! function "setHorizontalScroll( ```uint32``` absolute_pixel_scroll, ```uint32``` this_handle )"
	Scroll the current page horizontally.

	Triggers a [html_horizontal_scroll](/signals/html_surface/#html_horizontal_scroll) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#SetHorizontalScroll){ .md-button .md-button--store target="_blank" }

## setKeyFocus

!!! function "setKeyFocus( ```bool``` has_key_focus, ```uint32``` this_handle )"
	Tell a HTML surface if it has key focus currently, controls showing the I-beam cursor in text controls amongst other things.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#SetKeyFocus){ .md-button .md-button--store target="_blank" }

## setPageScaleFactor

!!! function "setPageScaleFactor( ```float``` zoom, ```int``` pointX, ```int``` pointY, ```uint32``` this_handle )"
	Zoom the current page in an HTML surface. The current scale factor is available from HTML_NeedsPaint_t.flPageScale, HTML_HorizontalScroll_t.flPageScale, and HTML_VerticalScroll_t.flPageScale.

	**Returns:** void

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#SetPageScaleFactor){ .md-button .md-button--store target="_blank" }

## setSize

!!! function "setSize( ```uint32``` width, ```uint32``` height, ```uint32``` this_handle )"
	Sets the display size of a surface in pixels.

	**Returns:** void

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#SetSize){ .md-button .md-button--store target="_blank" }

## setVerticalScroll

!!! function "setVerticalScroll( ```uint32``` absolute_pixel_scroll, ```uint32``` this_handle )"
	Scroll the current page vertically.

	Triggers a [html_vertical_scroll](/signals/html_surface/#html_vertical_scroll) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#SetVerticalScroll){ .md-button .md-button--store target="_blank" }

## htmlShutdown

!!! function "htmlShutdown()"
	Shutdown the ISteamHTMLSurface interface, releasing the memory and handles. You _must_ call this when you are done using this interface to prevent memory and handle leaks. After calling this then all of the functions provided in this interface will fail until you call [htmlInit](/functions/html_surface/#htmlinit) to reinitialize again.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#Shutdown){ .md-button .md-button--store target="_blank" }

## stopFind

!!! function "stopFind( ```uint32``` this_handle )"
	Cancel a currently running find.

	**Returns:** void

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#StopFind){ .md-button .md-button--store target="_blank" }

## stopLoad

!!! function "stopLoad( ```uint32``` this_handle )"
	Stop the load of the current HTML page.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#StopLoad){ .md-button .md-button--store target="_blank" }

## viewSource

!!! function "viewSource( ```uint32``` this_handle )"
	Open the current pages HTML source code in default local text editor, used for debugging.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#ViewSource){ .md-button .md-button--store target="_blank" }
