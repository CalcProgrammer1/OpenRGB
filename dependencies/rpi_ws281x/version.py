#
# SConstruct
#
# Copyright (c) 2016 Jeremy Garff <jer @ jers.net>
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

import SCons, os

def version_flags(env):
    if not env['V']:
        env['VERSIONCOMSTR'] = 'Version ${TARGET}'

def version_builders(env):
    def generate_version_header(target, source, env):
        headername = os.path.basename(target[0].abspath)
        headerdef = headername.replace('.', '_').replace('-', '_').upper()

        try:
            version = open(source[0].abspath, 'r').readline().strip().split('.')
        except:
            version = [ '0', '0', '0' ]

        f = open(headername, 'w')
        f.write('/* Auto Generated Header built by version.py - DO NOT MODIFY */\n')
        f.write('\n')
        f.write('#ifndef __%s__\n' % (headerdef))
        f.write('#define __%s__\n' % (headerdef))
        f.write('\n')
        f.write('#define VERSION_MAJOR %s\n' % version[0])
        f.write('#define VERSION_MINOR %s\n' % version[1])
        f.write('#define VERSION_MICRO %s\n' % version[2])
        f.write('\n')
        f.write('#endif /* __%s__ */\n' % (headerdef))
        f.close()

    env.Append(BUILDERS = {
        'Version' : SCons.Builder.Builder(
            action = SCons.Action.Action(generate_version_header, '${VERSIONCOMSTR}'),
            suffix = '.h',
        ),
    })

def exists(env):
    return 1

def generate(env, **kwargs):
    [f(env) for f in (version_flags, version_builders)]


