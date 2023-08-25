# HTML Surface

Interface for rendering and interacting with HTML pages.

You can use this interface to render and display HTML pages directly inside your game or application. You must call [htmlInit](#htmlinit) prior to using this interface, and [htmlShutdown](#htmlshutdown) when you're done using it.

See [Steam HTML Surface](https://partner.steamgames.com/doc/features/html_surface){ target="_blank" } for more information.

---

## Functions

The **this_handle** argument is optional. If you do not pass one, GodotSteam will use the internally stored handle instead.

---

### addHeader

!!! function "addHeader( ```string``` key, ```string``` value, ```uint32``` this_handle )"
	Add a header to any HTTP requests from this browser. A full list of standard request fields are available here on [Wikipedia](https://en.wikipedia.org/wiki/List_of_HTTP_header_fields){ target="_blank" }.

	If no handle is passed, GodotSteam will use the internal one.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#AddHeader){ .md-button .md-button--store target="_blank" }

### allowStartRequest

!!! function "allowStartRequest( ```bool``` allowed, ```uint32``` this_handle )"
	Sets whether a pending load is allowed or if it should be canceled. You can use this feature to limit the valid pages allowed in your HTML surface.

	If no handle is passed, GodotSteam will use the internal one.

	**Returns:** void

	**Note:** You _must_ call this in response to a [html_start_request](#html_start_request) callback.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#AllowStartRequest){ .md-button .md-button--store target="_blank" }

### copyToClipboard

!!! function "copyToClipboard( ```uint32``` this_handle )"
	Copy the currently selected text from the current page in an HTML surface into the local clipboard.

	If no handle is passed, GodotSteam will use the internal one.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#CopyToClipboard){ .md-button .md-button--store target="_blank" }

### createBrowser

!!! function "createBrowser( ```string``` user_agent, ```string``` user_css )"
	Create a browser object for displaying of an HTML page.

	If no handle is passed, GodotSteam will use the internal one.

	**Returns:** void

	**Note:** You _must_ call [removeBrowser](#removebrowser) when you are done using this browser to free up the resources associated with it. Failing to do so will result in a memory leak.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#CreateBrowser){ .md-button .md-button--store target="_blank" }

### executeJavascript

!!! function "executeJavascript( ```string``` script, ```uint32``` this_handle )"
	Run a javascript script in the currently loaded page.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#ExecuteJavascript){ .md-button .md-button--store target="_blank" }

### find

!!! function "find( ```string``` search, ```bool``` currently_in_find, ```bool``` reverse, ```uint32``` this_handle )"
	Find a string in the current page of an HTML surface. This is the equivalent of "ctrl+f" in your browser of choice. It will highlight all of the matching strings. You should call [stopFind](#stopfind) when the input string has changed or you want to stop searching.

	Triggers a [html_search_results](#html_search_results) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#Find){ .md-button .md-button--store target="_blank" }

### getLinkAtPosition

!!! function "getLinkAtPosition( ```int``` x, ```int``` y, ```uint32``` this_handle )"
	Retrieves details about a link at a specific position on the current page in an HTML surface.

	Triggers a [html_link_at_position](#html_link_at_position) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#GetLinkAtPosition){ .md-button .md-button--store target="_blank" }

### goBack

!!! function "goBack( ```uint32``` this_handle )"
	Navigate back in the page history.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#GoBack){ .md-button .md-button--store target="_blank" }

### goForward

!!! function "goForward( ```uint32``` this_handle )"
	Navigate forward in the page history.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#GoForward){ .md-button .md-button--store target="_blank" }

### htmlInit

!!! function "htmlInit()"
	Initializes the HTML Surface API. This must be called prior to using any other functions in this interface. You _must_ call [htmlShutdown](#htmlshutdown) when you are done using the interface to free up the resources associated with it. Failing to do so will result in a memory leak.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#Init){ .md-button .md-button--store target="_blank" }

### jsDialogResponse

!!! function "jsDialogResponse( ```bool``` result, ```uint32``` this_handle )"
	Allows you to react to a page wanting to open a javascript modal dialog notification.

	**Returns:** void

	**Note:** You _must_ call this in response to [html_js_alert](#html_js_alert) and [html_js_confirm](#html_js_confirm) callbacks.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#JSDialogResponse){ .md-button .md-button--store target="_blank" }

### keyChar

!!! function "keyChar( ```uint32``` unicode_char, ```int``` key_modifiers, ```uint32``` this_handle )"
	**unicode_char** is the unicode character point for this keypress (and potentially multiple chars per press).

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#KeyChar){ .md-button .md-button--store target="_blank" }

### keyDown

!!! function "keyDown( ```uint32``` native_key_code, ```int``` key_modifiers, ```uint32``` this_handle )"
	Keyboard interactions, native keycode is the virtual key code value from your OS.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#KeyDown){ .md-button .md-button--store target="_blank" }

### keyUp

!!! function "keyUp( ```uint32``` native_key_code, ```int``` key_modifiers, ```uint32``` this_handle )"
	Keyboard interactions, native keycode is the virtual key code value from your OS.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#KeyUp){ .md-button .md-button--store target="_blank" }

### loadURL

!!! function "loadURL( ```string``` url, ```string``` post_data, ```uint32``` this_handle )"
	Navigate to a specified URL. If you send POST data with ```post_data``` then the data should be formatted as: ```name1=value1&name2=value2```. You can load any URI scheme supported by Chromium Embedded Framework including but not limited to: ```http://, https://, ftp://, and file:///```. If no scheme is specified then ```http://``` is used.

	Triggers a [html_start_request](#html_start_request) callback.

	**Returns:** void

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#LoadURL){ .md-button .md-button--store target="_blank" }

### mouseDoubleClick

!!! function "mouseDoubleClick( ```int``` mouseButton, ```uint32``` this_handle )"
	Tells an HTML surface that a mouse button has been double clicked. The click will occur where the surface thinks the mouse is based on the last call to [mouseMove](#mousemove).

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#MouseDoubleClick){ .md-button .md-button--store target="_blank" }

### mouseDown

!!! function "mouseDown( ```int``` mouse_button, ```uint32``` this_handle )"
	Tells an HTML surface that a mouse button has been pressed. The click will occur where the surface thinks the mouse is based on the last call to [mouseMove](#mousemove).

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#MouseDown){ .md-button .md-button--store target="_blank" }

### mouseMove

!!! function "mouseMove( ```int``` x, ```int``` y, ```uint32``` this_handle )"
	Tells an HTML surface where the mouse is.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#MouseMove){ .md-button .md-button--store target="_blank" }

### mouseUp

!!! function "mouseUp( ```int``` mouse_button, ```uint32``` this_handle )"
	Tells an HTML surface that a mouse button has been released. The click will occur where the surface thinks the mouse is based on the last call to [mouseMove](#mousemove).

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#MouseUp){ .md-button .md-button--store target="_blank" }

### mouseWheel

!!! function "mouseWheel( ```int32``` delta )"
	Tells an HTML surface that the mouse wheel has moved.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#MouseWheel){ .md-button .md-button--store target="_blank" }

### pasteFromClipboard

!!! function "pasteFromClipboard( ```uint32``` this_handle )"
	Paste from the local clipboard to the current page in an HTML surface.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#PasteFromClipboard){ .md-button .md-button--store target="_blank" }

### reload

!!! function "reload( ```uint32``` this_handle )"
	Refreshes the current page. The reload will most likely hit the local cache instead of going over the network. This is equivalent to F5 or Ctrl+R in your browser of choice.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#Reload){ .md-button .md-button--store target="_blank" }

### removeBrowser

!!! function "removeBrowser( ```uint32``` this_handle )"
	You _must_ call this when you are done with an HTML surface, freeing the resources associated with it. Failing to call this will result in a memory leak.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#RemoveBrowser){ .md-button .md-button--store target="_blank" }

### setBackgroundMode

!!! function "setBackgroundMode( ```bool``` background_mode, ```uint32``` this_handle )"
	Enable/disable low-resource background mode, where Javascript and repaint timers are throttled, resources are more aggressively purged from memory, and audio/video elements are paused. When background mode is enabled, all HTML5 video and audio objects will execute ".pause()" and gain the property ".\_steam_background_paused = 1". When background mode is disabled, any video or audio objects with that property will resume with ".play()".

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#SetBackgroundMode){ .md-button .md-button--store target="_blank" }

### setCookie

!!! function "setCookie( ```string``` hostname, ```string``` key, ```string``` value, ```string``` path, ```uint32``` expires, ```bool``` secure, ```bool``` http_only )"
	Set a webcookie for a specific hostname. You can read more about the specifics of setting cookies here on [Wikipedia](https://en.wikipedia.org/wiki/HTTP_cookie#Implementation){ target="_blank" }.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#SetCookie){ .md-button .md-button--store target="_blank" }

### setHorizontalScroll

!!! function "setHorizontalScroll( ```uint32``` absolute_pixel_scroll, ```uint32``` this_handle )"
	Scroll the current page horizontally.

	Triggers a [html_horizontal_scroll](#html_horizontal_scroll) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#SetHorizontalScroll){ .md-button .md-button--store target="_blank" }

### setKeyFocus

!!! function "setKeyFocus( ```bool``` has_key_focus, ```uint32``` this_handle )"
	Tell a HTML surface if it has key focus currently, controls showing the I-beam cursor in text controls amongst other things.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#SetKeyFocus){ .md-button .md-button--store target="_blank" }

### setPageScaleFactor

!!! function "setPageScaleFactor( ```float``` zoom, ```int``` pointX, ```int``` pointY, ```uint32``` this_handle )"
	Zoom the current page in an HTML surface. The current scale factor is available from HTML_NeedsPaint_t.flPageScale, HTML_HorizontalScroll_t.flPageScale, and HTML_VerticalScroll_t.flPageScale.

	**Returns:** void

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#SetPageScaleFactor){ .md-button .md-button--store target="_blank" }

### setSize

!!! function "setSize( ```uint32``` width, ```uint32``` height, ```uint32``` this_handle )"
	Sets the display size of a surface in pixels.

	**Returns:** void

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#SetSize){ .md-button .md-button--store target="_blank" }

### setVerticalScroll

!!! function "setVerticalScroll( ```uint32``` absolute_pixel_scroll, ```uint32``` this_handle )"
	Scroll the current page vertically.

	Triggers a [html_vertical_scroll](#html_vertical_scroll) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#SetVerticalScroll){ .md-button .md-button--store target="_blank" }

### htmlShutdown

!!! function "htmlShutdown()"
	Shutdown the ISteamHTMLSurface interface, releasing the memory and handles. You _must_ call this when you are done using this interface to prevent memory and handle leaks. After calling this then all of the functions provided in this interface will fail until you call [htmlInit](#htmlinit) to reinitialize again.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#Shutdown){ .md-button .md-button--store target="_blank" }

### stopFind

!!! function "stopFind( ```uint32``` this_handle )"
	Cancel a currently running find.

	**Returns:** void

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#StopFind){ .md-button .md-button--store target="_blank" }

### stopLoad

!!! function "stopLoad( ```uint32``` this_handle )"
	Stop the load of the current HTML page.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#StopLoad){ .md-button .md-button--store target="_blank" }

### viewSource

!!! function "viewSource( ```uint32``` this_handle )"
	Open the current pages HTML source code in default local text editor, used for debugging.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#ViewSource){ .md-button .md-button--store target="_blank" }

---

## Signals

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

---

### html_browser_ready

!!! function "html_browser_ready"
	A new browser was created and is ready for use.

	**Returns:**

	* browser_handle (uint32)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_BrowserReady_t){ .md-button .md-button--store target="_blank" }

### html_can_go_backandforward

!!! function "html_can_go_backandforward"
	Called when page history status has changed the ability to go backwards and forward.

	**Returns:**

	* browser_handle (uint32)
	* go_back (bool)
	* go_forward (bool)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_CanGoBackAndForward_t){ .md-button .md-button--store target="_blank" }		

### html_changed_title

!!! function "html_changed_title"
	Called when the current page in a browser gets a new title.

	**Returns:**

	* browser_handle (uint32)
	* title (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_ChangedTitle_t){ .md-button .md-button--store target="_blank" }

### html_close_browser

!!! function "html_close_browser"
	Called when the browser has been requested to close due to user interaction; usually because of a javascript window.close() call.

	**Returns:**

	* browser_handle (uint32)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_CloseBrowser_t){ .md-button .md-button--store target="_blank" }

### html_file_open_dialog

!!! function "html_file_open_dialog"
	Called when a browser surface has received a file open dialog from a ```input type="file"``` click or similar, you must call [fileLoadDialogResponse](#fileLoadDialogResponse) with the file(s) the user selected.

	**Returns:**

	* browser_handle (uint32)
	* title (string)
	* initial_file (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_FileOpenDialog_t){ .md-button .md-button--store target="_blank" }

### html_finished_request

!!! function "html_finished_request"
	Called when a browser has finished loading a page.

	**Returns:**

	* browser_handle (uint32)
	* url (string)
	* title (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_FinishedRequest_t){ .md-button .md-button--store target="_blank" }

### html_hide_tooltip

!!! function "html_hide_tooltip"
	Called when a a browser wants to hide a tooltip.

	**Returns:**

	* browser_handle (uint32)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_HideToolTip_t){ .md-button .md-button--store target="_blank" }

### html_horizontal_scroll

!!! function "html_horizontal_scroll"
	Provides details on the visibility and size of the horizontal scrollbar.

	**Returns:**

	* browser_handle (uint32)
	* scroll_data (dictionary)
		* scroll_max (uint32)
		* scroll_current (uint32)
		* page_scale (float)
		* visible (bool)
		* page_size (uint32)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_HorizontalScroll_t){ .md-button .md-button--store target="_blank" }

### html_js_alert

!!! function "html_js_alert"
	Called when the browser wants to display a Javascript alert dialog, call [jsDialogResponse](#jsdialogresponse) when the user dismisses this dialog; or right away to ignore it.

	**Returns:**

	* browser_handle (uint32)
	* message (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_JSAlert_t){ .md-button .md-button--store target="_blank" }

### html_js_confirm

!!! function "html_js_confirm"
	Called when the browser wants to display a Javascript confirmation dialog, call [jsDialogResponse](#jsdialogresponse) when the user dismisses this dialog; or right away to ignore it.

	**Returns:**

	* browser_handle (uint32)
	* message (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_JSConfirm_t){ .md-button .md-button--store target="_blank" }	

### html_link_at_position

!!! function "html_link_at_position"
	Result of a call to [getLinkAtPosition](#getlinkatposition).

	**Returns:**

	* browser_handle (uint32)
	* link_data (dictionary)
		* x (uint32)
		* y (uint32)
		* url (string)
		* input (bool)
		* live_link (bool)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_LinkAtPosition_t){ .md-button .md-button--store target="_blank" }

### html_needs_paint

!!! function "html_needs_paint"
	Called when a browser surface has a pending paint. This is where you get the actual image data to render to the screen.
	
	**Returns:**

	* browser_handle (uint32)
	* page_data (dictionary)
		* bgra (string)
		* wide (uint32)
		* tall (uint32)
		* update_x (uint32)
		* update_y (uint32)
		* update_wide (uint32)
		* update_tall (uint32)
		* scroll_x (uint32)
		* scroll_y (uint32)
		* page_scale (float)
		* page_serial (uint32)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_NeedsPaint_t){ .md-button .md-button--store target="_blank" }

### html_new_window

!!! function "html_new_window"
	A browser has created a new HTML window.

	**Returns:**

	* browser_handle (uint32)
	* window_data (dictionary)
		* url (string)
		* x (uint32)
		* y (uint32)
		* wide (uint32)
		* tall (uint32)
		* new_handle (uint32)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_NewWindow_t){ .md-button .md-button--store target="_blank" }

### html_open_link_in_new_tab

!!! function "html_open_link_in_new_tab"
	The browser has requested to load a url in a new tab.

	**Returns:**

	* browser_handle (uint32)
	* url (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_OpenLinkInNewTab_t){ .md-button .md-button--store target="_blank" }

### html_search_results

!!! function "html_search_results"
	Results from a search.

	**Returns:**

	* browser_handle (uint32)
	* results (uint32)
	* current_match (uint32)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_SearchResults_t){ .md-button .md-button--store target="_blank" }

### html_set_cursor

!!! function "html_set_cursor"
	Called when a browser wants to change the mouse cursor.
	
	**Returns:**

	* browser_handle (uint32)
	* mouse_cursor (uint32)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_SetCursor_t){ .md-button .md-button--store target="_blank" }

### html_show_tooltip

!!! function "html_show_tooltip"
	Called when a browser wants to display a tooltip.
	
	**Returns:**

	* browser_handle (uint32)
	* message (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_ShowToolTip_t){ .md-button .md-button--store target="_blank" }

### html_start_request

!!! function "html_start_request"
	Called when a browser wants to navigate to a new page.

	**Returns:**

	* browser_handle (uint32)
	* url (string)
	* target (string)
	* post_data (string)
	* redirect (bool)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_StartRequest_t){ .md-button .md-button--store target="_blank" }

### html_status_text

!!! function "html_status_text"
	Called when a browser wants you to display an informational message. This is most commonly used when you hover over links.

	**Returns:**

	* browser_handle (uint32)
	* message (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_StatusText_t){ .md-button .md-button--store target="_blank" }

### html_update_tooltip

!!! function "html_update_tooltip"
	Called when the text of an existing tooltip has been updated.

	**Returns:**

	* browser_handle (uint32)
	* message (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_UpdateToolTip_t){ .md-button .md-button--store target="_blank" }

### html_url_changed

!!! function "html_url_changed"
	Called when the browser is navigating to a new url.

	**Returns:**

	* browser_handle (uint32)
	* url_data (dictionary)
		* url (string)
		* post_data (string)
		* redirect (bool)
		* title (string)
		* new_navigation (bool)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_URLChanged_t){ .md-button .md-button--store target="_blank" }
	
### html_vertical_scroll

!!! function "html_vertical_scroll"
	Provides details on the visibility and size of the vertical scrollbar.

	**Returns:**

	* browser_handle (uint32)
	* scroll_data (dictionary)
		* scroll_max (uint32)
		* scroll_current (uint32)
		* page_scale (float)
		* visible (bool)
		* page_size (uint32)

		---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_VerticalScroll_t){ .md-button .md-button--store target="_blank" }

---

## Enums

---

### HTMLKeyModifiers

Enumerator | Value
---------- | -----
HTML_KEY_MODIFIER_NONE | 0
HTML_KEY_MODIFIER_ALT_DOWN | (1<<0)
HTML_KEY_MODIFIER_CTRL_DOWN | (1<<1)
HTML_KEY_MODIFIER_SHIFT_DOWN | (1<<2)

### HTMLMouseButton

Enumerator | Value
---------- | -----
HTML_MOUSE_BUTTON_LEFT | 0
HTML_MOUSE_BUTTON_RIGHT | 1
HTML_MOUSE_BUTTON_MIDDLE | 2

### MouseCursor

Enumerator | Value
---------- | -----
DC_USER | 0
DC_NONE | 1
DC_ARROW | 2
DC_IBEAM | 3
DC_HOUR_GLASS | 4
DC_WAIT_ARROW | 5
DC_CROSSHAIR | 6
DC_UP | 7
DC_SIZE_NW | 8
DC_SIZE_SE | 9
DC_SIZE_NE | 10
DC_SIZE_SW | 11
DC_SIZE_W | 12
DC_SIZE_E | 13
DC_SIZE_N | 14
DC_SIZE_S | 15
DC_SIZE_WE | 16
DC_SIZE_NS | 17
DC_SIZE_ALL | 18
DC_NO | 19
DC_HAND | 20
DC_BLANK | 21
DC_MIDDLE_PAN | 22
DC_NORTH_PAN | 23
DC_NORTH_EAST_PAN | 24
DC_EAST_PAN | 25
DC_SOUTH_EAST_PAN | 26
DC_SOUTH_PAN | 27
DC_SOUTH_WEST_PAN | 28
DC_WEST_PAN | 29
DC_NORTH_WEST_PAN | 30
DC_ALIAS | 31
DC_CELL | 32
DC_COL_RESIZE | 33
DC_COPY_CUR | 34
DC_VERTICAL_TEXT | 35
DC_ROW_RESIZE | 36
DC_ZOOM_IN | 37
DC_ZOOM_OUT | 38
DC_HELP | 39
DC_CUSTOM | 40
DC_LAST | 41
