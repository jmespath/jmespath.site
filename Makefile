help:
	@echo 'Makefile for a pelican Web site                                        '
	@echo '                                                                       '
	@echo 'Usage:                                                                 '
	@echo '   make publish                     upload to s3 bucket '


publish:
	$(MAKE) -C docs/ publish

.PHONY: help publish
