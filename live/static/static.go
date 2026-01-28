package static

import (
	"embed"
	"io/fs"
	"net/http"

	"github.com/gorilla/mux"
)

//go:embed public/*
var staticFS embed.FS

func Server(path string, router *mux.Router) {

	files, err := fs.Sub(fs.FS(staticFS), "public")
	if err != nil {
		panic(err)
	}
	fs := http.FileServer(http.FS(files))
	router.PathPrefix(path).Handler(http.StripPrefix(path, fs))
}
