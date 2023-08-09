# Signals - HTML Surface

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

---

## html_browser_ready

!!! function "html_browser_ready"
	A new browser was created and is ready for use.

	**Returns:**

	* browser_handle (uint32)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_BrowserReady_t){ .md-button .md-button--store target="_blank" }

## html_can_go_backandforward

!!! function "html_can_go_backandforward"
	Called when page history status has changed the ability to go backwards and forward.

	**Returns:**

	* browser_handle (uint32)
	* go_back (bool)
	* go_forward (bool)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_CanGoBackAndForward_t){ .md-button .md-button--store target="_blank" }		

## html_changed_title

!!! function "html_changed_title"
	Called when the current page in a browser gets a new title.

	**Returns:**

	* browser_handle (uint32)
	* title (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_ChangedTitle_t){ .md-button .md-button--store target="_blank" }

## html_close_browser

!!! function "html_close_browser"
	Called when the browser has been requested to close due to user interaction; usually because of a javascript window.close() call.

	**Returns:**

	* browser_handle (uint32)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_CloseBrowser_t){ .md-button .md-button--store target="_blank" }

## html_file_open_dialog

!!! function "html_file_open_dialog"
	Called when a browser surface has received a file open dialog from a ```input type="file"``` click or similar, you must call [fileLoadDialogResponse](/functions/html-surface/#fileLoadDialogResponse) with the file(s) the user selected.

	**Returns:**

	* browser_handle (uint32)
	* title (string)
	* initial_file (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_FileOpenDialog_t){ .md-button .md-button--store target="_blank" }

## html_finished_request

!!! function "html_finished_request"
	Called when a browser has finished loading a page.

	**Returns:**

	* browser_handle (uint32)
	* url (string)
	* title (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_FinishedRequest_t){ .md-button .md-button--store target="_blank" }

## html_hide_tooltip

!!! function "html_hide_tooltip"
	Called when a a browser wants to hide a tooltip.

	**Returns:**

	* browser_handle (uint32)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_HideToolTip_t){ .md-button .md-button--store target="_blank" }

## html_horizontal_scroll

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

## html_js_alert

!!! function "html_js_alert"
	Called when the browser wants to display a Javascript alert dialog, call [jsDialogResponse](/functions/html_surface/#jsdialogresponse) when the user dismisses this dialog; or right away to ignore it.

	**Returns:**

	* browser_handle (uint32)
	* message (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_JSAlert_t){ .md-button .md-button--store target="_blank" }

## html_js_confirm

!!! function "html_js_confirm"
	Called when the browser wants to display a Javascript confirmation dialog, call [jsDialogResponse](/functions/html_surface/#jsdialogresponse) when the user dismisses this dialog; or right away to ignore it.

	**Returns:**

	* browser_handle (uint32)
	* message (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_JSConfirm_t){ .md-button .md-button--store target="_blank" }	

## html_link_at_position

!!! function "html_link_at_position"
	Result of a call to [getLinkAtPosition](/functions/html_surface/#getlinkatposition).

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

## html_needs_paint

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

## html_new_window

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

## html_open_link_in_new_tab

!!! function "html_open_link_in_new_tab"
	The browser has requested to load a url in a new tab.

	**Returns:**

	* browser_handle (uint32)
	* url (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_OpenLinkInNewTab_t){ .md-button .md-button--store target="_blank" }

## html_search_results

!!! function "html_search_results"
	Results from a search.

	**Returns:**

	* browser_handle (uint32)
	* results (uint32)
	* current_match (uint32)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_SearchResults_t){ .md-button .md-button--store target="_blank" }

## html_set_cursor

!!! function "html_set_cursor"
	Called when a browser wants to change the mouse cursor.
	
	**Returns:**

	* browser_handle (uint32)
	* mouse_cursor (uint32)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_SetCursor_t){ .md-button .md-button--store target="_blank" }

## html_show_tooltip

!!! function "html_show_tooltip"
	Called when a browser wants to display a tooltip.
	
	**Returns:**

	* browser_handle (uint32)
	* message (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_ShowToolTip_t){ .md-button .md-button--store target="_blank" }

## html_start_request

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

## html_status_text

!!! function "html_status_text"
	Called when a browser wants you to display an informational message. This is most commonly used when you hover over links.

	**Returns:**

	* browser_handle (uint32)
	* message (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_StatusText_t){ .md-button .md-button--store target="_blank" }

## html_update_tooltip

!!! function "html_update_tooltip"
	Called when the text of an existing tooltip has been updated.

	**Returns:**

	* browser_handle (uint32)
	* message (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTMLSurface#HTML_UpdateToolTip_t){ .md-button .md-button--store target="_blank" }

## html_url_changed

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
	
## html_vertical_scroll

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