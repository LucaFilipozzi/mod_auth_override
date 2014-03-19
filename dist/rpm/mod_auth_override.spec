Name:           mod_auth_override
Version:        X.Y
Release:        1%{?dist}
Summary:        Apache 2.2-compliant module that supports overriding REMOTE_USER

Group:          System Environment/Daemons
License:        GPLv3+ with exceptions
URL:            https://github.com/LucaFilipozzi/mod_auth_override
# git archive --format tar --prefix mod_auth_override-X.Y/ release-X.Y | gzip > ../mod_auth_override-X.Y.tar.gz
Source0:        mod_auth_override-X.Y.tar.gz
Source1:        auth_override.conf

BuildRoot:      %(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)

BuildRequires:  httpd-devel

Requires:       httpd

%description
mod_auth_override is an Apache 2.2-compliant module that supports overriding REMOTE_USER

%prep
%setup -q

%build
make APXS=/usr/sbin/apxs %{?_smp_mflags}

%install
rm -rf %{buildroot}
make install DESTDIR=%{buildroot}/
mkdir -p %{buildroot}/%{_sysconfdir}/httpd/conf.d
install -p -m 644 %{SOURCE1} %{buildroot}%{_sysconfdir}/httpd/conf.d/auth_override.conf

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root,-)
%{_libdir}/httpd/modules/*.so
%config(noreplace) %{_sysconfdir}/httpd/conf.d/*.conf

%changelog
