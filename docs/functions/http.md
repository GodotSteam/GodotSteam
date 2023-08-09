# Functions - HTTP

---

## createCookieContainer

!!! function "createCookieContainer( ```bool``` allow_responses_to_modify )"
	Creates a cookie container to store cookies during the lifetime of the process. This API is just for during process lifetime, after Steam restarts no cookies are persisted and you have no way to access the cookie container across repeat executions of your process.

	If **allow_responses_to_modify** is true then any response to your requests using this cookie container may add new cookies to the container which may be transmitted with future requests. Otherwise, if it's false then only cookies you explicitly set will be sent.

	You can associate the cookie container with a http request by using [setHTTPRequestCookieContainer](/functions/http/#sethttprequestcookiecontainer), and you can set a cookie using [setCookie](/functions/http/#setcookie). Don't forget to free the container when you're done with it to prevent leaking memory by calling [releaseCookieContainer](/functions/http/#releasecookiecontainer)!
	
	**Returns:** uint32_t

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTTP#CreateCookieContainer){ .md-button .md-button--store target="_blank" }

## createHTTPRequest

!!! function "createHTTPRequest( ```int``` request_method, ```string``` absolute_url )"

	Initializes a new HTTP request. Requires the method such as GET or POST and the absolute URL for the request. Both http and https are supported, so this string must start with "http://" or "https://" and should look like "http://store.steampowered.com/app/10/" or similar. This call returns a handle that you can use to make further calls to setup and then send the HTTP request with [sendHTTPRequest](/functions/http/#senthttprequest) or [sendHTTPRequestAndStreamResponse](/functions/http/#sendhttprequestandstreamresponse). Don't forget to free the HTTP request when you're done with it to prevent leaking memory by calling [releaseHTTPRequest](/functions/http/#releasehttprequest).

	**Returns:** uint32_t

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTTP#CreateHTTPRequest){ .md-button .md-button--store target="_blank" }

## deferHTTPRequest

!!! function "deferHTTPRequest( ```uint32``` request_handle )"
	Defers a request which has already been sent by moving it at the back of the queue.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTTP#DeferHTTPRequest){ .md-button .md-button--store target="_blank" }

## getHTTPDownloadProgressPct

!!! function "getHTTPDownloadProgressPct( ```uint32``` request_handle )"
	Gets progress on downloading the body for the request. This will be zero unless a response header has already been received which included a content-length field. For responses that contain no content-length it will report zero for the duration of the request as the size is unknown until the connection closes.

	**Returns:** float

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTTP#GetHTTPDownloadProgressPct){ .md-button .md-button--store target="_blank" }

## getHTTPRequestWasTimedOut

!!! function "getHTTPRequestWasTimedOut( ```int``` request_handle )"
	Check if the reason the request failed was because we timed it out (rather than some harder failure). You'll want to call this within the context of [http_request_completed](/signals/http/#http_request_completed) callback if **request_success** is false.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTTP#GetHTTPRequestWasTimedOut){ .md-button .md-button--store target="_blank" }

## getHTTPResponseBodyData

!!! function "getHTTPResponseBodyData( ```uint32``` request_handle, ```uint32``` buffer_size )"
	Gets the body data from an HTTP response. This must be called after the HTTP request has completed and returned the HTTP response via the [http_request_completed](/signals/http/#http_request_completed) call result associated with this request handle. You should first call [getHTTPResponseBodySize](/functions/http/#gethttpresponsebodysize) or use the **body_size** variable provided in the call result, you can then allocate a buffer with that size to pass into this function.
	This is only for HTTP requests which were sent with [sendHTTPRequest](/functions/http/#sendhttprequest). Use [getHTTPStreamingResponseBodyData](/functions/http/#gethttpstreamingresponsebodydata) if you're using streaming HTTP requests via [sendHTTPRequestAndStreamResponse](/functions/http/#sendhttprequestandstreamreponse).

	**Returns:** PoolByteArray

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTTP#GetHTTPResponseBodyData){ .md-button .md-button--store target="_blank" }

## getHTTPResponseBodySize

!!! function "getHTTPResponseBodySize( ```uint32``` request_handle )"
	Gets the size of the body data from an HTTP response. This must be called after the HTTP request has completed and returned the HTTP response via the [http_request_completed](/signals/http/#http_request_completed) or [http_request_data_received](/signals/http/#http_request_data_received) associated with this request handle. If this returns successfully, then you can allocate a buffer with the provided size to fill with the data obtained from [getHTTPResponseBodyData](/functions/http/#gethttpresponsebodydata) or [getHTTPStreamingResponseBodyData](/functions/http/#gethttpstreamingresponsebodydata).

	**Returns:** uint32

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTTP#GetHTTPResponseBodySize){ .md-button .md-button--store target="_blank" }

## getHTTPResponseHeaderSize

!!! function "getHTTPResponseHeaderSize( ```uint32``` request_handle, ```string``` header_name )"
	Checks if a header is present in an HTTP response and returns its size. This must be called after the HTTP request has completed and returned the HTTP response via the [http_request_completed](/signals/http/#http_request_completed) call result associated with this request handle. If the response header exists in the response, then you can allocate a correctly sized buffer to get the associated value with [getHTTPResponseHeaderValue](/functions/http/#gethttpresponseheadervalue). Here is a list of standard response header names on [Wikipedia](https://en.wikipedia.org/wiki/List_of_HTTP_header_fields#Response_fields){ target="_blank" }.

	**Returns:** uint32

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTTP#GetHTTPResponseHeaderSize){ .md-button .md-button--store target="_blank" }

## getHTTPResponseHeaderValue

!!! function "getHTTPResponseHeaderValue( ```uint32``` request_handle, ```string``` header_name, ```uint32``` buffer_size )"
	Gets a header value from an HTTP response. This must be called after the HTTP request has completed and returned the HTTP response via the [http_request_completed](/signals/http/#http_request_completed) call result associated with this request handle. You should first call [getHTTPResponseHeaderSize](/functions/http/#gethttpresponseheadersize) to check for the presence of the header and to get the size. You can then allocate a buffer with that size and pass it into this function. Here is a list of standard response header names on [Wikipedia](https://en.wikipedia.org/wiki/List_of_HTTP_header_fields#Response_fields){ target="_blank" }.

	**Returns:** uint8

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTTP#GetHTTPResponseHeaderValue){ .md-button .md-button--store target="_blank" }

## getHTTPStreamingResponseBodyData

!!! function "getHTTPStreamingResponseBodyData( ```uint32``` request_handle, ```uint32``` offset, ```uint32``` buffer_size )"
	Gets the body data from a streaming HTTP response. This must be called after data is received from a streaming HTTP request via the [http_request_completed](/signals/http/#http_request_completed) callback associated with this request handle. Typically you'll want to allocate a buffer associated with the request handle using the Content-Length HTTP response field to receive the total size of the data when you receive the header via [http_request_headers_received](/signals/http/#http_request_headers_received). You can then append data to that buffer as it comes in. This is only for streaming HTTP requests which were sent with [sendHTTPRequestAndStreamResponse](/functions/http/#sendhttprequestandstreamresponse). Use [getHTTPResponseBodyData](/functions/http/#gethttpresponsebodydata) if you're using [sendHTTPRequest](/functions/http/#sendhttprequest).

	**Returns:** uint32

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTTP#GetHTTPStreamingResponseBodyData){ .md-button .md-button--store target="_blank" }

## prioritizeHTTPRequest

!!! function "prioritizeHTTPRequest( ```uint32``` request_handle )"		
	Prioritizes a request which has already been sent by moving it at the front of the queue.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTTP#PrioritizeHTTPRequest){ .md-button .md-button--store target="_blank" }

## releaseCookieContainer

!!! function "releaseCookieContainer()"
	Releases a cookie container, freeing the memory allocated within Steam. You _must_ call this when you are done using each HTTP cookie container handle that you obtained via [createCookieContainer](/functions/http/#createcookiecontainer)!

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTTP#ReleaseCookieContainer){ .md-button .md-button--store target="_blank" }

## releaseHTTPRequest

!!! function "releaseHTTPRequest( ```uint32``` request_handle )"
	Releases an HTTP request handle, freeing the memory allocated within Steam. You _must_ call this when you are done using each [HTTPRequestHandle](/functions/http/#httprequesthandle) that you obtained via [createHTTPRequest](/functions/http/#createhttprequest)!

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTTP#ReleaseHTTPRequest){ .md-button .md-button--store target="_blank" }

## sendHTTPRequest

!!! function "sendHTTPRequest( ```uint32``` request_handle )"		
	Sends an HTTP request. This call is asynchronous and provides a call result handle which you must use to track the call to its completion. If you have multiple requests in flight at the same time you can use [prioritizeHTTPRequest](/functions/http/#prioritizehttprequest) or [deferHTTPRequest](/functions/http/#deferhttprequest) to set the priority of the request.

	If the user is in offline mode in Steam, then this will add an only-if-cached cache-control header and only do a local cache lookup rather than sending any actual remote request.

	If the data you are expecting is large, you can use [sendHTTPRequestAndStreamResponse](/functions/http/#sendhttprequestandstreamresponse) to stream the data in chunks.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTTP#SendHTTPRequest){ .md-button .md-button--store target="_blank" }

## sendHTTPRequestAndStreamResponse

!!! function "sendHTTPRequestAndStreamResponse( ```uint32``` request_handle )"		
	Sends an HTTP request and streams the response back in chunks. This call is asynchronous and provides a call result handle which you must use to track the call to its completion. Typically you'll want to allocate a buffer associated with the request handle using the Content-Length HTTP response field to receive the total size of the data when you receive the header via [http_request_headers_received](/signals/http/#http_request_headers_received). You can then append data to that buffer as it comes in.

	If you have multiple requests in flight at the same time you can use [prioritizeHTTPRequest](/functions/http/#prioritizehttprequest) or [deferHTTPRequest](/functions/http/#deferhttprequest) to set the priority of the request.

	If the user is in offline mode in Steam, then this will add an only-if-cached cache-control header and only do a local cache lookup rather than sending any actual remote request.

	If the data you are expecting is small (on the order of a few megabytes or less) then you'll likely want to use [sendHTTPRequest](/functions/http/#sendhttprequest).

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTTP#SendHTTPRequestAndStreamResponse){ .md-button .md-button--store target="_blank" }

## setHTTPCookie

!!! function "setHTTPCookie( ```string``` host, ```string``` url, ```string``` cookie )"
	Adds a cookie to the specified cookie container that will be used with future requests.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTTP#SetCookie){ .md-button .md-button--store target="_blank" }

## setHTTPRequestAbsoluteTimeoutMS

!!! function "setHTTPRequestAbsoluteTimeoutMS( ```uint32``` request_handle, ```uint32``` milliseconds )"
	Set an absolute timeout in milliseconds for the HTTP request.

	This is the total time timeout which is different than the network activity timeout which is set with [setHTTPRequestNetworkActivityTimeout](/functions/http/#sethttprequestnetworkactivitytimeout) which can bump everytime we get more data.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTTP#SetHTTPRequestAbsoluteTimeoutMS){ .md-button .md-button--store target="_blank" }

## setHTTPRequestContextValue

!!! function "setHTTPRequestContextValue( ```uint32``` request_handle, ```uint64_t``` context_value )"	
	Set a context value for the request, which will be returned in the [http_request_completed](/signals/http/#http_request_completed) callback after sending the request.

	This is just so the caller can easily keep track of which callbacks go with which request data.

	Must be called before sending the request.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTTP#SetHTTPRequestContextValue){ .md-button .md-button--store target="_blank" }

## setHTTPRequestCookieContainer

!!! function "setHTTPRequestCookieContainer( ```uint32``` request_handle, ```uint32``` cookie_handle )"
	Associates a cookie container to use for an HTTP request.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTTP#SetHTTPRequestCookieContainer){ .md-button .md-button--store target="_blank" }

## setHTTPRequestGetOrPostParameter

!!! function "setHTTPRequestGetOrPostParameter( ```uint32``` request_handle, ```string``` name, ```string``` value )"
	Set a GET or POST parameter value on the HTTP request.

	Must be called prior to sending the request.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTTP#SetHTTPRequestGetOrPostParameter){ .md-button .md-button--store target="_blank" }

## setHTTPRequestHeaderValue

!!! function "setHTTPRequestHeaderValue( ```uint32``` request_handle, ```string``` header_name, ```string``` header_value )"
	Set a request header value for the HTTP request.

	Must be called before sending the request.

	A full list of standard request fields are available here on [Wikipedia](https://en.wikipedia.org/wiki/List_of_HTTP_header_fields#Request_fields){ target="_blank" }. The User-Agent field is explicitly disallowed as it gets overwritten when the request is sent.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTTP#SetHTTPRequestHeaderValue){ .md-button .md-button--store target="_blank" }

## setHTTPRequestNetworkActivityTimeout

!!! function "setHTTPRequestNetworkActivityTimeout( ```uint32``` request_handle, ```uint32``` timeout_seconds )"
	Set the timeout in seconds for the HTTP request.

	The default timeout is 60 seconds if you don't call this. This can get bumped everytime we get more data. Use [setHTTPRequestAbsoluteTimeoutMS](/functions/http/#sethttprequestabsolutetimeout) if you need a strict maximum timeout.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTTP#SetHTTPRequestNetworkActivityTimeout){ .md-button .md-button--store target="_blank" }

## setHTTPRequestRawPostBody

!!! function "setHTTPRequestRawPostBody( ```uint32``` request_handle, ```string``` content_type, ```uint32``` body_length )"
	Sets the body for an HTTP Post request.

	Will fail and return false on a GET request, and will fail if POST params have already been set for the request. Setting this raw body makes it the only contents for the post, the **content_type** parameter will set the "content-type" header for the request to inform the server how to interpret the body.

	**Returns:** uint8

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTTP#SetHTTPRequestRawPostBody){ .md-button .md-button--store target="_blank" }

## setHTTPRequestRequiresVerifiedCertificate

!!! function "setHTTPRequestRequiresVerifiedCertificate( ```uint32``` request_handle, ```bool``` require_verified_certificate )"
	Sets that the HTTPS request should require verified SSL certificate via machines certificate trust store.

	This currently only works Windows and macOS.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTTP#SetHTTPRequestRequiresVerifiedCertificate){ .md-button .md-button--store target="_blank" }

## setHTTPRequestUserAgentInfo

!!! function "setHTTPRequestUserAgentInfo( ```uint32``` request_handle, ```string``` user_agent_info )"
	Set additional user agent info for a request.

	This doesn't clobber the normal user agent, it just adds the extra info on the end. Sending NULL or an empty string resets the user agent info to the default value.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTTP#SetHTTPRequestUserAgentInfo){ .md-button .md-button--store target="_blank" }