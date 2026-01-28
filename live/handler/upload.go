// Package handler provides web handlers
package handler

import (
	"encoding/json"
	"io"
	"live/ai"
	"log"
	"net/http"
	"os"

	"github.com/gorilla/mux"
)

// Assuming ValidateCatchPhoto is in the main package.
// If you move it to its own package (e.g., 'gemini'), adjust the import path.
// func ValidateCatchPhoto(ctx context.Context, imageBytes []byte, projectID, location string) (*CatchDetails, error)

// UploadResponse defines the structure of the JSON response.
type UploadResponse struct {
	Success bool        `json:"success"`
	Data    interface{} `json:"data"`
	Error   string      `json:"error,omitempty"`
}

// writeJSONResponse is a helper to write JSON responses.
func writeJSONResponse(w http.ResponseWriter, status int, data interface{}) {
	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(status)
	if err := json.NewEncoder(w).Encode(data); err != nil {
		log.Printf("Error writing JSON response: %v", err)
	}
}

// UploadHandler handles the image upload and validation.
// It expects a multipart form with a file field named "photo".
func UploadHandler(w http.ResponseWriter, r *http.Request) {
	// 1. Get Project and Location from environment variables
	projectID := os.Getenv("GCP_PROJECT_ID")
	location := os.Getenv("GCP_LOCATION")
	if projectID == "" || location == "" {
		log.Println("Error: GCP_PROJECT_ID and GCP_LOCATION environment variables must be set.")
		writeJSONResponse(w, http.StatusInternalServerError, UploadResponse{
			Success: false,
			Error:   "Server is not configured correctly.",
		})
		return
	}

	// 2. Parse the multipart form
	// Let's set a reasonable limit for the upload size, e.g., 10 MB.
	if err := r.ParseMultipartForm(10 << 20); err != nil {
		writeJSONResponse(w, http.StatusBadRequest, UploadResponse{
			Success: false,
			Error:   "Could not parse multipart form.",
		})
		return
	}

	// 3. Get the file from the form
	file, _, err := r.FormFile("photo")
	if err != nil {
		writeJSONResponse(w, http.StatusBadRequest, UploadResponse{
			Success: false,
			Error:   "Invalid file field. Make sure the key is 'photo'.",
		})
		return
	}
	defer file.Close()

	// 4. Read the file into a byte slice
	imageBytes, err := io.ReadAll(file)
	if err != nil {
		writeJSONResponse(w, http.StatusInternalServerError, UploadResponse{
			Success: false,
			Error:   "Could not read uploaded file.",
		})
		return
	}

	// 5. Call the Gemini validation function
	// We pass the request context to the validation function.
	// This is good practice for handling timeouts and cancellations.
	catchDetails, err := ai.ValidateCatchPhoto(r.Context(), imageBytes, projectID, location)
	if err != nil {
		log.Printf("Gemini validation failed: %v", err)
		writeJSONResponse(w, http.StatusInternalServerError, UploadResponse{
			Success: false,
			Error:   "Failed to validate image: " + err.Error(),
		})
		return
	}

	// 6. Return the successful validation details
	writeJSONResponse(w, http.StatusOK, UploadResponse{
		Success: true,
		Data:    catchDetails,
	})
}

// RegisterRoutes sets up the routes for the handlers.
// You would call this from your main function.
func RegisterRoutes(r *mux.Router) {
	r.HandleFunc("/upload", UploadHandler).Methods("POST")
}
