package job

import (
	"context"
	"fmt"

	"github.com/riverqueue/river"
)

type SlackMessage struct {
	Message string `json:"message,omitempty"`
}

func (SlackMessage) Kind() string { return "slack.message" }

type SlackNotify struct {
	river.WorkerDefaults[SlackMessage]
}

func (w SlackNotify) Work(ctx context.Context, job *river.Job[SlackMessage]) error {
	fmt.Printf("Sending Slack Message: %s\n", job.Args.Message)
	return nil
}
