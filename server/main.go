package main

import (
	"bank_server/api"
	"bank_server/data"
	"fmt"
	"os"
	"strconv"
)

func main() {
	if len(os.Args) < 2 {
		fmt.Printf("Usage: %s <port> [drop_rate]\n", os.Args[0])
		return
	}
	port := os.Args[1]
	var dropRate float64 = 0.0
	if len(os.Args) >= 3 {
		if rate, err := strconv.ParseFloat(os.Args[2], 64); err == nil {
			dropRate = rate
		}
	}

	// Initialize the storage repository (in-memory sync maps)
	repo := data.NewAccountRepository()

	// Set up the routing multiplexer for business logic services
	router := api.NewRouter(repo, dropRate)

	// Initialize the networking listener daemon
	server := api.NewUDPServer(port, router, dropRate)

	// Begin listening for UDP requests indefinitely
	if err := server.Start(); err != nil {
		fmt.Printf("Error starting server: %v\n", err)
	}
}
