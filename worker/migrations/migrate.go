package migrations

import (
	"context"
	"embed"
	"fmt"

	"github.com/jackc/pgx/v5/pgxpool"
	"github.com/riverqueue/river/riverdriver/riverpgxv5"
	"github.com/riverqueue/river/rivermigrate"
)

//go:embed *
var FS embed.FS

func Run(ctx context.Context, pool *pgxpool.Pool) error {

	_, err := pool.Exec(ctx, "create schema if not exists worker")
	if err != nil {
		return fmt.Errorf("could create schema scheduler: %w", err)
	}

	riverMigrator := rivermigrate.New(riverpgxv5.New(pool), nil)
	_, err = riverMigrator.Migrate(ctx, rivermigrate.DirectionUp, nil)
	if err != nil {
		return err
	}
	return nil
}
