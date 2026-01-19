package main

import (
	"context"
	"flag"
	"fmt"
	"gopark/handler"
	"os"
	"photocollection/migrations"
	"strings"

	"github.com/jackc/pgx/v5/pgxpool"
)

var (
	Version = "dev"
)

// Go FlagSet by example:
// https://gobyexample.com/command-line-subcommands
// https://www.kelche.co/blog/go/flag/

func main() {
	showVersion := flag.Bool("version", false, "Print the version number")
	flag.Parse()

	cmd := flag.Arg(0)
	migrateFlag := flag.NewFlagSet("migrate", flag.ExitOnError)
	skipPrompt := migrateFlag.Bool("y", false, "run migration without prompt")
	_ = migrateFlag.Parse(flag.Args()[1:]) // TODO: check error

	if *showVersion {
		version()
	}

	ctx := context.Background()

	config := handler.LoadConfig(Version, false, "")

	dbPoolConfig, err := pgxpool.ParseConfig(config.DB.URL())
	if err != nil {
		fmt.Fprintf(os.Stderr, "Database parse config error: %v\n", err)
		os.Exit(1)
	}

	// Set the schema in search path, to scheduler to the river table can be
	// accessed under the scheduler schema rather under public.
	// https://riverqueue.com/docs/alternate-schema
	dbPoolConfig.ConnConfig.RuntimeParams["search_path"] = "worker"

	pool, err := pgxpool.NewWithConfig(ctx, dbPoolConfig)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Database new pool error: %v\n", err)
		os.Exit(1)
	}

	fmt.Println("*** Fishing Comp Migration ***")
	fmt.Println()
	fmt.Println("--- Database config ---")
	fmt.Printf("host = %v\n", pool.Config().ConnConfig.Config.Host)
	fmt.Printf("port = %v\n", pool.Config().ConnConfig.Config.Port)
	fmt.Printf("user = %v\n", pool.Config().ConnConfig.Config.User)
	fmt.Printf("database = %v\n", pool.Config().ConnConfig.Config.Database)
	fmt.Printf("search_path = %v\n", pool.Config().ConnConfig.RuntimeParams["search_path"])
	fmt.Println("-----------------------")
	fmt.Println()

	err = pool.Ping(ctx)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Database ping error: %v\n", err)
		os.Exit(1)
	}

	if cmd == "migrate" {
		if !*skipPrompt {
			fmt.Print("Are you sure you want to run migration (yes/no): ")
			var yes string
			fmt.Scanln(&yes)
			yes = strings.ToLower(yes)
			if yes != "yes" && yes != "y" {
				fmt.Println("Aborting migration")
				os.Exit(1)
			}
		}
		err = migrations.RunAll(ctx, pool)
		if err != nil {
			fmt.Fprintf(os.Stderr, "Migration error: %v\n", err)
			os.Exit(1)
		}
		fmt.Println("** Migration run all: success **")
	}

}

func version() {
	fmt.Println(Version)
	os.Exit(0)
}
