// Package live
package live

// Generate Models
//go:generate go tool sqlc generate

// Generate Server
//go:generate go tool oapi-codegen -config api/config.yaml api/spec.yaml
