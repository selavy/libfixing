#!/usr/bin/env python

import argparse
import random
import math
import pprint
from ctypes import c_uint32


PRIMES = [
     98641,  98663,  98669,  98689,  98711,  98713,  98717,  98729,  98731,  98737,
     98773,  98779,  98801,  98807,  98809,  98837,  98849,  98867,  98869,  98873,
     98887,  98893,  98897,  98899,  98909,  98911,  98927,  98929,  98939,  98947,
     98953,  98963,  98981,  98993,  98999,  99013,  99017,  99023,  99041,  99053,
     99079,  99083,  99089,  99103,  99109,  99119,  99131,  99133,  99137,  99139,
     99149,  99173,  99181,  99191,  99223,  99233,  99241,  99251,  99257,  99259,
     99277,  99289,  99317,  99347,  99349,  99367,  99371,  99377,  99391,  99397,
     99401,  99409,  99431,  99439,  99469,  99487,  99497,  99523,  99527,  99529,
     99551,  99559,  99563,  99571,  99577,  99581,  99607,  99611,  99623,  99643,
     99661,  99667,  99679,  99689,  99707,  99709,  99713,  99719,  99721,  99733,
     99761,  99767,  99787,  99793,  99809,  99817,  99823,  99829,  99833,  99839,
     99859,  99871,  99877,  99881,  99901,  99907,  99923,  99929,  99961,  99971,
     99989,  99991, 100003, 100019, 100043, 100049, 100057, 100069, 100103, 100109,
    100129, 100151, 100153, 100169, 100183, 100189, 100193, 100207, 100213, 100237,
    100267, 100271, 100279, 100291, 100297, 100313, 100333, 100343, 100357, 100361,
    100363, 100379, 100391, 100393, 100403, 100411, 100417, 100447, 100459, 100469,
    100483, 100493, 100501, 100511, 100517, 100519, 100523, 100537, 100547, 100549,
    100559, 100591, 100609, 100613, 100621, 100649, 100669, 100673, 100693, 100699,
    100703, 100733, 100741, 100747, 100769, 100787, 100799, 100801, 100811, 100823,
    100829, 100847, 100853, 100907, 100913, 100927, 100931, 100937, 100943, 100957,
    100981, 100987, 100999, 101009, 101021, 101027, 101051, 101063, 101081, 101089,
    101107, 101111, 101113, 101117, 101119, 101141, 101149, 101159, 101161, 101173,
    101183, 101197, 101203, 101207, 101209, 101221, 101267, 101273, 101279, 101281,
    101287, 101293, 101323, 101333, 101341, 101347, 101359, 101363, 101377, 101383,
    101399, 101411, 101419, 101429, 101449, 101467, 101477, 101483, 101489, 101501,
    101503, 101513, 101527, 101531, 101533, 101537, 101561, 101573, 101581, 101599,
    101603, 101611, 101627, 101641, 101653, 101663, 101681, 101693, 101701, 101719,
    101723, 101737, 101741, 101747, 101749, 101771, 101789, 101797, 101807, 101833,
    101837, 101839, 101863, 101869, 101873, 101879, 101891, 101917, 101921, 101929,
    101939, 101957, 101963, 101977, 101987, 101999, 102001, 102013, 102019, 102023,
    102031, 102043, 102059, 102061, 102071, 102077, 102079, 102101, 102103, 102107,
    102121, 102139, 102149, 102161, 102181, 102191, 102197, 102199, 102203, 102217,
    102229, 102233, 102241, 102251, 102253, 102259, 102293, 102299, 102301, 102317,
    102329, 102337, 102359, 102367, 102397, 102407, 102409, 102433, 102437, 102451,
    102461, 102481, 102497, 102499, 102503, 102523, 102533, 102539, 102547, 102551,
    102559, 102563, 102587, 102593, 102607, 102611, 102643, 102647, 102653, 102667,
    102673, 102677, 102679, 102701, 102761, 102763, 102769, 102793, 102797, 102811,
    102829, 102841, 102859, 102871, 102877, 102881, 102911, 102913, 102929, 102931,
    102953, 102967, 102983, 103001, 103007, 103043, 103049, 103067, 103069, 103079,
    103087, 103091, 103093, 103099, 103123, 103141, 103171, 103177, 103183, 103217,
    103231, 103237, 103289, 103291, 103307, 103319, 103333, 103349, 103357, 103387,
    103391, 103393, 103399, 103409, 103421, 103423, 103451, 103457, 103471, 103483,
    103511, 103529, 103549, 103553, 103561, 103567, 103573, 103577, 103583, 103591,
    103613, 103619, 103643, 103651, 103657, 103669, 103681, 103687, 103699, 103703,
    103723, 103769, 103787, 103801, 103811, 103813, 103837, 103841, 103843, 103867,
    103889, 103903, 103913, 103919, 103951, 103963, 103967, 103969, 103979, 103981,
    103991, 103993, 103997, 104003, 104009, 104021, 104033, 104047, 104053, 104059,
    104087, 104089, 104107, 104113, 104119, 104123, 104147, 104149, 104161, 104173,
    104179, 104183, 104207, 104231, 104233, 104239, 104243, 104281, 104287, 104297,
    104309, 104311, 104323, 104327, 104347, 104369, 104381, 104383, 104393, 104399,
    104417, 104459, 104471, 104473, 104479, 104491, 104513, 104527, 104537, 104543,
    104549, 104551, 104561, 104579, 104593, 104597, 104623, 104639, 104651, 104659,
    104677, 104681, 104683, 104693, 104701, 104707, 104711, 104717, 104723, 104729,
]


