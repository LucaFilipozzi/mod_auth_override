# How To Build an RPM Package

* determine which release, X.Y, to package
* extract the release with `git archive --format tar --prefix mod_auth_override-X.Y/ release-X.Y | gzip > ~/rpmbuild/SOURCES/mod_auth_override-X.Y.tar.gz`
* copy the template, mod_auth_override.spec to ~/rpmbuild/SPECS/mod_auth_override-X.Y.spec
* edit ~/rpmbuild/SPECS/mod_auth_override-X.Y.spec, changing X.Y internally as necessary
* copy the configuration file, auth_override.conf to ~/rpmbuild/SOURCES/auth_override.conf
* build the source and binary packages with `rpmbuild -ba ~/rpmbuild/SPECS/mod_auth_override-X.Y.spec`
