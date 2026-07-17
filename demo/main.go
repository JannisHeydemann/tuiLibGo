package main

import (
	"fmt"

	ctui "tuilib/goTui"
)

func main() {
	fmt.Println("demo using ctui:", ctui.Version())
	obj := ctui.RenderFont("Jannis!", '.')
	for _, curr := range obj {
		fmt.Println(curr)
	}
}
