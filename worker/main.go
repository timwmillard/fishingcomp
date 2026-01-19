package main

import (
	"context"
	"flag"
	"fmt"
	"log/slog"
	"os"
	"os/signal"

	"github.com/jackc/pgx/v5/pgxpool"
	"github.com/riverqueue/river"
	"github.com/riverqueue/river/riverdriver/riverpgxv5"
)

func main() {

	flag.Parse()

	ctx := context.Background()
	ctx, cancel := context.WithCancel(ctx)

	// Database setup
	var connString string
	if flag.NArg() > 0 {
		connString = fmt.Sprintf("database=%s", flag.Arg(0))
	}

	if connString == "" {
		connString = os.Getenv("DATABASE_URL")
	}

	dbPoolConfig, err := pgxpool.ParseConfig(connString)
	if err != nil {
		slog.Error("Database Pool: config", "error", err)
		os.Exit(1)
	}

	// Set the schema in search path.
	dbPoolConfig.ConnConfig.RuntimeParams["search_path"] = "worker"

	pool, err := pgxpool.NewWithConfig(ctx, dbPoolConfig)
	if err != nil {
		slog.Error("Database Pool: connection", "error", err)
		os.Exit(1)
	}

	slog.Info("Database Pool: successful connection",
		"host", pool.Config().ConnConfig.Config.Host,
		"port", pool.Config().ConnConfig.Config.Port,
		"user", pool.Config().ConnConfig.Config.User,
		"database", pool.Config().ConnConfig.Config.Database,
		"search_path", pool.Config().ConnConfig.RuntimeParams["search_path"],
	)

	err = pool.Ping(ctx)
	if err != nil {
		slog.Error("Database Pool: ping", "error", err)
		os.Exit(1)
	}
	slog.Info("Database Pool: ping success")

	err = startRiver(ctx, pool)
	if err != nil {
		slog.Error("River: start", "error", err)
		os.Exit(1)
	}
	slog.Info("River: start success")

	// Wait for CTRL-C
	sigChan := make(chan os.Signal, 1)
	signal.Notify(sigChan, os.Interrupt)
	// We block here until a CTRL-C / SigInt is received
	// Once received, we exit and the server is cleaned up
	<-sigChan

	cancel()
}

func startRiver(ctx context.Context, db *pgxpool.Pool) error {
	slackNotify := SlackNotify{}

	workers := river.NewWorkers()
	river.AddWorker(workers, slackNotify)

	riverClient, err := river.NewClient(riverpgxv5.New(db), &river.Config{
		Queues: map[string]river.QueueConfig{
			river.QueueDefault: {MaxWorkers: 50},
		},
		Workers: workers,
	})
	if err != nil {
		return err
	}
	err = riverClient.Start(ctx)
	if err != nil {
		return err
	}

	return nil
}
