# Signals - HTTP

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

---

## http_request_completed

!!! function "http_request_completed"
	Result when an HTTP request completes. If you're using [getHTTPStreamingResponseBodyData](/functions/http/#gethttpstreamingresponsebodydata) then you should be using the [http_request_headers_received](/signals/http/#http_request_headers_received) or [http_request_data_received](/signals/http/#http_request_data_received).

	**Returns:**

	* cookie_handle (uint32)
	* context_value (uint64_t)
	* request_success (bool)
	* status_code (int)
	* body_zize (uint32)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTTP#HTTPRequestCompleted_t){ .md-button .md-button--store target="_blank" }

## http_request_data_received

!!! function "http_request_data_received"
	Triggered when a chunk of data is received from a streaming HTTP request.

	**Returns:**

	* cookie_handle (uint32)
	* context_value (uint64_t)
	* offset (uint32)
	* bytes_received (uint32)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTTP#HTTPRequestDataReceived_t){ .md-button .md-button--store target="_blank" }

## http_request_headers_received

!!! function "http_request_headers_received"
	Triggered when HTTP headers are received from a streaming HTTP request.

	**Returns:**

	* cookie_handle (uint32)
	* context_value (uint64_t)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamHTTP#HTTPRequestHeadersReceived_t){ .md-button .md-button--store target="_blank" }