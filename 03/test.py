import subprocess
import time

def run(command):
    process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    out = process.stdout.readlines()
    code = process.wait()
    return code, out

def test(command, expected_code, expected_value):
    print command
    code, out = run(command)
    if code != expected_code:
        print 'return value', expected_code, '(expected) !=', code
        return
    i = 0
    for line in out:
        try:
            if line.rstrip() != expected_value[i]:
                print expected_value[i], '(expected) !=', line.rstrip()
                return
            i += 1
        except ValueError:
            print 'invalid output'
            return
        except IndexError:
            print 'invalid output'
            return
    if i != len(expected_value):
        print 'empty output'
        return
    print 'ok'

#. = successful allocate
#! = unsuccessful allocate, nullptr return
#r = memory was reset
test('./run 16 8 8', 0, [ '..' ])
test('./run 16 8 9', 0, [ '.!' ])
test('./run 2 1 1 1 reset 1 1 1', 0, [ '..!r..!' ])

test('./run ', 1, [ 'no parameters' ])
test('./run abc', 1, [ 'incorrect maxSize' ])
test('./run 0', 1, [ 'incorrect maxSize' ])
test('./run 5 0', 1, [ 'incorrect size or unknown command' ])
test('./run 5 abc', 1, [ 'incorrect size or unknown command' ])