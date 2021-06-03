%global _name OpenRGB

Name:           openrgb
Version:        0.6.1
Release:        1%{?dist}
Summary:        Open source RGB lighting control that doesn't depend on manufacturer software

License:        GPLv2
URL:            https://gitlab.com/CalcProgrammer1/%{_name}
Source0:        OpenRGB.tar.gz

BuildRequires:  gcc-c++ libusb-devel libstdc++-devel qt5-qtbase-devel desktop-file-utils hidapi-devel mbedtls-devel
Requires:       hicolor-icon-theme

%description
Open source RGB lighting control that doesn't depend on manufacturer software. Supports Windows and Linux.

ASUS, ASRock, Corsair, G.Skill, Gigabyte, HyperX, MSI, Razer, ThermalTake, and more supported

%prep
cp %{_sourcedir}/%{_name}/* %{_builddir} -r

%build
cd %{_builddir}
%qmake_qt5 PREFIX=%{_prefix}
%make_build

%install
%make_install INSTALL_ROOT=%{buildroot}

#icon
install -Dpm 644 qt/%{_name}.png %{buildroot}%{_datadir}/icons/hicolor/128x128/apps/%{_name}.png

#desktop
desktop-file-install %{_sourcedir}/%{_name}/qt/%{_name}.desktop

%post -n %{name}
if [ -S /run/udev/control ]; then
    udevadm control --reload
    udevadm trigger
fi

%files
%{_bindir}/%{name}
%{_datadir}/icons/hicolor/128x128/apps/%{_name}.png
%{_datadir}/applications/%{_name}.desktop
%{_datadir}/pixmaps/%{_name}.png
/lib/udev/rules.d/60-%{name}.rules
%license LICENSE
%doc README.md

%changelog
* Tue Jun 1 2021 Adam Honse <calcprogrammer1@gmail.com> - 0.6.1-0
- Updated to 0.6-1

* Mon May 31 2021 Adam Honse <calcprogrammer1@gmail.com> - 0.6-0
- Updated to 0.6

* Mon May 03 2021 Pol Rius <alpemwarrior@gmail.com> - 0.5-0
- Modified to build in Gitlab's CI

* Mon Dec 07 2020 Artem S. Tashkinov <aros gmx com> - 0.5-0
- Updated to 0.5

* Mon Sep 28 2020 Artem S. Tashkinov <aros gmx com> - 0.4-0
- Updated to 0.4

* Sat May 16 2020 Carlos Mogas da Silva <r3pek@r3pek.org> - 0.2-2
- Applied review changes in https://bugzilla.redhat.com/show_bug.cgi?id=1835958#c1

* Thu May 14 2020 Carlos Mogas da Silva <r3pek@r3pek.org> - 0.2-1
- Initial import
