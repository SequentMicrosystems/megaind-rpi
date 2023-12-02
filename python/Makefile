PYTHON        = python
SETUP_PY      = setup.py
DIST          = dist

.PHONY: all dist

all: dist
	@echo "Everything done!"
	@echo '  > Run "twine upload dist/*" to upload to pypi.org.'

dist:
	@echo "Creating python distribution."
	@rm -rf $(DIST)
	@$(PYTHON) $(SETUP_PY) sdist bdist_wheel 1>/dev/null
