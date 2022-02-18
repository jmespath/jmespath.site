help:
	@echo 'Makefile for a pelican Web site                                        '
	@echo '                                                                       '
	@echo 'Usage:                                                                 '
	@echo '   make clean                                                          '
	@echo '   make html                                                           '


clean:
	$(MAKE) -C documentation/ clean

html:
	$(MAKE) -C documentation/ html

doclint:
	find . -type f -name "*.rst" | xargs doc8