def get_cmdline():
    parser = argparse.ArgumentParser('FixParser Perfect Hash Generator')
    parser.add_argument('-f', '--parser-def', required=True,
                        help='Parser definition file.')
    parser.add_argument('-d', '--fix-defs', default='fix_tags.txt',
                        help='FIX tag definition file.')
    parser.add_argument('-t', '--power-of-two', action='store_true',
                        help='Force M to be power of 2.')
    parser.add_argument('-m', '--print-mapping', action='store_true',
                        help='Print mapping.')
    return parser.parse_args()


def read_fix_tags_from_handle(f):
    result = {}
    for line in f:
        line = line.strip()
        if not line or line[0] == '#':
            continue
        parts = line.split()
        if parts < 2:
            raise Exception("Error: malformed line: '%s'" % line)
        tag, name = parts[0:2]
        result[name] = int(tag)
    return result


def read_fix_tags(filename):
    with open(filename, 'r') as f:
        return read_fix_tags_from_handle(f)


def read_parser_def_from_handle(f, fix_tags):
    result = {}
    for line in f:
        line = line.strip()
        if not line or line[0] == '#':
            continue
        parts = line.split()
        if len(parts) == 1:
            try:
                value = fix_tags[parts[0]]
            except ValueError:
                raise ValueError("Unknown FIX tag: '%s'. Please add to fix_tags.txt" % parts[0])
            name = parts[0]
        elif len(parts) == 2:
            name, value = parts[0:2]
        else:
            raise Exception("Error: malformed line: '%s'" % line)
        name = str(name)
        value = int(value)
        if name in result:
            raise ValueError("Duplicate tag name: '%s'" % name)
        result[name] = value
    # check for duplicated values
    values = {}
    for tag1, value in result.iteritems():
        try:
            tag2 = values[value]
            raise Exception("Duplicated tag value %d between tags %s and %s" % (value, tag1, tag2))
        except KeyError:
            values[value] = tag1
    return result


def read_parser_def(filename, fix_tags):
    with open(filename, 'r') as f:
        return read_parser_def_from_handle(f, fix_tags)


def hash1(k, a, b, p, m):
    # result = (a*k + b) % p % m
    dtype = c_uint32
    r1 = dtype(a*k)
    r2 = dtype(r1.value + b)
    r3 = dtype(r2.value % p)
    r4 = dtype(r3.value % m)
    return int(r4.value)


def find_next_power_of_2(x):
    return int(2**(math.ceil(math.log(x, 2))))


def no_collisions(hashed):
    return len(set(hashed)) == len(hashed)


def output_mapping(values, hashed):
    print("Mapping:")
    vv = dict(zip(hashed, values))
    for v in sorted(vv.keys()):
        print("% 3d -> % 3d" % (v, vv[v]))


def solve2(parser, values, m, print_mapping=True):
    bits = math.ceil(math.log(m, 2))
    print("%d requires %d bits" % (m, bits))
    M = find_next_power_of_2(m)
    for i in range(3):
        for p in PRIMES:
            def h(k):
                dtype = c_uint32
                r1 = dtype(k*p)
                r2 = dtype(r1.value % M)
                # return (k^p) % M
                return r2.value

            hashed = [h(v) for v in values]
            success = no_collisions(hashed)
            if success:
                print("Success!")
                print("P = %d, M = %d, N = %d" % (p, M, len(values)))
                if print_mapping:
                    output_mapping(values, hashed)
                return True
        M = find_next_power_of_2(M+1)
    return False


def solve(parser, values, m, iters=10000, print_mapping=False):
    for i in range(50):
        p = PRIMES[random.randint(0, len(PRIMES) - 1)]
        for _ in range(iters):
            a = b = random.randint(0, maxval)
            while b == a:
                b = random.randint(0, maxval)
            hashed = [hash1(k=x, a=a, b=b, p=p, m=m) for x in values]
            # no_collisions = len(set(hashed)) == len(hashed)
            success = no_collisions(hashed)
            if success:
                print("Success!")
                print("A = %d, B = %d, P = %d, M = %d, N = %d" % (a, b, p, m, len(values)))
                if print_mapping:
                    output_mapping(values, hashed)
                return True
    return False


if __name__ == '__main__':
    args = get_cmdline()

    fix_tags = read_fix_tags(args.fix_defs)
    parser = read_parser_def(args.parser_def, fix_tags)

    values = list(parser.values())
    maxval = (1 << 32) - 1

    m = len(values)
    if args.power_of_two:
        m = find_next_power_of_2(m)

    if not solve2(parser, values, m=m, print_mapping=args.print_mapping):
        print("Failed version 2 :(")

    if not solve(parser, values, m=m, print_mapping=args.print_mapping):
        print("Failed :(")
