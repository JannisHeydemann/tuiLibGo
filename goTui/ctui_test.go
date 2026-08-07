package ctui

import (
	"math"
	"strings"
	"testing"
)

func TestVersionIsNonEmpty(t *testing.T) {
	if Version() == "" {
		t.Fatal("Version() returned an empty string")
	}
}

func TestRenderFontUsesRequestedFontChar(t *testing.T) {
	lines := RenderFont("H", '@')
	if len(lines) == 0 {
		t.Fatal("RenderFont returned no lines")
	}
	found := false
	for _, l := range lines {
		if strings.ContainsRune(l, '@') {
			found = true
		}
		if strings.ContainsRune(l, '#') {
			t.Errorf("unexpected hardcoded '#' in rendered line: %q", l)
		}
	}
	if !found {
		t.Error("expected the requested font char '@' to appear somewhere in the rendered glyph")
	}
}

func TestRenderFontEmptyTextReturnsBlankLines(t *testing.T) {
	lines := RenderFont("", '#')
	for i, l := range lines {
		if l != "" {
			t.Errorf("line %d: expected empty output for empty input text, got %q", i, l)
		}
	}
}

func TestNewEngineRejectsNonPositiveDimensions(t *testing.T) {
	cases := []struct{ w, h int }{
		{0, 10}, {10, 0}, {-1, 10}, {10, -1}, {0, 0},
	}
	for _, c := range cases {
		e, err := NewEngine(c.w, c.h)
		if err == nil {
			e.Close()
			t.Errorf("NewEngine(%d, %d): expected an error, got nil", c.w, c.h)
		}
	}
}

func TestNewEngineRejectsDimensionsThatOverflowCInt(t *testing.T) {
	huge := math.MaxInt32 + 1
	e, err := NewEngine(huge, 10)
	if err == nil {
		e.Close()
		t.Fatalf("NewEngine(%d, 10): expected an overflow error, got nil", huge)
	}
}

// Engine's methods guard on a nil handle so a zero-value Engine (or one
// whose Close() already ran) is always safe to call into again.
func TestZeroValueEngineMethodsAreNoOpsAndDoNotPanic(t *testing.T) {
	var e Engine
	e.Clear()
	e.DrawText(0, 0, "hi")
	e.DrawBox(0, 0, 5, 5, "hi", "#")
	e.Render()
	e.Close()
	e.Close() // double-close must also be safe
}
