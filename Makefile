.PHONY: default
default: compile

.PHONY: clean
clean:
	rm -f *.la *.lo *.slo
	rm -rf .libs

.PHONY: activate
activate:
	apxs2 -c -i -a mod_auth_override.c

.PHONY: install
install:
	apxs2 -c -i mod_auth_override.c

.PHONY: compile
compile:
	apxs2 -c mod_auth_override.c
