APXS:=apxs
DESTDIR:=./install
LIBEXECDIR:=$(shell ${APXS} -q LIBEXECDIR)

.PHONY: default
default: compile

.PHONY: clean
clean:
	rm -f *.o *.la *.lo *.slo
	rm -rf .libs
	-case '$(DESTDIR)' in ''|/*) ;; *) rm -rf $(DESTDIR) ;; esac

.PHONY: install
install:
	install -D .libs/mod_auth_override.so ${DESTDIR}${LIBEXECDIR}/mod_auth_override.so

.PHONY: compile
compile:
	${APXS} -c mod_auth_override.c
