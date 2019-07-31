tests:
	for i in *Test/Makefile ; do make -C $$(dirname $$i) -j ; done

clean:
	for i in *Test/Makefile ; do echo $$i ; make -C $$(dirname $$i) clean ; done

runtests:
	for i in *Test/Makefile ; do $$(dirname $$i)/$$(dirname $$i).out ; done
