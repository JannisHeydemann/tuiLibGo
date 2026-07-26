// Command demo is a minimal smoke test for the ctui Go wrapper (goTui):
// prints the linked library version, then renders and prints a banner.
// Build the C++ library first (`make lib` from the repo root or `ctui/`)
// before running this with `go run .` (or `make demo`/`make run`).
package main

import (
	"fmt"

	ctui "tuilib/goTui"
)

func main() {
	fmt.Println("demo using ctui:", ctui.Version())
	print("ABCDEFG")
	fmt.Println("")
	print("HIJKLMN")
	fmt.Println("")
	print("OPQRSTU")
	fmt.Println("")
	print("VWXYZ")
	fmt.Println("")
	print("1234567890")
	fmt.Println("")
	print("!=?%&$#+*~")
}

func print(text string) {
	obj := ctui.RenderFont(text, '#')
	for _, curr := range obj {
		fmt.Println(curr)
	}
}
