package ai

import (
	"context"
	"encoding/json"
	"fmt"
	"log"
	"os"

	"cloud.google.com/go/vertexai/genai"
)

// CatchDetails holds the structured data returned by the Gemini API call.
type CatchDetails struct {
	Species               string  `json:"species"`
	LengthCM              float64 `json:"length_cm"`
	CompetitorCardVisible bool    `json:"competitor_card_visible"`
	IsValid               bool    `json:"is_valid"`
	Reason                string  `json:"reason,omitempty"`
}

// ValidateCatchPhoto sends the fish photo to Gemini for validation.
// It uses the new cloud.google.com/go/vertexai/genai library.
func ValidateCatchPhoto(ctx context.Context, imageBytes []byte, projectID, location string) (*CatchDetails, error) {
	if os.Getenv("GOOGLE_APPLICATION_CREDENTIALS") == "" {
		log.Println("Warning: GOOGLE_APPLICATION_CREDENTIALS environment variable is not set. Authentication may fail.")
	}

	// Create a new genai client.
	client, err := genai.NewClient(ctx, projectID, location)
	if err != nil {
		return nil, fmt.Errorf("failed to create genai client: %w", err)
	}
	defer client.Close()

	// Define the function tool for Gemini
	recordCatchTool := &genai.Tool{
		FunctionDeclarations: []*genai.FunctionDeclaration{{
			Name:        "record_catch",
			Description: "Records and validates a fish catch for the Murrabit Cod Challenge.",
			Parameters: &genai.Schema{
				Type: genai.TypeObject,
				Properties: map[string]*genai.Schema{
					"species":                 {Type: genai.TypeString, Description: "The species of the fish. Must be 'Murray Cod' or 'Golden Perch'."},
					"length_cm":               {Type: genai.TypeNumber, Description: "The total length of the fish in centimeters, measured on the official mat."},
					"competitor_card_visible": {Type: genai.TypeBoolean, Description: "Set to true if the competitor's entry card is clearly visible in the photo."},
					"is_valid":                {Type: genai.TypeBoolean, Description: "Set to true ONLY if the fish meets all competition rules."},
					"reason":                  {Type: genai.TypeString, Description: "If is_valid is false, provide a specific reason based on the rules."},
				},
				Required: []string{"species", "length_cm", "competitor_card_visible", "is_valid"},
			},
		}},
	}

	// Select the model and apply the tool
	model := client.GenerativeModel("gemini-1.5-flash-001") // Or "gemini-1.5-pro-001"
	model.Tools = []*genai.Tool{recordCatchTool}

	// Construct the prompt content
	prompt := genai.Text(`You are an official adjudicator for the Murrabit Cod Challenge. Your role is to meticulously validate photo submissions based on the official competition rules.

Official Competition Rules for Photo Validation:
1. Eligible Species: Only 'Murray Cod' and 'Golden Perch' (also known as 'Yellowbelly') are accepted.
2. Required Items: The photo MUST contain three things:
    * The entire fish on the official 2024 measure mat.
    * The competitor's entry card must be present and clearly legible.
    * The fish itself.
3. Photo Quality & Angle:
    * The photo must be taken from directly above the fish, looking straight down. Reject photos taken on a significant angle.
    * The photo must be clear, in focus, and well-lit. Reject any blurry, dark, or overexposed images where the length cannot be clearly determined.
4. Measurement Protocol:
    * The fish's mouth must be closed and touching the zero (0) mark on the mat.
    * Measure the total length in centimeters. The length is determined by the longest point of the fish's tail.

Your Adjudication Task:
Analyze the provided image and strictly apply all rules. Then, call the record_catch function with the information you extract.

Be specific in your reasoning for invalid catches. For example:
* If the card is missing, state 'Competitor entry card not visible.'
* If the mouth is not at zero, state 'Fish mouth not at zero mark.'
* If the species is wrong, state 'Invalid species: [detected species].'
* If the image is angled or blurry, state 'Image quality is poor (blurry or angled) and cannot be reliably measured.'`)

	// Create the parts of the request
	imgPart := genai.ImageData("jpeg", imageBytes)
	content := []genai.Part{prompt, imgPart}

	// Call the API
	resp, err := model.GenerateContent(ctx, content...)
	if err != nil {
		return nil, fmt.Errorf("failed to generate content: %w", err)
	}

	// Extract the function call and arguments
	if len(resp.Candidates) == 0 || len(resp.Candidates[0].Content.Parts) == 0 {
		return nil, fmt.Errorf("no response content from API")
	}

	part := resp.Candidates[0].Content.Parts[0]
	if fc, ok := part.(genai.FunctionCall); ok {
		if fc.Args == nil {
			return nil, fmt.Errorf("function call has no arguments")
		}

		// Convert map[string]interface{} to JSON bytes
		jsonArgs, err := json.Marshal(fc.Args)
		if err != nil {
			return nil, fmt.Errorf("failed to marshal function call args: %w", err)
		}

		// Unmarshal JSON into our struct
		var details CatchDetails
		if err := json.Unmarshal(jsonArgs, &details); err != nil {
			return nil, fmt.Errorf("failed to unmarshal args into CatchDetails struct: %w", err)
		}

		return &details, nil
	}

	// If we got here, the model didn't make a function call.
	// Log the text response for debugging.
	if txt, ok := part.(genai.Text); ok {
		return nil, fmt.Errorf("API returned text instead of a function call: %s", txt)
	}

	return nil, fmt.Errorf("API response did not contain a function call")
}
