// Command demo is a minimal smoke test for the ctui Go wrapper (goTui):
// prints the linked library version, renders font banners, and tests
// the full C++ TUI rendering engine in raw mode.
//
// Build the C++ library first (`make lib` from the repo root)
// before running this with `go run main.go` (or `make demo`).
package main

import (
	"fmt"
	"time"

	ctui "tuilib/goTui"
)

func main() {
	fmt.Println("demo using ctui version:", ctui.Version())

	// 1. Quick test of your static ASCII font rendering
	fmt.Println("\n--- Testing Font Renderer ---")
	printFontBanner("CTUI", '#')

	// Short pause so you can see the font test output before entering raw mode
	time.Sleep(2 * time.Second)

	// 2. Full TUI Engine Test (Raw Mode + Canvas Grid)
	fmt.Println("\nStarting TUI Engine...")

	// Create engine with a 80x24 canvas grid.
	// This automatically enters the alternate screen buffer and raw mode!
	app, err := ctui.NewEngine(80, 24)
	if err != nil {
		fmt.Printf("Error starting TUI engine: %v\n", err)
		return
	}
	// VERY IMPORTANT: defer Close() so the terminal is restored
	// even if the program panics!
	defer app.Close()

	// Clear back-buffer
	app.Clear()

	// Draw decorative borders and simple text
	app.DrawText(2, 1, "==========================================================================")
	app.DrawText(2, 2, "   CTUI ENGINE DEMO — C++ Canvas + Go Cgo Wrapper")
	app.DrawText(2, 3, "==========================================================================")

	app.DrawText(4, 6, "Hello from Go via Cgo!")
	app.DrawText(4, 7, "This frame is drawn onto a 1D C++ grid array and flushed to stdout.")

	// Render your ASCII Font banner DIRECTLY inside the 2D TUI Canvas!
	bannerLines := ctui.RenderFont("GO + C++", '*')
	for i, line := range bannerLines {
		app.DrawText(4, 10+i, line)
	}

	app.DrawText(4, 20, "Exiting in 3 seconds...")
	app.DrawText(2, 22, "==========================================================================")

	// Flush the back-buffer to the physical terminal screen
	app.Render()

	// Hold the screen so you can inspect your live TUI layout
	time.Sleep(3 * time.Second)

	// When main() finishes, app.Close() fires, restoring standard canonical mode cleanly.
}

// Helper to print font banners directly to stdout (for standard terminal mode)
func printFontBanner(text string, fontChar byte) {
	obj := ctui.RenderFont(text, fontChar)
	for _, curr := range obj {
		fmt.Println(curr)
	}
}
