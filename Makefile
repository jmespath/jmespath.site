help:
	@echo 'Makefile for a pelican Web site                                        '
	@echo '                                                                       '
	@echo 'Usage:                                                                 '
	@echo '   make publish                     upload to s3 bucket '


publish:
	$(MAKE) -C docs/ publish

clean:
	$(MAKE) -C docs/ clean

html:
	$(MAKE) -C docs/ html

doclint:
	find . -type f -name "*.rst" | xargs doc8


.PHONY: help publish
