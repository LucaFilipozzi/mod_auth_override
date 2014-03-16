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

It is critical, from a security perspective, that SOME\_HEADER be set by an
AuthN or AuthZ provider such as [mod_auth_cas][2] rather than passed in by the
client.

## a CAS example

RequestHeaderPlus, from [mod_setenvifplus][1], is used in the example above
solely to demonstrate the use of AuthOverride.

It is assumed that the request header used with AuthOverride will be set by
[mod_auth_cas][2].

For example, suppose that the orginal value of REMOTE\_USER as set by
[mod_auth_cas][2] might be 'alice' but that the value desired by the
application is 'alice@example.com' and that this desired value is available as
CAS\_eduPersonPrincipalName in the request headers; then the configuration
could be:

```apache
<IfModule mod_auth_cas.c>
    # ... other CAS directives ...
    CASValidateSAML on
    CASAuthoritative on
    CASAttributePrefix CAS_
</IfModule>

<VirtualHost www.example.com:443>
    <IfModule mod_auth_cas.c>
        <Location "/secure">
            AuthType CAS
            CASScrubRequestHeaders on
            CASAuthNHeader CAS_user
            Require cas-attribute eduPersonPrincipalName~.+
            AuthOverride CAS_eduPersonPrincipalName
        </Location>
    </IfModule>
</VirtualHost>
```

Use of the combination of directives 'CASScrubRequestHeaders' and 'Require
cas-attribute' ensures that the header used with AuthOverride was, in fact, set
by [mod_auth_cas][2].

*PLEASE NOTE* that a recent version of [mod_auth_cas][2] is required for
'Require cas-attribute' functionality: one that supports the 'CASAuthoritative'
directive.

Patches providing rpm or deb packaging welcome.

[0]: http://httpd.apache.org/docs/2.2/mod/mod_authn_file.html
[1]: http://opensource.adnovum.ch/mod_setenvifplus
[2]: https://github.com/Jasig/mod_auth_cas

