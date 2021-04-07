#
# Makefile for FCNC analysis note
#
MAIN=fcncnote
DATE=`date +%Y%m%d`

TEX=$(wildcard *.tex)
BIB=$(wildcard *.bib)
PIC=$(wildcard pics/*.eps)
BST=$(wildcard *.bst)
STY=$(wildcard *.sty)

all: 	$(MAIN).pdf

%.pdf: %.ps
	@ps2pdf14 $<

%.ps: %.dvi
	@dvips -Ppdf -t letter -o $@ $<

%.dvi: %.tex $(TEX) $(BIB) $(PIC)
	@latex $(basename $<)
	@bibtex $(basename $<)
	@latex $(basename $<)
	@latex $(basename $<)

tar:
	@tar cvfz fcncnote_$(DATE).tar.gz Makefile $(TEX) $(BIB) $(PIC) $(BST) $(STY)

echo:
	@echo "MAIN=$(MAIN)"
	@echo "TEX=$(TEX)"
	@echo "BIB=$(BIB)"

clean:
	 @rm -f *bbl *log *aux fcncnote.ps fcncnote.pdf
