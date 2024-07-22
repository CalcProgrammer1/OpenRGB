#
# SConstruct
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


import os


opts = Variables()
opts.Add(BoolVariable('V',
                      'Verbose build',
                      False))

opts.Add('TOOLCHAIN',
         'Set toolchain for cross compilation (e.g. arm-linux-gnueabihf)',
         '')

platforms = [ 
    [
        'userspace',            # Target Name
        [ 'linux', 'version' ], # Scons tool (linux, avr, etc.)
        {                       # Special environment setup
            'CPPPATH' : [
            ],
            'LINKFLAGS' : [
                "-lrt",
                "-lm",
            ],
        },
    ], 
]

clean_envs = {}
for platform, tool, flags in platforms:
    env = Environment(
        options = opts,
        tools = tool,
        toolpath = ['.'],
        ENV = {'PATH' : os.environ['PATH']},
        LIBS = [],
    )
    env.MergeFlags(flags)
    clean_envs[platform] = env

Help(opts.GenerateHelpText(clean_envs))

if env['TOOLCHAIN'] != '':
    env['CC'] = env['TOOLCHAIN'] + '-gcc'
    env['AR'] = env['TOOLCHAIN'] + '-ar'

Export(['clean_envs'])
SConscript('SConscript');

