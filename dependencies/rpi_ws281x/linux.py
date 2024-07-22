#
# linux.py
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


import SCons
import string
import array
import os


tools = ['gcc', 'g++', 'gnulink', 'ar', 'gas']


def linux_tools(env):
    for tool in tools:
        env.Tool(tool)

    if not env['V']:
        env['ARCOMSTR']      = 'AR      ${TARGET}'
        env['ASCOMSTR']      = 'AS      ${TARGET}'
        env['CCCOMSTR']      = 'CC      ${TARGET}'
        env['CXXCOMSTR']     = 'C++     ${TARGET}'
        env['LINKCOMSTR']    = 'LINK    ${TARGET}'
        env['RANLIBCOMSTR']  = 'RANLIB  ${TARGET}'

def linux_flags(env):
    env.MergeFlags({
        'CPPFLAGS' : '''
            -fPIC
            -g
            -O2
            -Wall
            -Wextra
            -Werror
        '''.split(),
    }),
    env.MergeFlags({
        'LINKFLAGS' : '''
        '''.split()
    })


def linux_builders(env):
    env.Append(BUILDERS = {
        'Program' : SCons.Builder.Builder(
            action = SCons.Action.Action('${LINK} -o ${TARGET} ${SOURCES} ${LINKFLAGS}',
                                         '${LINKCOMSTR}'),
        ),
    })
    return 1


# The following are required functions by SCons when incorporating through tools
def exists(env):
    return 1

def generate(env, **kwargs):
    [f(env) for f in (linux_tools, linux_flags, linux_builders)]

