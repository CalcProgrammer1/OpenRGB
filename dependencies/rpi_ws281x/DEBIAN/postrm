#!/bin/sh

set -e

# summary of how this script can be called:
#        * <postinst> `configure' <most-recently-configured-version>
#        * <old-postinst> `abort-upgrade' <new version>
#        * <conflictor's-postinst> `abort-remove' `in-favour' <package>
#          <new-version>
#        * <postinst> `abort-remove'
#        * <deconfigured's-postinst> `abort-deconfigure' `in-favour'
#          <failed-install-package> <version> `removing'
#          <conflicting-package> <version>
# for details, see http://www.debian.org/doc/debian-policy/ or
# the debian-policy package

# source debconf library
. /usr/share/debconf/confmodule

# Source dbconfig-common functions
if [ -f /usr/share/dbconfig-common/dpkg/postinst.pgsql  ]; then
  . /usr/share/dbconfig-common/dpkg/postinst.pgsql
fi

case "$1" in

  remove)
    ldconfig
  ;;

  abort-upgrade|abort-remove|abort-deconfigure)
    exit 0
  ;;

  *)
    echo "postrm called with unknown argument \`$1'" >&2
    exit 1
  ;;

esac

db_stop

exit 0
~               
