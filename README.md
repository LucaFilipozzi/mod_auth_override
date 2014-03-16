# mod_auth_override

## installation

Ensure that apxs (Redhat) / apxs2 (Debian) is installed.

```bash
apxs2 -c -i -a mod_auth_override.c
```

## configuration

Where needed, add the AuthOverride directive to cause REMOTE_USER to be updated.

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
mod_authn_file), REMOTE_USER is set to 'alice'.

This module, mod_auth_override, implements an fixhup hook that will update
REMOTE_USER to the value of the request header, SOME_HEADER (in this case,
'bob').

## notes

RequestHeaderPlus, from [mod_setenvifplus][1], is used in the example above
solely to demonstrate the use of AuthOverride.  It is assumed that the request
header used with AuthOverride will be set by mod_shib2 or mod_auth_cas.

Patches providing rpm or deb packaging welcome.

[1]: http://opensource.adnovum.ch/mod_setenvifplus

