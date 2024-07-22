#
# SConscript
#
# Copyright (c) 2014 Jeremy Garff <jer @ jers.net>
#
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification, are permitted
# provided that the following conditions are met:
#
#     1.  Redistributions of source code must retain the above copyright notice, this list of
#         conditions and the following disclaimer.
#     2.  Redistributions in binary form must reproduce the above copyright notice, this list
#         of conditions and the following disclaimer in the documentation and/or other materials
#         provided with the distribution.
#     3.  Neither the name of the owner nor the names of its contributors may be used to endorse
#         or promote products derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
# FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
# OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
# OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#


Import(['clean_envs'])

tools_env = clean_envs['userspace'].Clone()


# Build Library
lib_srcs = Split('''
    mailbox.c
    ws2811.c
    pwm.c
    pcm.c
    dma.c
    rpihw.c
''')

version_hdr = tools_env.Version('version')
ws2811_lib = tools_env.Library('libws2811', lib_srcs)
tools_env['LIBS'].append(ws2811_lib)

# Shared library (if required)
ws2811_slib = tools_env.SharedLibrary('libws2811', lib_srcs)

# Test Program
srcs = Split('''
    main.c
''')

objs = []
for src in srcs:
   objs.append(tools_env.Object(src))

test = tools_env.Program('test', objs + tools_env['LIBS'])

Default([test, ws2811_lib])

package_version = "1.1.0-1"
package_name = 'libws2811_%s' % package_version

debian_files = [
    'DEBIAN/control',
    'DEBIAN/postinst',
    'DEBIAN/prerm',
    'DEBIAN/postrm',
]

package_files_desc = [
    [ '/usr/lib', ws2811_slib ],
]

package_files = []
for target in package_files_desc:
    package_files.append(tools_env.Install(package_name + target[0], target[1]))

for deb_file in debian_files:
    package_files.append(
        tools_env.Command('%s/%s' % (package_name, deb_file), deb_file, [
            Copy("$TARGET", "$SOURCE"),
            Chmod("$TARGET", 0o755)
        ])
    )

package = tools_env.Command('%s.deb' % package_name, package_files,
                            'cd %s; dpkg-deb --build %s' % (Dir('.').abspath, package_name));

Alias("deb", package)

