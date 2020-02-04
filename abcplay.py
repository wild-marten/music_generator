#!/usr/bin/env python
# 
# Copyright (C) 2009-2010 W. Trevor King <wking@drexel.edu>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this program.  If not, see
# <http://www.gnu.org/licenses/>.

"""Play ABC_ files using abc2midi_ and timidity_.

For example::

    abcplay.py my_collection.abc:3:7:8 another_collection.abc

An example of using timidity options would be slowing down a tune
while you are learning::

    abcplay.py --timidity '--adjust-tempo 50' my_collection.abc:3

SIGINT (usually ^C) will kill the current tune and proceed the next
one.  Two SIGINTs in less than one second will kill the abcplay.py
process.  This should be familiar to users of mpg123_ or ogg123_.

You can also play LilyPond_ files (converted to MIDI via ``lilypond``)
by using the ``--ly`` option::

    abcplay.py --ly somefile.ly anotherfile.ly

.. _abc2midi: http://abc.sourceforge.net/abcMIDI/
.. _timidity: http://timidity.sourceforge.net/
.. _mpg123: http://www.mpg123.org/
.. _ogg123: http://www.vorbis.com
.. _LilyPond: http://lilypond.org/
"""

import shlex
from subprocess import Popen
from tempfile import mkstemp
from time import time
from os import remove
import os.path


class MIDIPlayer (object):
    def __init__(self, to_midi_program=None,
                 to_midi_options=None, timidity_options=None):
        f,self._tempfile = mkstemp(prefix='abcplay-', suffix='.midi')
        self._to_midi = [to_midi_program]
        if to_midi_options:
            self._to_midi.extend(to_midi_options)
        self._timidity = ['timidity']
        if timidity_options:
            self._timidity.extend(timidity_options)
        self._last_interrupt = 0
        self._p = None

    def cleanup(self):
        remove(self._tempfile)

    def play_files(self, filenames):
        raise NotImplementedError()

    def _return_after_interrupt(self):
        t = time()
        ret = t-self._last_interrupt < 1
        self._last_interrupt = t
        return ret

    def play(self, filename, **kwargs):
        self._convert_to_midi(filename, **kwargs)
        self._play_midi()

    def _convert_to_midi(self, filename, **kwargs):
        raise NotImplementedError()

    def _play_midi(self):
        self._p = Popen(self._timidity + [self._tempfile])
        self._p.wait()
        self._p = None

    def _kill_p(self):
        if self._p != None:
            try:
                self._p.terminate()
            except OSError, e:
                if e.errno == 3:
                    pass  # no such process
                else:
                    raise
            else:
                self._p.wait()
            self._p = None


class ABCPlayer (MIDIPlayer):
    refnum_sep = ':'

    def __init__(self, abc2midi_options=None, timidity_options=None):
        super(ABCPlayer, self).__init__(
            'abc2midi', abc2midi_options, timidity_options)

    def play_files(self, filenames):
        for filename in filenames:
            if self.refnum_sep in filename:
                fields = filename.split(self.refnum_sep)
                filename = fields[0]
                refnums = fields[1:]
            else:
                refnums = list(self._refnums(filename))
            while len(refnums) > 0:
                refnum = refnums.pop(0)
                try:
                    self.play(filename, refnum=refnum)
                except KeyboardInterrupt:
                    self._kill_p()
                    if self._return_after_interrupt():
                        return

    def _refnums(self, filename):
        with open(filename, 'r') as f:
            for line in f:
                if line.startswith('X:'):
                    yield int(line[len('X:'):])

    def _convert_to_midi(self, filename, refnum):
        self._p = Popen(
            self._to_midi[:1] + [filename, str(refnum)] +
            self._to_midi[1:] + ['-o', self._tempfile])
        self._p.wait()
        self._p = None


class LilyPondPlayer (MIDIPlayer):
    def __init__(self, lilypond_options=None, timidity_options=None):
        default_lilypond_options = [
            '-dbackend=null',  # don't create a typeset version
            '-ddelete-intermediate-files',  # clean up after ourselves
            ]
        if lilypond_options:
            lilypond_options = default_lilypond_options + lilypond_options
        else:
            lilypond_options = default_lilypond_options
        super(LilyPondPlayer, self).__init__(
            'lilypond', lilypond_options, timidity_options)

    def play_files(self, filenames):
        for filename in filenames:
            try:
                self.play(filename)
            except KeyboardInterrupt:
                self._kill_p()
                if self._return_after_interrupt():
                    return

    def _convert_to_midi(self, filename):
        ofilename,ext = os.path.splitext(self._tempfile)
        assert ext == '.midi', ext  # lilypond adds suffix automatically
        #-dmidi-extension=midi
        self._p = Popen(
            self._to_midi + ['-o', ofilename, filename])
        self._p.wait()
        self._p = None


if __name__ == '__main__':
    import sys
    import optparse

    usage = '%prog [options] file[:refnum[:refnum:...]] ...'
    epilog = __doc__
    p = optparse.OptionParser(usage=usage, epilog=epilog)
    p.format_epilog = lambda formatter: epilog+'\n'
    p.add_option('-a', '--abc2midi', dest='abc2midi',
                 help='Extra options to pass to abc2midi.')
    p.add_option('-t', '--timidity', dest='timidity',
                 help='Extra options to pass to timidity.')
    p.add_option('-l', '--ly', dest='use_lilypond', action='store_true',
                 help='Use LilyPond input instead of ABC.')
    p.add_option('--lilypond', dest='lilypond',
                 help='Extra options to pass to LilyPond.')
    options,args = p.parse_args()
    del p

    abc2midi = options.abc2midi
    if abc2midi:
        abc2midi = shlex.split(abc2midi)
    timidity = options.timidity
    if timidity:
        timidity = shlex.split(timidity)
    lilypond = options.lilypond
    if lilypond:
        lilypond = shlex.split(lilypond)

    if options.use_lilypond:
        p = LilyPondPlayer(lilypond, timidity)
    else:
        p = ABCPlayer(abc2midi, timidity)
    try:
        p.play_files(args)
    finally:
        p.cleanup()
