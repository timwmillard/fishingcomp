package job

import (
	"context"
	"fmt"

	"github.com/riverqueue/river"
)

type Email struct {
	To      string `json:"to,omitempty"`
	From    string `json:"from,omitempty"`
	CC      string `json:"cc,omitempty"`
	BCC     string `json:"bcc,omitempty"`
	Subject string `json:"subject,omitempty"`
	Message string `json:"message,omitempty"`
}

func (Email) Kind() string { return "email.send" }

type SendEmail struct {
	river.WorkerDefaults[Email]
}

func (w SendEmail) Work(ctx context.Context, job *river.Job[Email]) error {
	fmt.Printf("Sending Email Message: %s\n", job.Args.Message)
	return nil
}
