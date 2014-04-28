S3_BUCKET=jmespath.org
BUILD_DIR=docs/_build/html/


help:
	@echo 'Makefile for a pelican Web site                                        '
	@echo '                                                                       '
	@echo 'Usage:                                                                 '
	@echo '   make publish                     upload to s3 bucket '


publish:
	aws s3 sync $(BUILD_DIR)/ s3://$(S3_BUCKET) --acl public-read --delete --region us-west-2 --profile jmespath-deployer
	@echo 'Site uploaded to s3://$(S3_BUCKET)'

.PHONY: help publish
