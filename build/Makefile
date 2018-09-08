DIST=misc-0.1

all: w64

w64:
	make dist -C ../
	cp ../$(DIST).tar w64
	rm w64/$(DIST) -rf
	cd w64 ; tar xf $(DIST).tar
	cp config.w64 w64/$(DIST)/config.args
	cd w64/$(DIST) ; ./configure
	$(MAKE) -C w64/$(DIST)

.PHONY: all w64
