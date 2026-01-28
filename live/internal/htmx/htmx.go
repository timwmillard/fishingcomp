package htmx

import "net/http"

// Boosted indicates that the request is via an element using hx-boost
func Boosted(req *http.Request) bool {
	return req.Header.Get("HX-Boosted") == "true"
}

// CurrentURL the current URL of the browser
func CurrentURL(req *http.Request) string {
	return req.Header.Get("HX-Current-URL")
}

// HistoryRestoreRequest if the request is for history restoration after a
// miss in the local history cache
func HistoryRestoreRequest(req *http.Request) bool {
	return req.Header.Get("HX-History-Restore-Request") == "true"
}

// Prompt the user response to an `hx-prompt`
func Prompt(req *http.Request) string {
	return req.Header.Get("HX-Prompt")
}

// Request was requested via htmx
func Request(req *http.Request) bool {
	return req.Header.Get("HX-Request") == "true"
}

// Target the `id` of the target element if it exists
func Target(req *http.Request) string {
	return req.Header.Get("HX-Target")
}

// TriggerName the `name` of the triggered element if it exists
func TriggerName(req *http.Request) string {
	return req.Header.Get("HX-Trigger-Name")
}

// Trigger the `id` of the triggered element if it exists
func TriggerID(req *http.Request) string {
	return req.Header.Get("HX-Trigger")
}

// Location allows you to do a client-side redirect that does not do a full
// page reload
//
// https://htmx.org/headers/hx-location/
func Location(wr http.ResponseWriter, location string) {
	wr.Header().Add("HX-Location", location)
}

// PushURL pushes a new url into the history stack
//
// https://htmx.org/headers/hx-push-url/
func PushURL(wr http.ResponseWriter, location string) {
	wr.Header().Add("HX-Push-Url", location)
}

// Redirect can be used to do a client-side redirect to a new location
func Redirect(wr http.ResponseWriter, location string) {
	wr.Header().Add("HX-Redirect", location)
}

// ReplaceURL replaces the current URL in the location bar
//
// https://htmx.org/headers/hx-replace-url/
func ReplaceURL(wr http.ResponseWriter, location string) {
	wr.Header().Add("HX-Replace-Url ", location)
}

// Reswap allows you to specify how the response will be swapped.
// See https://htmx.org/attributes/hx-swap/ for possible values
func Reswap(wr http.ResponseWriter, selector string) {
	wr.Header().Add("HX-Reswap", selector)
}

// Retarget a CSS selector that updates the target of the content update to a
// different element on the page
func Retarget(wr http.ResponseWriter, selector string) {
	wr.Header().Add("HX-Retarget", selector)
}

// Reselect a CSS selector that allows you to choose which part of the
// response is used to be swapped in. Overrides an existing `hx-select` on the
// triggering element
func Reselect(wr http.ResponseWriter, selector string) {
	wr.Header().Add("HX-Reselect", selector)
}

// Trigger allows you to trigger client-side events
//
// https://htmx.org/headers/hx-trigger/
func Trigger(wr http.ResponseWriter, events ...any) {
	// TODO: marshal events into json
	wr.Header().Add("HX-Trigger", "TODO")
}
