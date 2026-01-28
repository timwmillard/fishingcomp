package htmx_test

import (
	"live/internal/htmx"
	"net/http"
	"net/http/httptest"
	"testing"
)

func TestRequest(t *testing.T) {
	req := httptest.NewRequest(http.MethodGet, "/", nil)
	req.Header.Add("HX-Request", "true")

	got := htmx.Request(req)
	if !got {
		t.Error("should be htmx request")
	}
}

func TestRedirect(t *testing.T) {
	wr := httptest.NewRecorder()
	htmx.Redirect(wr, "/home")

	if wr.Result().Header.Get("HX-Redirect") != "/home" {
		t.Error("should be redirect to /home")

	}

}
