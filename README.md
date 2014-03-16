# mod_auth_override

## installation

Ensure that apxs (Redhat) / apxs2 (Debian) is installed.

```bash
apxs2 -c -i -a mod_auth_override.c
```

## configuration

Where needed, add the AuthOverride directive to cause REMOTE_USER to be
updated.

```apache
<Directory "/srv/www.example.com/htdocs">
    AuthType Basic
    AuthName "Authentication Required"
    AuthUserFile "/srv/www.example.com/etc/htpasswd"
    Require valid-user
    RequestHeaderPlus set SOME_HEADER bob
    AuthOverride SOME_HEADER
</Directory>
```

Suppose that the 'htpasswd' file contains an entry for a user 'alice'.

After successful authentication (based on the standard behaviour of
[mod_authn_file][0]), REMOTE\_USER is set to 'alice'.

This module, mod_auth_override, implements a fixhup hook that will update
REMOTE\_USER to the value of the request header, SOME\_HEADER (in this case,
'bob').

It is critical, from a security perspective, that SOME_HEADER be set by an
AuthN or AuthZ provider such as [mod_auth_cas][3] rather than passed in by the
client.

## notes

RequestHeaderPlus, from [mod_setenvifplus][1], is used in the example above
solely to demonstrate the use of AuthOverride.

It is assumed that the request header used with AuthOverride will be set by
[mod_shib2][2] or [mod_auth_cas][3].

For example, suppose the orginal value of REMOTE_USER might be 'alice' as set
by [mod_auth_cas][3] but the desired value by the application is
'alice@example.com' and this value is available as CAS\_eduPersonPrincipalName
in the request headers; then the configuration could be:

```apache
<Location "/secure">
    AuthType CAS
    # ... more CAS stuff ...
    Require valid-user
    Require cas-attribute eduPersonPrincipalName~.*
    AuthOverride CAS_eduPersonPrincipalName
</Location>
```

Use of the 'Require cas-attribute' directive helps to ensure that the header
used with AuthOverride was, in fact, set by [mod_auth_cas][3].

Patches providing rpm or deb packaging welcome.

[0]: http://httpd.apache.org/docs/2.2/mod/mod_authn_file.html
[1]: http://opensource.adnovum.ch/mod_setenvifplus
[2]: http://www.internet2.edu/products-services/trust-identity-middleware/shibboleth/
[3]: https://github.com/Jasig/mod_auth_cas

