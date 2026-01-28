package server

import (
	"context"
	"fmt"
	"net/http"
	"os"
	"sync"
	"time"

	"log/slog"
)

type Server struct {
	port    int32
	server  http.Server
	handler http.Handler
	wg      sync.WaitGroup
}

func New(port int32, handler http.Handler) *Server {
	return &Server{
		handler: handler,
		port:    port,
	}
}

// Start will start the server and if it cannot bind to the port
// it will exit with a fatal log message
func (s *Server) Start() {
	_, cancel := context.WithCancel(context.Background())
	defer cancel()

	s.server = http.Server{
		Addr:              fmt.Sprintf(":%d", s.port),
		Handler:           s.handler,
		ReadHeaderTimeout: 10 * time.Second,
		ReadTimeout:       5 * time.Minute,
		WriteTimeout:      5 * time.Minute,
		MaxHeaderBytes:    http.DefaultMaxHeaderBytes,
	}

	s.wg.Add(1)

	go func() {
		slog.Info("Server: starting, listening on", "addr", s.server.Addr)
		err := s.server.ListenAndServe()
		if err != http.ErrServerClosed {
			slog.Error("Server: failed to start", "error", err)
			os.Exit(1)
		}
		slog.Info("Server: stopped")
		s.wg.Done()
	}()
}

// Stop the Server
func (s *Server) Stop() {
	// Create a context to attempt a graceful shutdown.
	const timeout = 5 * time.Second
	ctx, cancel := context.WithTimeout(context.Background(), timeout)
	defer cancel()

	slog.Info("Server: stopping")

	// Attempt the graceful shutdown by closing the listener
	// and completing all inflight requests
	if err := s.server.Shutdown(ctx); err != nil {
		// Looks like we timed out on the graceful shutdown. Force close.
		slog.Error("Server: shutdown failed, forcing shutdown", "error", err)
		if err = s.server.Close(); err != nil {
			slog.Error("Server: failed to close", "error", err)
		}
	}

	s.wg.Wait()
}
