DIST=misc-0.1

all: w64

w64:
	if [ ! -e w64 ] ; then mkdir w64 ; fi
	make dist -C ../
	rm -rf w64/$(DIST) w64/$(DIST).tar
	cp ../$(DIST).tar w64/$(DIST).tar
	cd w64 ; tar xf $(DIST).tar
	cp config.w64 w64/$(DIST)/config.args
	cd w64/$(DIST) ; ./configure
	$(MAKE) -C w64/$(DIST)

.PHONY: all w64
