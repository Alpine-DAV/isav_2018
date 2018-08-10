
SHELL = /bin/sh

all: isav_ascent_triggers.pdf

isav_ascent_triggers.pdf: *.tex *.bib *.inc
	pdflatex isav_ascent_triggers
	pdflatex isav_ascent_triggers
	bibtex isav_ascent_triggers
	#biber isav_ascent_triggers
	touch isav_ascent_triggers.tex
	pdflatex isav_ascent_triggers
	pdflatex isav_ascent_triggers


clean:
	if (rm -f *.log *.aux *.bbl *.blg *.toc *.dvi *.ps *.brf *.lbl *.gz isav_ascent_triggers.pdf) then :; fi

