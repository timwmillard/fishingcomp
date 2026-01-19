package migrations

import (
	"context"
	"embed"
	"fmt"

	"github.com/jackc/pgx/v5"
	"github.com/jackc/tern/v2/migrate"
)

//go:embed *
var FS embed.FS

func Run(ctx context.Context, conn *pgx.Conn) error {
	_, err := conn.Exec(ctx, "create schema if not exists worker")
	if err != nil {
		return fmt.Errorf("could create schema scheduler: %w", err)
	}

	mig, err := migrate.NewMigrator(ctx, conn, "photo.schema_version")
	if err != nil {
		return err
	}
	err = mig.LoadMigrations(FS)
	if err != nil {
		return err
	}
	err = mig.Migrate(ctx)
	if err != nil {
		return err
	}

	return nil
}
