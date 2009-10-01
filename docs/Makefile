all:	icse_ant.pdf

icse_ant.pdf:	index.tex tex/*
	pdflatex index
	bibtex index
	pdflatex index
	pdflatex index
	mv index.pdf icse_ant.pdf

clean:
	rm -f *pdf *aux *toc *nav *log *snm *dvi *ps *out *vrb
