#files_tex = $(wildcard *.tex)
file_name = "paper1"
file_tex = "paper1.tex"

all: pdf 
	@echo "Done!"

pdf: *.tex 
	@echo "Building.... $^"
	@$(foreach var,$(file_tex),pdflatex -interaction=nonstopmode -jobname=$(file_name) '$(var)' 1>/dev/null;)

bib: *.bib 
	@echo "Building.... $^"
	@$(foreach var,$(file_tex),bibtex -interaction=nonstopmode -jobname=$(file_name) '$(var)' 1>/dev/null;)]

clean:
	@rm -f *.aux *.toc *.log *~ *.spl *.blg *.bbl
	@echo "Clean done.";
debug:
	@echo "Finding Latex errors..."
	@pdflatex $(file_tex)
