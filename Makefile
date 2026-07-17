CTUI_DIR := $(CURDIR)/ctui
DEMO_DIR := $(CURDIR)/demo

.PHONY: clean lib demo run clean

lib:
	$(MAKE) -C $(CTUI_DIR) lib

demo: lib
	cd $(DEMO_DIR) && go run .

run: demo

clean:
	$(MAKE) -C $(CTUI_DIR) clean
