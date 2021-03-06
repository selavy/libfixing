#!/usr/bin/env python

import argparse
import random
import math
from ctypes import c_uint32 #, c_uint64

DTYPE = c_uint32


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
    parser.add_argument('tags', nargs='+', help='FIX tags.')
    parser.add_argument('-t', '--power-of-two', action='store_true',
                        help='Force M to be power of 2.')
    return parser.parse_args()


def find_next_power_of_2(x):
    return int(2**(math.ceil(math.log(x, 2))))


def solve1(m, tags):
    print("\nSOLVER1")

    def pickprime():
        i = random.randint(0, len(PRIMES)-1)
        return PRIMES[i]

    def picknum():
        return random.randint(0, (1 << 32) - 1)

    for i in range(100):
        p = pickprime()
        for j in range(10000):
            a = b = picknum()
            while a == b:
                b = picknum()

            def h(k):
                r1 = DTYPE(a*k)
                r2 = DTYPE(r1.value + b)
                return r2.value % p % m

            hashed = tuple(h(k) for k in tags)
            success = len(hashed) == len(set(hashed))
            if success:
                print("A = %d, B = %d, P = %d, M = %d" % (a, b, p, m))
                for tag, hh in zip(tags, hashed):
                    print("%d -> %d" % (tag, hh))
                return

    print("Failed.")


# static U32 XXH32_avalanche(U32 h32)
# {
#     h32 ^= h32 >> 15;
#     h32 *= PRIME32_2;
#     h32 ^= h32 >> 13;
#     h32 *= PRIME32_3;
#     h32 ^= h32 >> 16;
#     return(h32);
# }

# static U64 XXH64_avalanche(U64 h64)
# {
#     h64 ^= h64 >> 33;
#     h64 *= PRIME64_2;
#     h64 ^= h64 >> 29;
#     h64 *= PRIME64_3;
#     h64 ^= h64 >> 32;
#     return h64;
# }

def solve2(m, tags):
    print("\nSOLVER2")

    def pickprime():
        return PRIMES[random.randint(0, len(PRIMES)-1)]

    def pickshift():
        return random.randint(1, 31)

    for i in range(100):
        p1 = pickprime()
        p2 = pickprime()

        for j in range(100):
            s1 = pickshift()
            s2 = pickshift()
            s3 = pickshift()

            def h(k):
                c1 = DTYPE(k ^ DTYPE(k >> s1).value)
                c2 = DTYPE(c1.value * p1)
                c3 = DTYPE(c2.value ^ DTYPE(c2.value >> s2).value)
                c4 = DTYPE(c3.value * p2)
                c5 = DTYPE(c4.value ^ DTYPE(c4.value >> s3).value)
                return c5.value % m

            hashed = tuple(h(k) for k in tags)
            success = len(hashed) == len(set(hashed))
            if success:
                print("P1 = %d, P2 = %d, S1 = %d, S2 = %d, S3 = %d, M = %d" %
                      (p1, p2, s1, s2, s3, m))
                for tag, hh in zip(tags, hashed):
                    print("%d -> %d" % (tag, hh))
                return

    print("Failed.")


def solve3(m, tags):
    print("\nSOLVER3")

    def pickprime():
        return PRIMES[random.randint(0, len(PRIMES)-1)]

    def pickshift():
        return random.randint(1, 31)

    for i in range(100):
        p1 = pickprime()

        for j in range(100):
            s1 = pickshift()
            s2 = pickshift()

            def h(k):
                c1 = DTYPE(k ^ DTYPE(k >> s1).value)
                c2 = DTYPE(c1.value * p1)
                c3 = DTYPE(c2.value ^ DTYPE(c2.value >> s2).value)
                return c3.value % m

            hashed = tuple(h(k) for k in tags)
            success = len(hashed) == len(set(hashed))
            if success:
                print("P = %d, A = %d, B = %d, M = %d" % (p1, s1, s2, m))
                for tag, hh in zip(tags, hashed):
                    print("%d -> %d" % (tag, hh))
                return

    print("Failed.")


def solve4(m, tags):
    print("\nSOLVER4")

    def pickprime():
        return PRIMES[random.randint(0, len(PRIMES)-1)]

    def pickshift():
        return random.randint(1, 31)

    for i in range(100):
        p1 = pickprime()

        for j in range(100):
            s1 = pickshift()

            def h(k):
                c1 = DTYPE(k * p1)
                c2 = DTYPE(c1.value ^ DTYPE(c1.value >> s1).value)
                return c2.value % m

            hashed = tuple(h(k) for k in tags)
            success = len(hashed) == len(set(hashed))
            if success:
                print("P = %d, A = %d, M = %d" % (p1, s1, m))
                for tag, hh in zip(tags, hashed):
                    print("%d -> %d" % (tag, hh))
                return
    print("Failed.")


def solve5(m, tags):
    print("\nSOLVER5")

    def pickshift():
        return random.randint(1, 31)

    for i in range(1000):
        s1 = pickshift()
        s2 = pickshift()

        def h(k):
            c1 = DTYPE(k ^ DTYPE(k >> s1).value)
            c2 = DTYPE(c1.value ^ DTYPE(c1.value >> s2).value)
            return c2.value % m

        hashed = tuple(h(k) for k in tags)
        success = len(hashed) == len(set(hashed))
        if success:
            print("A = %d, B = %d, M = %d" % (s1, s2, m))
            for tag, hh in zip(tags, hashed):
                print("%d -> %d" % (tag, hh))
            return

    print("Failed.")


# def solve6(m, tags):
#     print("\nSOLVER6")
# 
#     def pickprime():
#         return PRIMES[random.randint(0, len(PRIMES)-1)]
# 
#     for i in range(10000):
#         p1 = pickprime()
#         p2 = pickprime()
# 
#         def h(k):
#             c1 = DTYPE(k * p1)
#             c2 = DTYPE(c1.value * p2)
#             return c2.value % m
# 
#         hashed = tuple(h(k) for k in tags)
#         success = len(hashed) == len(set(hashed))
#         if success:
#             print("P1 = %d, P2 = %d, M = %d" % (p1, p2, m))
#             for tag, hh in zip(tags, hashed):
#                 print("%d -> %d" % (tag, hh))
#             return
# 
#     print("Failed.")


if __name__ == '__main__':
    args = get_cmdline()
    tags = tuple(sorted({int(x) for x in args.tags}))
    m = len(tags)
    if args.power_of_two:
        m = find_next_power_of_2(m)

    solve1(m, tags)
    solve2(m, tags)
    solve3(m, tags)
    solve4(m, tags)
    solve5(m, tags)
    # solve6(m, tags)

