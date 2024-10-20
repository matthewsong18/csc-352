setup :
	# Create assignment assg folder or skip if exists
	mkdir -p $(assg)

	# Make prob# folders for var prob 
	for i in $(shell seq 1 $(prob)); do \
		mkdir -p $(assg)/prob$$i; \
		touch $(assg)/prob$$i/Makefile; \
		mkdir -p $(assg)/prob$$i/tests; \
		mkdir -p $(assg)/prob$$i/tests/outputs; \
		touch $(assg)/prob$$i/tests/outputs/.git-keep-directory; \
	done
